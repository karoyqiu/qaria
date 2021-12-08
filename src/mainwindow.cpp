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

#include "aria2c.h"
#include "aria2optionsbuilder.h"
#include "datasizedelegate.h"
#include "downloadtablemodel.h"
#include "newbittorrentdialog.h"
#include "optionsdialog.h"
#include "statusdelegate.h"


MainWindow::MainWindow(QWidget *parent /*= nullptr*/)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , aria2c_(nullptr)
    , model_(nullptr)
{
    ui->setupUi(this);
    ui->treeFilter->expandAll();
    ui->centralWidget->setStretchFactor(0, 1);
    ui->centralWidget->setStretchFactor(1, 5);
    ui->actionResume->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(ui->actionAdd, &QAction::triggered, this, &MainWindow::addUri);
    connect(ui->actionRemove, &QAction::triggered, this, &MainWindow::remove);
    connect(ui->actionResume, &QAction::triggered, this, &MainWindow::resume);
    connect(ui->actionOptions, &QAction::triggered, this, &MainWindow::showOptions);

    model_ = new DownloadTableModel(this);
    ui->treeMain->setModel(model_);

    auto *sizeDlgt = new DataSizeDelegate(this);
    ui->treeMain->setItemDelegateForColumn(DownloadTableModel::SizeColumn, sizeDlgt);

    auto *speedDlgt = new DataSizeDelegate(QS("/s"), this);
    ui->treeMain->setItemDelegateForColumn(DownloadTableModel::DownloadSpeedColumn, speedDlgt);
    ui->treeMain->setItemDelegateForColumn(DownloadTableModel::UploadSpeedColumn, speedDlgt);

    auto *statusDlgt = new StatusDelegate(this);
    ui->treeMain->setItemDelegateForColumn(DownloadTableModel::StatusColumn, statusDlgt);


    loadSettings();

    aria2c_ = new Aria2c(this);
    connect(aria2c_, &Aria2c::aria2Started, this, &MainWindow::downloadTrackers);
    connect(aria2c_, &Aria2c::changed, model_, &DownloadTableModel::reset);
    connect(aria2c_, &Aria2c::removed, model_, &DownloadTableModel::remove);
    connect(model_, &QAbstractItemModel::rowsInserted, this, &MainWindow::handleAdded);

    aria2c_->start();
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


void MainWindow::addUri()
{
    bool ok = false;
    auto uri = QInputDialog::getText(this, {}, tr("Input the URL to download"), QLineEdit::Normal,
                                     {}, &ok);

    if (ok && !uri.isEmpty())
    {
        aria2c_->addUri(uri);
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
                QStringList selected;

                for (const auto &index : dialog.selectedFiles())
                {
                    selected.append(QSS(index));
                }

                OptionsBuilder builder;
                builder.setSelectFile(selected.join(QL(',')));
                aria2c_->changeOption(item.gid, builder.options());
                aria2c_->resume(item.gid);
            }
            else
            {
            }
        }
    }
}
