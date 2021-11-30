/*! ***********************************************************************************************
 *
 * \file        aria2c.h
 * \brief       Aria2c 类头文件。
 *
 * \version     1.0
 * \date        2021-11-30
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#pragma once
#include <QObject>

class QProcess;


class Aria2c : public QObject
{
    Q_OBJECT

public:
    explicit Aria2c(QObject *parent = nullptr);

    void start();

private:
    static QString generateToken();

private:
    QProcess *aria2c_;
    QString secret_;
};
