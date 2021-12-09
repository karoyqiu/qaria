/*! ***********************************************************************************************
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
    GET(item, obj, following);
    GET(item, obj, followedBy);
    GET(item, obj, infoHash);

    auto status = obj.value(QS("status")).toString();
    item.status = toStatus(status);

    const auto bt = obj.value(QS("bittorrent")).toHash();
    GET(item.bittorrent, bt, comment);
    GET(item.bittorrent, bt, creationDate);
    GET(item.bittorrent, bt, mode);
    item.bittorrent.name = bt.value(QS("info")).toHash().value(QS("name")).toString();

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
    , tellingTimer_(nullptr)
{
}


void Aria2c::start()
{
    Q_ASSERT(ws_ == nullptr && tellingTimer_ == nullptr);

    QSettings settings;
    auto aria2c = settings.value(QS("aria2c")).toString();

    QDir appDataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    appDataDir.mkpath(QS("."));
    auto sessionFile = appDataDir.absoluteFilePath(QS("aria2.session"));

#ifdef QT_DEBUG
    auto logfile = QDir::temp().absoluteFilePath(QS("aria2c.log"));
    QFile::remove(logfile);

    secret_ = QS("03f682c8b4f94201995da1471ec47f23");
#else
    secret_ = generateToken();
#endif

    QStringList args{
        QS("--enable-rpc"),
        QS("--rpc-secret"), secret_,
        QS("--save-session"), sessionFile,
        QS("--stop-with-process"), QSS(qApp->applicationPid()),
        QS("--daemon"),
        QS("--quiet"),
#ifdef QT_DEBUG
        //QS("--log"), logfile,
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
    BatchMethods methods;
    methods.append({ QS("aria2.tellActive"), {} });
    methods.append({ QS("aria2.tellWaiting"), { 0, 1024 } });
    methods.append({ QS("aria2.tellStopped"), { 0, 1024 } });
    batchCall(handler, methods);
}


void Aria2c::pause(const QStringList &gids)
{
    BatchMethods methods;

    for (const auto &gid : gids)
    {
        methods.append({ QS("aria2.pause"), { gid } });
    }

    batchCall(dontCare, methods);
}


void Aria2c::resume(const QStringList &gids)
{
    BatchMethods methods;

    for (const auto &gid : gids)
    {
        methods.append({ QS("aria2.unpause"), { gid } });
    }

    batchCall(dontCare, methods);
}


void Aria2c::changeOption(const QString &gid, const QVariantHash &options)
{
    callAsync(dontCare, QS("aria2.changeOption"), gid, options);
}


void Aria2c::setBtTrackers(const QStringList &trackers)
{
    OptionsBuilder builder;
    builder.setBtTracker(trackers.join(QL(',')));
    callAsync(dontCare, QS("aria2.changeGlobalOption"), builder.options());
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
    opts.setPauseMetadata();
    opts.setAutoSaveInterval(60);
    opts.setSaveSessionInterval(60);
    opts.setBtSaveMetadata();
    opts.setBtEnableLpd();
    opts.setBtLoadSavedMetadata();
    opts.setBtRemoveUnselectedFile();
    opts.setMaxConnectionPerServer(8);
    opts.setMinSplitSize(1_M);
    opts.setFileAllocation(QS("falloc"));
    opts.setOptimizeConcurrentDownloads();
    callAsync(dontCare, QS("aria2.changeGlobalOption"), opts.options());

    emit aria2Started();

    Q_ASSERT(tellingTimer_ == nullptr);
    tellingTimer_ = new QTimer(this);
    tellingTimer_->setInterval(1000);
    tellingTimer_->setTimerType(Qt::VeryCoarseTimer);
    tellingTimer_->setSingleShot(true);
    connect(tellingTimer_, &QTimer::timeout, this, &Aria2c::tellAll);
    tellingTimer_->start();
}


void Aria2c::handleMessage(const QString &msg)
{
#ifdef QT_DEBUG
    qDebug() << ">>>" << qUtf8Printable(msg);
#endif

    const auto json = QJsonDocument::fromJson(msg.toUtf8());

    if (json.isObject())
    {
        const auto obj = json.object().toVariantHash();

        if (obj.contains(QS("id")))
        {
            auto id = obj.value(QS("id")).toString();

            if (!calls_.contains(id))
            {
                return;
            }

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
    else if (json.isArray())
    {
        // Batch call
        const auto list = json.array().toVariantList();
        QVariantList results;
        QString id;

        for (const auto &var : list)
        {
            const auto obj = var.toHash();

            if (id.isEmpty())
            {
                id = obj.value(QS("id")).toString();

                if (!calls_.contains(id))
                {
                    return;
                }
            }

            results.append(obj.value(QS("result")));
        }

        auto handler = calls_.take(id);
        handler(results);
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


QStringList Aria2c::batchCall(MessageHandler handler, const BatchMethods &methods)
{
    QStringList ids;
    QJsonArray arr;

    for (const auto &m : methods)
    {
        auto params = m.second;
        params.prepend(QString(QL("token:") % secret_));

        auto id = generateToken();
        ids.append(id);

        QJsonObject obj{
            { QS("jsonrpc"), QS("2.0") },
            { QS("id"), id },
            { QS("method"), m.first },
            { QS("params"), QJsonArray::fromVariantList(params) },
        };
        arr.append(obj);
    }

    calls_.insert(ids.first(), handler);

    send(QJsonDocument(arr));
    return ids;
}


void Aria2c::handleAdd(const QVariant &result)
{
    auto gid = result.toString();
    callAsync([this](const QVariant &var)
    {
        auto item = toItem(var);
        emit changed({ item });
    }, QS("aria2.tellStatus"), gid);
}


void Aria2c::handleTellDownload(const QVariant &result)
{
    const auto list = result.toList();
    DownloadItems all;

    for (const auto &var : list)
    {
        const auto sublist = var.toList();
        DownloadItems items;

        for (const auto &subvar : sublist)
        {
            items.append(toItem(subvar));
        }

        all.append(items);
    }

    emit changed(all);

    tellingTimer_->start();
}
