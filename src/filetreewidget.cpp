/*! ***********************************************************************************************
 *
 * \file        filetreewidget.cpp
 * \brief       FileTreeWidget 类源文件。
 *
 * \version     1.0
 * \date        2021-12-26
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 Roy QIU。
 *
 **************************************************************************************************/
#include "filetreewidget.h"

#include "aria2c.h"
#include "datasizedelegate.h"


static inline Qt::CheckState checkState(bool on)
{
    return on ? Qt::Checked : Qt::Unchecked;
}


FileTreeWidget::FileTreeWidget(QWidget *parent /*= nullptr*/)
    : QTreeWidget(parent)
{
    setColumnCount(ColumnCount);
    auto *item = new QTreeWidgetItem;

    for (int i = 0; i < ColumnCount; i++)
    {
        auto col = static_cast<Column>(i);
        item->setText(i, headerLabel(col));
        item->setTextAlignment(i, columnAlignment(col));
    }

    setHeaderItem(item);


    header()->setSectionResizeMode(0, QHeaderView::Stretch);
    sortItems(1, Qt::DescendingOrder);
}


void FileTreeWidget::setDownloadItem(const DownloadItem &download)
{
    const auto dir = QDir::fromNativeSeparators(download.dir);
    QFileIconProvider iconProvider;
    auto dirIcon = iconProvider.icon(QFileIconProvider::Folder);

    exts_.clear();
    fileItems_.clear();

    for (const auto &file : download.files)
    {
        auto path = QDir::fromNativeSeparators(file.path);
        Q_ASSERT(path.startsWith(dir));
        path.remove(0, dir.length() + 1);

        auto segments = path.split(QL('/'), Qt::SkipEmptyParts);
        auto filename = segments.takeLast();

        if (filename.startsWith(QL("_____padding_file_")))
        {
            continue;
        }

        QFileInfo info(file.path);
        auto icon = iconProvider.icon(info);

        if (!exts_.contains(info.suffix()))
        {
            exts_.insert(info.suffix(), icon);
        }

        QTreeWidgetItem *p = nullptr;

        for (const auto &seg : qAsConst(segments))
        {
            p = addSegmentTo(p, seg);
            p->setIcon(0, dirIcon);
        }

        auto *item = new QTreeWidgetItem();
        item->setText(0, filename);
        item->setIcon(0, icon);
        item->setData(0, Qt::UserRole, file.index);
        item->setCheckState(0, checkState(file.selected));
        item->setData(1, Qt::DisplayRole, file.length);
        item->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        fileItems_.append(item);

        if (p == nullptr)
        {
            addTopLevelItem(item);
        }
        else
        {
            p->addChild(item);
        }
    }

    calcSize();

    auto *dlgt = new DataSizeDelegate(this);
    setItemDelegateForColumn(1, dlgt);

    expandAll();
}


QString FileTreeWidget::headerLabel(Column col)
{
    switch (col)
    {
    case NameColumn:
        return tr("Name");
    case SizeColumn:
        return tr("Size");
    case ProgressColumn:
        return tr("Progress");
    case RemainingSizeColumn:
        return tr("Remaining Size");
    case ColumnCount:
    default:
        Q_UNREACHABLE();
    }
}


Qt::Alignment FileTreeWidget::columnAlignment(Column col)
{
    switch (col)
    {
    case SizeColumn:
    case RemainingSizeColumn:
        return Qt::AlignRight | Qt::AlignVCenter;
    case ProgressColumn:
        return Qt::AlignCenter;
    default:
        return {};
    }
}


void FileTreeWidget::clear()
{
    while (topLevelItemCount() > 0)
    {
        delete takeTopLevelItem(0);
    }
}


QTreeWidgetItem *FileTreeWidget::addSegmentTo(QTreeWidgetItem *parent, const QString &seg)
{
    QTreeWidgetItem *item = nullptr;

    if (parent == nullptr)
    {
        for (int i = 0; i < topLevelItemCount(); i++)
        {
            auto *p = topLevelItem(i);

            if (p->text(0) == seg)
            {
                item = p;
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < parent->childCount(); i++)
        {
            auto *p = parent->child(i);

            if (p->text(0) == seg)
            {
                item = p;
                break;
            }
        }
    }

    if (item == nullptr)
    {
        item = new QTreeWidgetItem;
        item->setText(0, seg);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);

        if (parent == nullptr)
        {
            addTopLevelItem(item);
        }
        else
        {
            parent->addChild(item);
        }
    }

    return item;
}


void FileTreeWidget::calcSize()
{
    totalSize_ = 0;

    for (int i = 0; i < topLevelItemCount(); i++)
    {
        auto *p = topLevelItem(i);
        totalSize_ += calcSize(p);
    }
}


qint64 FileTreeWidget::calcSize(QTreeWidgetItem *parent)
{
    qint64 total = 0;

    for (int i = 0; i < parent->childCount(); i++)
    {
        auto *item = parent->child(i);

        if (item->childCount() == 0)
        {
            total += item->data(1, Qt::DisplayRole).toLongLong();
        }
        else
        {
            total += calcSize(item);
        }
    }

    parent->setData(1, Qt::DisplayRole, total);
    parent->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);

    return total;
}
