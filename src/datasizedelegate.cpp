/*! ***********************************************************************************************
 *
 * \file        datasizedelegate.cpp
 * \brief       DataSizeDelegate 类源文件。
 *
 * \version     1.0
 * \date        2021-12-01
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#include "datasizedelegate.h"


DataSizeDelegate::DataSizeDelegate(QObject *parent /*= nullptr*/)
    : DataSizeDelegate({}, parent)
{
}


DataSizeDelegate::DataSizeDelegate(const QString &suffix, QObject *parent /*= nullptr*/)
    : QStyledItemDelegate(parent)
    , suffix_(suffix)
{
}


QString DataSizeDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    auto size = value.toLongLong();
    auto text = locale.formattedDataSize(size);
    return text % suffix_;
}
