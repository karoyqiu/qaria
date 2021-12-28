/*! ***********************************************************************************************
 *
 * \file        filetreewidget.h
 * \brief       FileTreeWidget 类头文件。
 *
 * \version     1.0
 * \date        2021-12-26
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 Roy QIU。
 *
 **************************************************************************************************/
#pragma once
#include <QTreeWidget>

struct DownloadItem;


class FileTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    enum Column
    {
        NameColumn,
        SizeColumn,
        ProgressColumn,
        RemainingSizeColumn,
        ColumnCount,
    };

    explicit FileTreeWidget(QWidget *parent = nullptr);

    void setDownloadItem(const DownloadItem &download);
    void clear();

    qint64 totalSize() const { return totalSize_; }
    const QVector<QTreeWidgetItem *> &fileItems() const { return fileItems_; }
    const QMap<QString, QIcon> &extensions() { return exts_; }
    QString selectedFiles() const;

private:
    static QString headerLabel(Column col);
    static Qt::Alignment columnAlignment(Column col);

    QTreeWidgetItem *addSegmentTo(QTreeWidgetItem *parent, const QString &seg);
    void calcSize();
    qint64 calcSize(QTreeWidgetItem *parent);

private:
    QVector<QTreeWidgetItem *> fileItems_;
    QMap<QString, QIcon> exts_;
    qint64 totalSize_;
};
