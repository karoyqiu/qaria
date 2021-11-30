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

    secret_ = generateToken();

    QStringList args{
        QS("--enable-rpc"),
        QS("--pause"),
        QS("--pause-metadata"),
        QS("--rpc-secret"), secret_,
        QS("--save-session"), sessionFile,
        QS("--save-session-interval"), QS("60"),
        QS("--stop-with-process"), QSS(qApp->applicationPid()),
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


QString Aria2c::generateToken()
{
    return QUuid::createUuid().toString(QUuid::Id128);
}


void Aria2c::onConnected()
{
    callAsync(QS("aria2.getGlobalOption"));
}


void Aria2c::handleMessage(const QString &msg)
{
    qDebug() << qUtf8Printable(msg);
}


void Aria2c::send(const QJsonDocument &doc)
{
    auto json = doc.toJson(QJsonDocument::Compact);
    ws_->sendTextMessage(QSS(json));
}
