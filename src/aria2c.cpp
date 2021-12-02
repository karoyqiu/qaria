﻿/*! ***********************************************************************************************
 *
 * \file        aria2c.cpp
 * \brief       Aria2c 类源文件。
 *
 * \version     1.0
 * \date        2021-11-30
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#include "aria2c.h"
#include "aria2optionsbuilder.h"

#define GET(item, obj, key)     get(item.key, obj, QS(#key))

using std::placeholders::_1;

template<typename T>
static inline void get(T &value, const QVariantHash &obj, const QString &key)
{
    value = obj.value(key).value<T>();
}

static QString toString(DownloadStatus status)
{
    switch (status)
    {
    case DownloadStatus::Active:
        return QS("active");
    case DownloadStatus::Waiting:
        return QS("waiting");
    case DownloadStatus::Paused:
        return QS("paused");
    case DownloadStatus::Error:
        return QS("error");
    case DownloadStatus::Complete:
        return QS("complete");
    case DownloadStatus::Removed:
        return QS("removed");
    default:
        Q_UNREACHABLE();
    }
}

static DownloadStatus toStatus(const QString &s)
{
    static const QMap<QString, DownloadStatus> map{
        { QS("active"), DownloadStatus::Active },
        { QS("waiting"), DownloadStatus::Waiting },
        { QS("paused"), DownloadStatus::Paused },
        { QS("error"), DownloadStatus::Error },
        { QS("complete"), DownloadStatus::Complete },
        { QS("removed"), DownloadStatus::Removed },
    };

    Q_ASSERT(map.contains(s));
    return map.value(s);
}

static DownloadItem toItem(const QVariant &var)
{
    DownloadItem item;
    const auto obj = var.toHash();
    GET(item, obj, gid);
    GET(item, obj, totalLength);
    GET(item, obj, completedLength);
    GET(item, obj, uploadLength);
    GET(item, obj, downloadSpeed);
    GET(item, obj, uploadSpeed);
    GET(item, obj, errorCode);
    GET(item, obj, errorMessage);
    GET(item, obj, dir);

    auto status = obj.value(QS("status")).toString();
    item.status = toStatus(status);

    const auto files = obj.value(QS("files")).toList();

    for (const auto &f : files)
    {
        DownloadFile df;
        const auto fobj = f.toHash();
        GET(df, fobj, index);
        GET(df, fobj, path);
        GET(df, fobj, length);
        GET(df, fobj, completedLength);
        GET(df, fobj, selected);
        GET(df, fobj, index);

        item.files.append(df);
    }

    return item;
}

static inline void dontCare(const QVariant &) { }


Aria2c::Aria2c(QObject *parent /*= nullptr*/)
    : QObject(parent)
    , ws_(nullptr)
{
}


void Aria2c::start()
{
    Q_ASSERT(ws_ == nullptr);

    QSettings settings;
    auto aria2c = settings.value(QS("aria2c")).toString();

    QDir appDataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    appDataDir.mkpath(QS("."));
    auto sessionFile = appDataDir.absoluteFilePath(QS("aria2.session"));

#ifdef QT_DEBUG
    auto logfile = QDir::temp().absoluteFilePath(QS("aria2c.log"));
    QFile::remove(logfile);
#endif

    secret_ = generateToken();

    QStringList args{
        QS("--enable-rpc"),
        QS("--pause-metadata"),
        QS("--rpc-secret"), secret_,
        QS("--save-session"), sessionFile,
        QS("--save-session-interval"), QS("60"),
        QS("--stop-with-process"), QSS(qApp->applicationPid()),
        QS("--quiet"),
#ifdef QT_DEBUG
        QS("--log"), logfile,
#endif
    };

    if (QFileInfo::exists(sessionFile))
    {
        args.append({ QS("--input-file"), sessionFile });
    }

    QProcess::startDetached(aria2c, args);


    ws_ = new QWebSocket(qApp->applicationName(), QWebSocketProtocol::VersionLatest, this);
    connect(ws_, &QWebSocket::connected, this, &Aria2c::onConnected);
    connect(ws_, &QWebSocket::textMessageReceived, this, &Aria2c::handleMessage);

    ws_->open(QS("ws://127.0.0.1:6800/jsonrpc"));
}


void Aria2c::addUri(const QString &uris, const QVariantHash &options /*= {}*/)
{
    callAsync(std::bind(&Aria2c::handleAdd, this, _1),
              QS("aria2.addUri"), QStringList{ uris }, options);
}


void Aria2c::remove(const QString &gid)
{
    callAsync([this](const QVariant result)
    {
        emit removed(result.toString());
    }, QS("aria2.remove"), gid);
}


void Aria2c::tellAll()
{
    auto handler = std::bind(&Aria2c::handleTellDownload, this, _1);
    callAsync(handler, QS("aria2.tellActive"));
    callAsync(handler, QS("aria2.tellWaiting"), 0, 1024);
    callAsync(handler, QS("aria2.tellStopped"), 0, 1024);
}


QString Aria2c::generateToken()
{
    return QUuid::createUuid().toString(QUuid::Id128);
}


void Aria2c::onConnected()
{
    // 设置全局选项
    QSettings settings;
    OptionsBuilder opts;
    opts.setDir(settings.value(QS("dir")).toString());
    opts.setPauseMetadata(true);
    callAsync(dontCare, QS("aria2.changeGlobalOption"), opts.options());

    tellAll();
}


void Aria2c::handleMessage(const QString &msg)
{
#ifdef QT_DEBUG
    qDebug() << ">>>" << qUtf8Printable(msg);
#endif

    const auto json = QJsonDocument::fromJson(msg.toUtf8());
    const auto obj = json.object().toVariantHash();

    if (obj.contains(QS("id")))
    {
        auto id = obj.value(QS("id")).toString();
        Q_ASSERT(calls_.contains(id));

        auto handler = calls_.take(id);
        handler(obj.value(QS("result")));
    }
    else
    {
        const auto method = obj.value(QS("method")).toString();
        Q_ASSERT(!method.isEmpty());
        handleNotification(method, obj.value(QS("params")).toList());
    }
}


void Aria2c::handleNotification(const QString &method, const QVariantList &params)
{
    Q_ASSERT(params.length() == 1);
    const auto gid = params.first().toString();

    if (method == QL("aria2.onDownloadStart"))
    {
        emit started(gid);
    }
    else if (method == QL("aria2.onDownloadPause"))
    {
        emit paused(gid);
    }
    else if (method == QL("aria2.onDownloadStop"))
    {
        emit stopped(gid);
    }
    else if (method == QL("aria2.onDownloadComplete"))
    {
        emit completed(gid);
    }
    else if (method == QL("aria2.onDownloadError"))
    {
        emit failed(gid);
    }
    else if (method == QL("aria2.onBtDownloadComplete"))
    {
        emit btCompleted(gid);
    }
    else
    {
        qWarning() << "Unknown notification" << method << params;
    }
}


void Aria2c::send(const QJsonDocument &doc)
{
    auto json = doc.toJson(QJsonDocument::Compact);
    auto s = QSS(json);

#ifdef QT_DEBUG
    qDebug() << "<<<" << qUtf8Printable(s);
#endif

    ws_->sendTextMessage(s);
}


void Aria2c::handleAdd(const QVariant &result)
{
    auto gid = result.toString();
    callAsync([this](const QVariant &var)
    {
        auto item = toItem(var);
        emit added({ item });
    }, QS("aria2.tellStatus"), gid);
}


void Aria2c::handleTellDownload(const QVariant &result)
{
    const auto list = result.toList();
    DownloadItems items;

    for (const auto &var : list)
    {
        items.append(toItem(var));
    }

    emit added(items);
}
