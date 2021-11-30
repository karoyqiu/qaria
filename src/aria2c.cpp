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
    , aria2c_(nullptr)
{
}


void Aria2c::start()
{
    Q_ASSERT(aria2c_ == nullptr);

    QSettings settings;
    auto aria2c = settings.value(QS("aria2c")).toString();

    aria2c_ = new QProcess(this);
    aria2c_->setStandardOutputFile(QProcess::nullDevice());
    aria2c_->setStandardErrorFile(QProcess::nullDevice());

    //QDir appDataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    //appDataDir.mkpath(QS("."));
    //auto sessionFile = appDataDir.absoluteFilePath(QS("aria2.session"));

    secret_ = generateToken();

    QStringList args{
        QS("--enable-rpc"),
        QS("--pause"),
        QS("--pause-metadata"),
        QS("--rpc-secret"), secret_,
        //QS("--input-file"), sessionFile,
        //QS("--save-session"), sessionFile,
    };

    aria2c_->start(aria2c, args);
}


QString Aria2c::generateToken()
{
    return QUuid::createUuid().toString(QUuid::Id128);
}
