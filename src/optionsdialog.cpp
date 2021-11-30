/*! ***********************************************************************************************
 *
 * \file        optionsdialog.cpp
 * \brief       OptionsDialog 类源文件。
 *
 * \version     1.0
 * \date        2021-11-30
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#include "optionsdialog.h"
#include "ui_optionsdialog.h"


OptionsDialog::OptionsDialog(QWidget *parent /*= nullptr*/)
    : QDialog(parent)
    , ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

    QSettings settings;
    ui->editAria2c->setText(settings.value(QS("aria2c")).toString());

    connect(ui->buttonAria2c, &QPushButton::clicked, this, &OptionsDialog::browseAria2c);
}


OptionsDialog::~OptionsDialog()
{
    delete ui;
}


void OptionsDialog::accept()
{
    QSettings settings;
    settings.setValue(QS("aria2c"), ui->editAria2c->text());

    QDialog::accept();
}


void OptionsDialog::changeEvent(QEvent *e)
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


void OptionsDialog::browseAria2c()
{
    auto aria2c = QFileDialog::getOpenFileName(this, {}, ui->editAria2c->text(),
#ifdef Q_OS_WIN
                                               QS("Aria2c (aria2c.exe)")
#else
                                               QS("Aria2c (aria2c)")
#endif
    );

    if (!aria2c.isEmpty())
    {
        ui->editAria2c->setText(QDir::toNativeSeparators(aria2c));
    }
}
