/*! ***********************************************************************************************
 *
 * \file        qaria2host.h
 * \brief       QAria2Host 类头文件。
 *
 * \version     1.0
 * \date        2022-01-06
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2022 Roy QIU。
 *
 **************************************************************************************************/
#pragma once
#include <QObject>


class QAria2Host : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

public slots:
    void addUris(const QStringList &uris)
    {
        emit urisAddRequested(uris);
    }

signals:
    void urisAddRequested(const QStringList &uris);
};
