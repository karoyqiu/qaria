/*! ***********************************************************************************************
 *
 * \file        statusdelegate.cpp
 * \brief       StatusDelegate 类源文件。
 *
 * \version     1.0
 * \date        2021-12-06
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#include "statusdelegate.h"
#include "aria2c.h"


StatusDelegate::StatusDelegate(QObject *parent /*= nullptr*/)
    : QStyledItemDelegate(parent)
{
}


QString StatusDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    Q_UNUSED(locale);
    auto status = static_cast<DownloadStatus>(value.toInt());

    switch (status)
    {
    case DownloadStatus::Active:
        return tr("Active");
    case DownloadStatus::Waiting:
        return tr("Waiting");
    case DownloadStatus::Paused:
        return tr("Paused");
    case DownloadStatus::Error:
        return tr("Error");
    case DownloadStatus::Complete:
        return tr("Complete");
    case DownloadStatus::Removed:
        return tr("Removed");
    default:
        Q_UNREACHABLE();
    }
}
