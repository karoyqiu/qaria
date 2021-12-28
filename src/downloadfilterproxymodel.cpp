/*! ***********************************************************************************************
 *
 * \file        downloadfilterproxymodel.cpp
 * \brief       DownloadFilterProxyModel 类源文件。
 *
 * \version     1.0
 * \date        2021-12-28
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 Roy QIU。
 *
 **************************************************************************************************/
#include "downloadfilterproxymodel.h"
#include "downloadtablemodel.h"


DownloadFilterProxyModel::DownloadFilterProxyModel(QObject *parent /*= nullptr*/)
    : QSortFilterProxyModel(parent)
    , status_(DownloadStatus::Any)
{
}


void DownloadFilterProxyModel::setStatus(DownloadStatus value)
{
    if (status_ != value)
    {
        status_ = value;
        invalidateFilter();
    }
}


bool DownloadFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent);

    if (status_ == DownloadStatus::Any)
    {
        return true;
    }

    auto *m = qobject_cast<DownloadTableModel *>(sourceModel());
    Q_ASSERT(m != nullptr);
    auto idx = m->index(sourceRow, 0, sourceParent);
    const auto &item = m->item(sourceRow);
    return item.status == status_;
}
