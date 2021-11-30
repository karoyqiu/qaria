/*! ***********************************************************************************************
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
        case NumberColumn:
            var = tr("#");
            break;
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

    if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole)
    {
    }

    return var;
}
