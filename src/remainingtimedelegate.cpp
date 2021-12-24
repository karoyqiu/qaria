/*! ***********************************************************************************************
 *
 * \file        remainingtimedelegate.cpp
 * \brief       RemainingTimeDelegate 类源文件。
 *
 * \version     1.0
 * \date        2021-12-01
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#include "remainingtimedelegate.h"


QString RemainingTimeDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    Q_UNUSED(locale);
    static const int SECONDS_PER_MIN = 60;
    static const int SECONDS_PER_HOUR = 60 * SECONDS_PER_MIN;
    static const int SECONDS_PER_DAY = 24 * SECONDS_PER_HOUR;

    auto time = value.toInt();

    if (time < 0)
    {
        return QS("∞");
    }

    auto d = time / SECONDS_PER_DAY;
    time = time % SECONDS_PER_DAY;
    auto h = time / SECONDS_PER_HOUR;
    time = time % SECONDS_PER_HOUR;
    auto m = time / SECONDS_PER_MIN;
    time = time % SECONDS_PER_MIN;

    QStringList parts;

    if (d > 0)
    {
        parts << tr("%Ln day(s)", nullptr, d);
        parts << tr("%1 hour(s)", nullptr, h).arg(h, 2, 10, QL('0'));
        parts << tr("%1 minute(s)", nullptr, m).arg(m, 2, 10, QL('0'));
        parts << tr("%1 second(s)", nullptr, time).arg(time, 2, 10, QL('0'));
    }
    else if (h > 0)
    {
        parts << tr("%Ln hour(s)", nullptr, h);
        parts << tr("%1 minute(s)", nullptr, m).arg(m, 2, 10, QL('0'));
        parts << tr("%1 second(s)", nullptr, time).arg(time, 2, 10, QL('0'));
    }
    else if (m > 0)
    {
        parts << tr("%Ln minute(s)", nullptr, m);
        parts << tr("%1 second(s)", nullptr, time).arg(time, 2, 10, QL('0'));
    }
    else
    {
        parts << tr("%Ln second(s)", nullptr, time);
    }

    return parts.join(QL(' '));
}
