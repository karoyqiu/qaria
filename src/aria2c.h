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
class QWebSocket;


class Aria2c : public QObject
{
    Q_OBJECT

public:
    explicit Aria2c(QObject *parent = nullptr);

    void start();

private:
    static QString generateToken();

    void onConnected();
    void handleMessage(const QString &msg);

    void send(const QJsonDocument &doc);

    template<typename... T>
    void callAsync(const QString &method, T&&... args);

    void toQVariantList(QVariantList & /*result*/) {}

    template<typename T>
    void toQVariantList(QVariantList &result, T &&x);

    template<typename T, typename... Ts>
    void toQVariantList(QVariantList &result, T &&head, Ts&&... tail);

private:
    QProcess *aria2c_;
    QWebSocket *ws_;
    QString secret_;
};


template<typename... T>
void Aria2c::callAsync(const QString &method, T&&... args)
{
    QVariantList params;
    toQVariantList(params, std::forward<T>(args)...);
    params.prepend(QString(QL("token:") % secret_));

    QJsonObject obj{
        { QS("jsonrpc"), QS("2.0") },
        { QS("id"), generateToken() },
        { QS("method"), method },
        { QS("params"), QJsonArray::fromVariantList(params) },
    };

    send(QJsonDocument(obj));
}


template<typename T>
void Aria2c::toQVariantList(QVariantList &result, T &&x)
{
    result.prepend(x);
}


template<typename T, typename... Ts>
void Aria2c::toQVariantList(QVariantList &result, T &&head, Ts&&... tail)
{
    toQVariantList(result, std::forward<Ts>(tail)...);
    result.prepend(head);
}
