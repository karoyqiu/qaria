﻿/*! ***********************************************************************************************
 *
 * \file        downloadtablemodel.cpp
 * \brief       DownloadTableModel 类源文件。
 *
 * \version     1.0
 * \date        2021-11-30
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#include "downloadtablemodel.h"


static QString itemName(const DownloadItem &item)
{
    if (!item.bittorrent.name.isEmpty())
    {
        return item.bittorrent.name;
    }

    Q_ASSERT(!item.files.isEmpty());
    return item.files.first().path;
}


DownloadTableModel::DownloadTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}


QVariant DownloadTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant var;

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case NameColumn:
            var = tr("Name");
            break;
        case SizeColumn:
            var = tr("Size");
            break;
        case ProgressColumn:
            var = tr("Progress");
            break;
        case StatusColumn:
            var = tr("Status");
            break;
        case DownloadSpeedColumn:
            var = tr("Download Speed");
            break;
        case UploadSpeedColumn:
            var = tr("Upload Speed");
            break;
        case RemainingTimeColumn:
            var = tr("Remaining Time");
            break;
        case CreationTimeColumn:
            var = tr("Creation Time");
            break;
        case FinishTimeColumn:
            var = tr("Finish Time");
            break;
        case ColumnCount:
        default:
            break;
        }
    }
    else
    {
        var = QAbstractTableModel::headerData(section, orientation, role);
    }

    return var;
}


QVariant DownloadTableModel::data(const QModelIndex &idx, int role) const
{
    Q_ASSERT(checkIndex(idx, CheckIndexOption::IndexIsValid | CheckIndexOption::ParentIsInvalid));

    QVariant var;

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
    case Qt::ToolTipRole:
        {
            const auto &item = items_.at(idx.row());

            switch (idx.column())
            {
            case NameColumn:
                var = itemName(item);
                break;
            case SizeColumn:
                var = item.totalLength;
                break;
            case ProgressColumn:
                var = item.totalLength > 0 ? item.completedLength * 100 / item.totalLength : 0;
                break;
            case StatusColumn:
                var = static_cast<int>(item.status);
                break;
            case DownloadSpeedColumn:
                var = item.downloadSpeed;
                break;
            case UploadSpeedColumn:
                var = item.uploadSpeed;
                break;
            case RemainingTimeColumn:
                var = item.downloadSpeed > 0 ? (item.totalLength - item.completedLength) / item.downloadSpeed : 0;
                break;
            case CreationTimeColumn:
                //var = item;
                break;
            case FinishTimeColumn:
                //var = tr("Finish Time");
                break;
            }
        }
        break;

    case Qt::TextAlignmentRole:
        switch (idx.column())
        {
        case SizeColumn:
        case ProgressColumn:
        case DownloadSpeedColumn:
        case UploadSpeedColumn:
        case RemainingTimeColumn:
        case CreationTimeColumn:
        case FinishTimeColumn:
            var = static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);
            break;
        case StatusColumn:
            var = Qt::AlignCenter;
            break;
        }
        break;

    case GidRole:
        {
            const auto &item = items_.at(idx.row());
            var = item.gid;
        }
        break;
    }

    return var;
}


void DownloadTableModel::reset(const DownloadItems &items)
{
    DownloadItems insert;
    QSet<int> updatedIndexes;

    // 更新相同的
    for (const auto &item : items)
    {
        if (!item.followedBy.isEmpty())
        {
            // 跳过生成子任务的
            continue;
        }

        bool updated = false;

        for (int i = 0; i < items_.count(); i++)
        {
            auto &e = items_[i];

            if (e.gid == item.gid)
            {
                e = item;
                updated = true;
                updatedIndexes.insert(i);

                auto first = index(i, 0);
                auto last = first.siblingAtRow(ColumnCount - 1);
                emit dataChanged(first, last);

                break;
            }
        }

        if (!updated)
        {
            insert.append(item);
        }
    }

    // 删除没有的
    if (updatedIndexes.count() < items_.count())
    {
        for (int i = items_.count() - 1; i >= 0; i--)
        {
            if (!updatedIndexes.contains(i))
            {
                beginRemoveRows({}, i, i);
                items_.removeAt(i);
                endRemoveRows();
            }
        }
    }

    // 添加剩下的
    if (!insert.isEmpty())
    {
        beginInsertRows({}, items_.count(), items_.count() + insert.count() - 1);
        items_.append(insert);
        endInsertRows();
    }
}


void DownloadTableModel::remove(const QString &gid)
{
    for (int i = 0; i < items_.count(); i++)
    {
        const auto &item = items_.at(i);

        if (item.gid == gid)
        {
            beginRemoveRows({}, i, i);
            items_.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}


const DownloadItem &DownloadTableModel::item(const QModelIndex &idx) const
{
    Q_ASSERT(checkIndex(idx, CheckIndexOption::IndexIsValid | CheckIndexOption::ParentIsInvalid));
    return items_.at(idx.row());
}


QString DownloadTableModel::fileSizeToString(qint64 bytes)
{
    static const QLatin1String units[] = {
        QL("B"),
        QL("KB"),
        QL("MB"),
        QL("GB"),
        QL("TB"),
        QL("PB"),
    };

    if (bytes < 1024)
    {
        return tr("%1 %2").arg(bytes).arg(units[0]);
    }

    auto n = qFloor(qLn(bytes) / qLn(1024));
    return tr("%1 %2").arg(bytes / qPow(1024, n), 0, 'f', 2).arg(units[n]);
}
