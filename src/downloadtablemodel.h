/*! ***********************************************************************************************
 *
 * \file        downloadtablemodel.h
 * \brief       DownloadTableModel 类头文件。
 *
 * \version     1.0
 * \date        2021-11-30
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#pragma once
#include <QAbstractTableModel>

#include "aria2c.h"


class DownloadTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        SerialColumn,
        NameColumn,
        SizeColumn,
        ProgressColumn,
        StatusColumn,
        DownloadSpeedColumn,
        UploadSpeedColumn,
        RemainingTimeColumn,
        ColumnCount,
        CreationTimeColumn,
        FinishTimeColumn,
    };

    enum Role
    {
        GidRole = Qt::UserRole,
        ItemRole,
    };

    explicit DownloadTableModel(QObject *parent = nullptr);

    // Header:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    virtual int rowCount(const QModelIndex &parent = {}) const override
    {
        return parent.isValid() ? 0 : items_.count();
    }

    virtual int columnCount(const QModelIndex &parent = {}) const override
    {
        return parent.isValid() ? 0 : ColumnCount;
    }

    virtual QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;

    void reset(const DownloadItems &items);
    void remove(const QString &gid);

    const DownloadItem &item(const QModelIndex &idx) const;
    const DownloadItem &item(int row) const { return items_.at(row); }

private:
    static int columnAlignment(int col);

    void tickProgress();

private:
    DownloadItems items_;
};
