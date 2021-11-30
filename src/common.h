﻿/*! ***********************************************************************************************
 *
 * \file        common.h
 * \brief       公共头文件。
 *
 * \version     1.0
 * \date        2021-09-17
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2021 karoyqiu。
 *
 **************************************************************************************************/
#pragma once
#include <QString>

#define QS(x)   QStringLiteral(x)


inline QLatin1String QL(const char *s)
{
    return QLatin1String(s);
}

inline QLatin1String QL(const QByteArray &s)
{
    return QLatin1String(s);
}

inline QLatin1String QL(const char *f, const char *l)
{
    return QLatin1String(f, l);
}

inline QLatin1String QL(const char *s, int n)
{
    return QLatin1String(s, n);
}

inline QLatin1Char QL(char ch)
{
    return QLatin1Char(ch);
}

inline QString QSS(const std::string &s)
{
    return QString::fromStdString(s);
}

inline QString QSS(const std::wstring &s)
{
    return QString::fromStdWString(s);
}

inline QString QSS(int number)
{
    return QString::number(number, 10);
}
