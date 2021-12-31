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
#include "progressitemdelegate.h"


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

    auto *dlgt = new DataSizeDelegate(this);
    setItemDelegateForColumn(SizeColumn, dlgt);
    setItemDelegateForColumn(RemainingSizeColumn, dlgt);

    auto *pdlgt = new ProgressItemDelegate(this);
    setItemDelegateForColumn(ProgressColumn, pdlgt);

    header()->setSectionResizeMode(NameColumn, QHeaderView::Stretch);
    sortItems(1, Qt::DescendingOrder);


    connect(this, &QTreeWidget::itemDoubleClicked, this, &FileTreeWidget::openFile);
}


void FileTreeWidget::setDownloadItem(const DownloadItem &download)
{
    clear();
    fileItems_.fill(nullptr, download.files.count());

    const auto dir = QDir::fromNativeSeparators(download.dir);
    QFileIconProvider iconProvider;
    auto dirIcon = iconProvider.icon(QFileIconProvider::Folder);

    for (const auto &file : download.files)
    {
        if (file.path.isEmpty() || file.path.startsWith(QL("[METADATA]")))
        {
            continue;
        }

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
        item->setText(NameColumn, filename);
        item->setIcon(NameColumn, icon);
        item->setData(NameColumn, IndexRole, file.index);
        item->setData(NameColumn, PathRole, file.path);
        item->setCheckState(NameColumn, checkState(file.selected));
        item->setData(SizeColumn, Qt::DisplayRole, file.length);

        if (file.length > 0)
        {
            item->setData(ProgressColumn, Qt::DisplayRole, file.completedLength * 100 / file.length);
            item->setData(RemainingSizeColumn, Qt::DisplayRole, file.length - file.completedLength);
        }
        else
        {
            item->setData(ProgressColumn, Qt::DisplayRole, 100);
            item->setData(RemainingSizeColumn, Qt::DisplayRole, 0);
        }

        item->setTextAlignment(SizeColumn, Qt::AlignRight | Qt::AlignVCenter);
        item->setTextAlignment(ProgressColumn, Qt::AlignCenter);
        item->setTextAlignment(RemainingSizeColumn, Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        fileItems_[file.index - 1] = item;

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

    expandAll();
}


void FileTreeWidget::updateDownloadItem(const DownloadItem &download)
{
    if (!fileItems_.isEmpty())
    {
        for (const auto &file : download.files)
        {
            if (file.length > 0)
            {
                auto *item = fileItems_.at(file.index - 1);

                if (item != nullptr)
                {
                    item->setData(ProgressColumn, Qt::DisplayRole, file.completedLength * 100 / file.length);
                    item->setData(RemainingSizeColumn, Qt::DisplayRole, file.length - file.completedLength);
                }
            }
        }

        calcSize();
    }
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
    exts_.clear();
    fileItems_.clear();

    while (topLevelItemCount() > 0)
    {
        delete takeTopLevelItem(0);
    }
}


QString FileTreeWidget::selectedFiles() const
{
    QStringList selected;

    for (const auto *item : fileItems_)
    {
        if (item != nullptr && item->checkState(0) == Qt::Checked)
        {
            selected.append(item->data(0, IndexRole).toString());
        }
    }

    return selected.join(QL(','));
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
        totalSize_ += calcSize(p).first;
    }
}


QPair<qint64, qint64> FileTreeWidget::calcSize(QTreeWidgetItem *parent)
{
    qint64 total = 0;
    qint64 remaining = 0;

    for (int i = 0; i < parent->childCount(); i++)
    {
        auto *item = parent->child(i);

        if (item->childCount() == 0)
        {
            if (item->checkState(0) == Qt::Checked)
            {
                total += item->data(SizeColumn, Qt::DisplayRole).toLongLong();
                remaining += item->data(RemainingSizeColumn, Qt::DisplayRole).toLongLong();
            }
        }
        else
        {
            auto sizes = calcSize(item);
            total += sizes.first;
            remaining += sizes.second;
        }
    }

    parent->setData(SizeColumn, Qt::DisplayRole, total);
    parent->setTextAlignment(SizeColumn, Qt::AlignRight | Qt::AlignVCenter);
    parent->setTextAlignment(ProgressColumn, Qt::AlignCenter);
    parent->setTextAlignment(RemainingSizeColumn, Qt::AlignRight | Qt::AlignVCenter);

    if (total > 0)
    {
        parent->setData(ProgressColumn, Qt::DisplayRole, (total - remaining) * 100 / total);
        parent->setData(RemainingSizeColumn, Qt::DisplayRole, remaining);
    }
    else
    {
        parent->setData(ProgressColumn, Qt::DisplayRole, 100);
        parent->setData(RemainingSizeColumn, Qt::DisplayRole, 0);
    }

    return qMakePair(total, remaining);
}


void FileTreeWidget::openFile(QTreeWidgetItem *item)
{
    auto path = item->data(NameColumn, PathRole).toString();

    if (!path.isEmpty())
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }
}
