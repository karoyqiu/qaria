/*! ***********************************************************************************************
 *
 * \file        mainwindow.cpp
 * \brief       MainWindow 类源文件。
 *
 * \version     1.0
 * \date        2021-11-30
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <rep_qaria2_source.h>

#include "aria2c.h"
#include "aria2optionsbuilder.h"
#include "datasizedelegate.h"
#include "downloadfilterproxymodel.h"
#include "downloadtablemodel.h"
#include "newdownloaddialog.h"
#include "newbittorrentdialog.h"
#include "optionsdialog.h"
#include "progressitemdelegate.h"
#include "remainingtimedelegate.h"
#include "statusdelegate.h"


MainWindow::MainWindow(QWidget *parent /*= nullptr*/)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , aria2c_(nullptr)
    , model_(nullptr)
    , proxy_(nullptr)
    , downLabel_(nullptr)
    , upLabel_(nullptr)
{
    ui->setupUi(this);
    ui->treeFilter->expandAll();
    ui->centralWidget->setStretchFactor(0, 1);
    ui->centralWidget->setStretchFactor(1, 5);
    ui->actionResume->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    downLabel_ = new QLabel(this);
    ui->statusBar->addPermanentWidget(downLabel_);

    upLabel_ = new QLabel(this);
    ui->statusBar->addPermanentWidget(upLabel_);

    connect(ui->actionAdd, &QAction::triggered, this, &MainWindow::addUri);
    connect(ui->actionRemove, &QAction::triggered, this, &MainWindow::remove);
    connect(ui->actionPause, &QAction::triggered, this, &MainWindow::pause);
    connect(ui->actionResume, &QAction::triggered, this, &MainWindow::resume);
    connect(ui->actionOptions, &QAction::triggered, this, &MainWindow::showOptions);

    model_ = new DownloadTableModel(this);
    proxy_ = new DownloadFilterProxyModel(this);
    proxy_->setSourceModel(model_);
    ui->treeMain->setModel(proxy_);

    auto *sizeDlgt = new DataSizeDelegate(this);
    ui->treeMain->setItemDelegateForColumn(DownloadTableModel::SizeColumn, sizeDlgt);

    auto *prgDlgt = new ProgressItemDelegate(this);
    ui->treeMain->setItemDelegateForColumn(DownloadTableModel::ProgressColumn, prgDlgt);

    auto *speedDlgt = new DataSizeDelegate(QS("/s"), this);
    ui->treeMain->setItemDelegateForColumn(DownloadTableModel::DownloadSpeedColumn, speedDlgt);
    ui->treeMain->setItemDelegateForColumn(DownloadTableModel::UploadSpeedColumn, speedDlgt);

    auto *statusDlgt = new StatusDelegate(this);
    ui->treeMain->setItemDelegateForColumn(DownloadTableModel::StatusColumn, statusDlgt);

    auto *rtDlgt = new RemainingTimeDelegate(this);
    ui->treeMain->setItemDelegateForColumn(DownloadTableModel::RemainingTimeColumn, rtDlgt);


    loadSettings();

    aria2c_ = new Aria2c(this);
    connect(aria2c_, &Aria2c::aria2Started, this, &MainWindow::downloadTrackers);
    connect(aria2c_, &Aria2c::globalStat, this, &MainWindow::updateStat);
    connect(aria2c_, &Aria2c::removed, this, &MainWindow::handleRemoved);
    connect(aria2c_, &Aria2c::changed, model_, &DownloadTableModel::reset);
    connect(model_, &QAbstractItemModel::rowsInserted, this, &MainWindow::handleAdded);
    connect(model_, &QAbstractItemModel::dataChanged, this, &MainWindow::updateFiles);
    connect(ui->treeMain, &QTreeView::doubleClicked, this, &MainWindow::edit);
    connect(ui->treeMain->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::showFiles);

    aria2c_->start();


    auto *tray = new QSystemTrayIcon(QIcon(QS(":/icons/qa2.png")), this);
    tray->setToolTip(qApp->applicationDisplayName());
    tray->show();
    connect(tray, &QSystemTrayIcon::activated, this, &MainWindow::showMe);


    auto *host = new QRemoteObjectHost(QS("local:qaria2"), this);

    if (!host->enableRemoting<QAria2SourceAPI>(this))
    {
        qWarning() << "Failed to enable remoting.";
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);

    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    case QEvent::WindowStateChange:
        if (isMinimized())
        {
            hide();
        }
        break;
    default:
        break;
    }
}


void MainWindow::closeEvent(QCloseEvent *e)
{
    saveSettings();
    QMainWindow::closeEvent(e);
}


void MainWindow::loadSettings()
{
    QSettings settings;
    restoreGeometry(settings.value(QS("geo")).toByteArray());
    restoreState(settings.value(QS("sta")).toByteArray());
    ui->treeMain->header()->restoreState(settings.value(QS("hdr")).toByteArray());
    ui->centralWidget->restoreState(settings.value(QS("spl")).toByteArray());
}


void MainWindow::saveSettings() const
{
    QSettings settings;
    settings.setValue(QS("geo"), saveGeometry());
    settings.setValue(QS("sta"), saveState());
    settings.setValue(QS("hdr"), ui->treeMain->header()->saveState());
    settings.setValue(QS("spl"), ui->centralWidget->saveState());
}


void MainWindow::showMe()
{
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    show();
}


void MainWindow::updateFilter()
{

}


void MainWindow::addUri()
{
    addUris({});
}


void MainWindow::addUris(const QStringList &uris)
{
    NewDownloadDialog dialog(this);
    dialog.setUris(uris);

    if (dialog.exec() == QDialog::Accepted)
    {
        const auto us = dialog.uris();
        auto opts = dialog.options();

        for (const auto &uri : us)
        {
            aria2c_->addUri(uri, opts);
        }
    }
}


void MainWindow::remove()
{
    auto idx = ui->treeMain->currentIndex();

    if (idx.isValid())
    {
        auto answer = QMessageBox::question(this, {}, tr("Do you really want to remove current download?"));

        if (answer == QMessageBox::Yes)
        {
            auto gid = idx.data(DownloadTableModel::GidRole).toString();
            aria2c_->remove(gid);
        }
    }
}


void MainWindow::pause()
{
    QStringList gids;
    const auto rows = ui->treeMain->selectionModel()->selectedRows();

    for (const auto &idx : rows)
    {
        gids.append(idx.data(DownloadTableModel::GidRole).toString());
    }

    aria2c_->pause(gids);
}


void MainWindow::resume()
{
    QStringList gids;
    const auto rows = ui->treeMain->selectionModel()->selectedRows();

    for (const auto &idx : rows)
    {
        gids.append(idx.data(DownloadTableModel::GidRole).toString());
    }

    aria2c_->resume(gids);
}


void MainWindow::showOptions()
{
    OptionsDialog dialog(this);
    dialog.exec();
}


void MainWindow::downloadTrackers()
{
    auto *net = new QNetworkAccessManager(this);
    QNetworkRequest req(QS("https://ngosang.github.io/trackerslist/trackers_all.txt"));
    auto *reply = net->get(req);
    connect(reply, &QNetworkReply::finished, this, &MainWindow::handleTrackers);
}


void MainWindow::handleTrackers()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    auto body = reply->readAll();
    reply->deleteLater();
    reply->manager()->deleteLater();

    auto s = QSS(body);
    auto list = s.split(QL('\n'), Qt::SkipEmptyParts);
    aria2c_->setBtTrackers(list);
}


void MainWindow::handleRemoved(const QString &gid)
{
    auto n = model_->rowCount();

    for (int i = 0; i < n; i++)
    {
        auto idx = model_->index(i, 0);
        const auto *item = static_cast<DownloadItem *>(idx.data(DownloadTableModel::ItemRole).value<void *>());

        if (item->gid == gid)
        {
            auto file = item->files.first().path;
            file.remove(0, item->dir.length() + 1);

            auto name = file.section(QL('/'), 0, 0);

            if (!name.isEmpty())
            {
                QDir dir(item->dir);
                QFile::remove(dir.absoluteFilePath(name));
                QFile::remove(dir.absoluteFilePath(name % QL(".aria2")));
                QFile::remove(dir.absoluteFilePath(item->infoHash % QL(".torrent")));

                if (dir.cd(name))
                {
                    dir.removeRecursively();
                }
            }

            break;
        }
    }
}


void MainWindow::handleAdded(const QModelIndex &parent, int first, int last)
{
    for (int i = first; i <= last; i++)
    {
        auto idx = model_->index(i, 0, parent);
        const auto &item = model_->item(idx);

        if (item.status == DownloadStatus::Paused
            && !item.infoHash.isEmpty() && !item.files.isEmpty()
            && !item.files.first().path.startsWith(QL("[METADATA]")))
        {
            NewBitTorrentDialog dialog(item, this);

            if (dialog.exec() == QDialog::Accepted)
            {
                OptionsBuilder builder;
                builder.setSelectFile(dialog.selectedFiles());
                aria2c_->changeOption(item.gid, builder.options());
                aria2c_->resume(item.gid);
            }
            else
            {
                aria2c_->remove(item.gid);
            }
        }
    }
}


void MainWindow::edit(const QModelIndex &idx)
{
    if (!idx.isValid())
    {
        return;
    }

    const auto *item = static_cast<DownloadItem *>(idx.data(DownloadTableModel::ItemRole).value<void *>());
    NewBitTorrentDialog dialog(*item, this);

    if (dialog.exec() == QDialog::Accepted)
    {
        OptionsBuilder builder;
        builder.setSelectFile(dialog.selectedFiles());
        aria2c_->changeOption(item->gid, builder.options());
    }
}


void MainWindow::showFiles(const QModelIndex &idx)
{
    if (idx.isValid())
    {
        const auto *item = static_cast<DownloadItem *>(idx.data(DownloadTableModel::ItemRole).value<void *>());
        ui->treeFiles->setDownloadItem(*item);
    }
    else
    {
        ui->treeFiles->clear();
    }
}


void MainWindow::updateFiles()
{
    auto idx = ui->treeMain->currentIndex();

    if (idx.isValid())
    {
        const auto *item = static_cast<DownloadItem *>(idx.data(DownloadTableModel::ItemRole).value<void *>());
        ui->treeFiles->updateDownloadItem(*item);
    }
}


void MainWindow::updateStat(const GlobalStat &stat)
{
    QLocale loc;
    downLabel_->setText(tr("Download speed: %1/s").arg(loc.formattedDataSize(stat.downloadSpeed)));
    upLabel_->setText(tr("Upload speed: %1/s").arg(loc.formattedDataSize(stat.uploadSpeed)));
}
