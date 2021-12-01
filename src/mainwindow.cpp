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
#include "downloadtablemodel.h"
#include "optionsdialog.h"


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
    connect(ui->actionOptions, &QAction::triggered, this, &MainWindow::showOptions);

    model_ = new DownloadTableModel(this);
    ui->tableMain->setModel(model_);

    aria2c_ = new Aria2c(this);
    connect(aria2c_, &Aria2c::downloadTold, model_, &DownloadTableModel::append);

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


void MainWindow::addUri()
{
    bool ok = false;
    auto uri = QInputDialog::getMultiLineText(this, {}, tr(""), {}, &ok);

    if (ok && !uri.isEmpty())
    {
        aria2c_->addUri(uri.split(QL('\n'), Qt::SkipEmptyParts));
    }
}


void MainWindow::showOptions()
{
    OptionsDialog dialog(this);
    dialog.exec();
}
