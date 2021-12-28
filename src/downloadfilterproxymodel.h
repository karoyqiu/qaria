/*! ***********************************************************************************************
 *
 * \file        downloadfilterproxymodel.h
 * \brief       DownloadFilterProxyModel 类头文件。
 *
 * \version     1.0
 * \date        2021-12-28
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 Roy QIU。
 *
 **************************************************************************************************/
#pragma once
#include <QSortFilterProxyModel>

#include "aria2c.h"


class DownloadFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit DownloadFilterProxyModel(QObject *parent = nullptr);

    void setStatus(DownloadStatus value);

protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    DownloadStatus status_;
};
