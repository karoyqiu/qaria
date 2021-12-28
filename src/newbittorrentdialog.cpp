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

#include "aria2optionsbuilder.h"
#include "datasizedelegate.h"


static inline Qt::CheckState checkState(bool on)
{
    return on ? Qt::Checked : Qt::Unchecked;
}


NewBitTorrentDialog::NewBitTorrentDialog(const DownloadItem &download, QWidget *parent /*= nullptr*/)
    : QDialog(parent)
    , ui(new Ui::NewBitTorrentDialog)
{
    ui->setupUi(this);
    ui->treeFiles->setDownloadItem(download);

    const auto &exts = ui->treeFiles->extensions();

    for (const auto &ext : exts.keys())
    {
        auto *item = new QListWidgetItem(exts.value(ext), ext, ui->listExt);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
    }


    ui->editDir->setText(QDir::toNativeSeparators(download.dir));


    connect(ui->checkAllExt, &QCheckBox::toggled, this, &NewBitTorrentDialog::handleAllExtChecked);
    connect(ui->listExt, &QListWidget::itemChanged, this, &NewBitTorrentDialog::handleExtChanged);
}


NewBitTorrentDialog::~NewBitTorrentDialog()
{
    delete ui;
}


void NewBitTorrentDialog::buildOptions(OptionsBuilder &builder) const
{
    QString selected;

    for (const auto *item : fileItems_)
    {
        if (item->checkState(0) == Qt::Checked)
        {
            selected.append(item->data(0, Qt::UserRole).toString());
            selected.append(QL(','));
        }
    }

    builder.setSelectFile(selected);
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


void NewBitTorrentDialog::handleAllExtChecked(bool on)
{
    for (int i = 0; i < ui->listExt->count(); i++)
    {
        auto *item = ui->listExt->item(i);
        item->setCheckState(checkState(on));
    }
}


void NewBitTorrentDialog::handleExtChanged(QListWidgetItem *item)
{
    auto ext = item->text();
    auto check = item->checkState();

    for (auto *f : fileItems_)
    {
        if (f->text(0).endsWith(ext))
        {
            f->setCheckState(0, check);
        }
    }
}
