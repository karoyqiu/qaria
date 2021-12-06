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

    connect(ui->actionAdd, &QAction::triggered, this, &MainWindow::addUri);
    connect(ui->actionRemove, &QAction::triggered, this, &MainWindow::remove);
    connect(ui->actionOptions, &QAction::triggered, this, &MainWindow::showOptions);

    model_ = new DownloadTableModel(this);
    ui->tableMain->setModel(model_);

    auto *sizeDlgt = new DataSizeDelegate(this);
    ui->tableMain->setItemDelegateForColumn(DownloadTableModel::SizeColumn, sizeDlgt);
    ui->tableMain->setItemDelegateForColumn(DownloadTableModel::ProgressColumn, sizeDlgt);

    auto *speedDlgt = new DataSizeDelegate(QS("/s"), this);
    ui->tableMain->setItemDelegateForColumn(DownloadTableModel::DownloadSpeedColumn, speedDlgt);
    ui->tableMain->setItemDelegateForColumn(DownloadTableModel::UploadSpeedColumn, speedDlgt);

    auto *statusDlgt = new StatusDelegate(this);
    ui->tableMain->setItemDelegateForColumn(DownloadTableModel::StatusColumn, statusDlgt);


    loadSettings();

    aria2c_ = new Aria2c(this);
    connect(aria2c_, &Aria2c::aria2Started, this, &MainWindow::downloadTrackers);
    connect(aria2c_, &Aria2c::changed, model_, &DownloadTableModel::upsert);
    connect(aria2c_, &Aria2c::removed, model_, &DownloadTableModel::remove);

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
    ui->tableMain->horizontalHeader()->restoreState(settings.value(QS("hdr")).toByteArray());
    ui->centralWidget->restoreState(settings.value(QS("spl")).toByteArray());
}


void MainWindow::saveSettings() const
{
    QSettings settings;
    settings.setValue(QS("geo"), saveGeometry());
    settings.setValue(QS("sta"), saveState());
    settings.setValue(QS("hdr"), ui->tableMain->horizontalHeader()->saveState());
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
    auto idx = ui->tableMain->currentIndex();

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
