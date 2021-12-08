/*! ***********************************************************************************************
 *
 * \file        newbittorrentdialog.cpp
 * \brief       NewBitTorrentDialog 类源文件。
 *
 * \version     1.0
 * \date        2021-12-08
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#include "newbittorrentdialog.h"
#include "ui_newbittorrentdialog.h"

#include "datasizedelegate.h"


NewBitTorrentDialog::NewBitTorrentDialog(const DownloadItem &download, QWidget *parent /*= nullptr*/)
    : QDialog(parent)
    , ui(new Ui::NewBitTorrentDialog)
{
    ui->setupUi(this);
    ui->treeFiles->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeFiles->sortItems(1, Qt::DescendingOrder);

    const auto dir = QDir::fromNativeSeparators(download.dir);
    QFileIconProvider iconProvider;
    auto dirIcon = iconProvider.icon(QFileIconProvider::Folder);

    for (const auto &file : download.files)
    {
        auto path = QDir::fromNativeSeparators(file.path);
        Q_ASSERT(path.startsWith(dir));
        path.remove(0, dir.length() + 1);

        auto segments = path.split(QL('/'), Qt::SkipEmptyParts);
        auto filename = segments.takeLast();
        QTreeWidgetItem *p = nullptr;

        for (const auto &seg : qAsConst(segments))
        {
            p = addSegmentTo(p, seg);
            p->setIcon(0, dirIcon);
        }

        QTreeWidgetItem *item = new QTreeWidgetItem();
        QFileInfo info(file.path);
        item->setText(0, filename);
        item->setIcon(0, iconProvider.icon(info));
        item->setData(0, Qt::UserRole, file.index);
        item->setCheckState(0, file.selected ? Qt::Checked : Qt::Unchecked);
        item->setData(1, Qt::DisplayRole, file.length);
        item->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        fileItems_.append(item);

        if (p == nullptr)
        {
            ui->treeFiles->addTopLevelItem(item);
        }
        else
        {
            p->addChild(item);
        }
    }

    calcSize();

    auto *dlgt = new DataSizeDelegate(this);
    ui->treeFiles->setItemDelegateForColumn(1, dlgt);

    ui->treeFiles->expandAll();
}


NewBitTorrentDialog::~NewBitTorrentDialog()
{
    delete ui;
}


QVector<int> NewBitTorrentDialog::selectedFiles() const
{
    QVector<int> selected;
    return selected;
}


void NewBitTorrentDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);

    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


QTreeWidgetItem *NewBitTorrentDialog::addSegmentTo(QTreeWidgetItem *parent, const QString &seg)
{
    QTreeWidgetItem *item = nullptr;

    if (parent == nullptr)
    {
        for (int i = 0; i < ui->treeFiles->topLevelItemCount(); i++)
        {
            auto *p = ui->treeFiles->topLevelItem(i);

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
            ui->treeFiles->addTopLevelItem(item);
        }
        else
        {
            parent->addChild(item);
        }
    }

    return item;
}


void NewBitTorrentDialog::calcSize()
{
    for (int i = 0; i < ui->treeFiles->topLevelItemCount(); i++)
    {
        auto *p = ui->treeFiles->topLevelItem(i);
        calcSize(p);
    }
}


qint64 NewBitTorrentDialog::calcSize(QTreeWidgetItem *parent)
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
