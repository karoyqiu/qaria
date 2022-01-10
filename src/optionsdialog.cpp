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
#include "aria2c.h"


OptionsDialog::OptionsDialog(QWidget *parent /*= nullptr*/)
    : QDialog(parent)
    , ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

    QSettings settings;
    ui->editAria2c->setText(settings.value(QS("aria2c")).toString());
    ui->editSecret->setText(settings.value(QS("secret")).toString());
    ui->editDir->setText(settings.value(QS("dir")).toString());

    connect(ui->buttonAria2c, &QPushButton::clicked, this, &OptionsDialog::browseAria2c);
    connect(ui->buttonDir, &QPushButton::clicked, this, &OptionsDialog::browseDir);
    connect(ui->buttonRandom, &QPushButton::clicked, this, &OptionsDialog::randomizeSecret);
    connect(ui->buttonAssociate, &QPushButton::clicked, this, &OptionsDialog::associate);
}


OptionsDialog::~OptionsDialog()
{
    delete ui;
}


void OptionsDialog::accept()
{
    QSettings settings;
    settings.setValue(QS("aria2c"), ui->editAria2c->text());
    settings.setValue(QS("secret"), ui->editSecret->text());
    settings.setValue(QS("dir"), ui->editDir->text());

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


void OptionsDialog::browseDir()
{
    auto dir = QFileDialog::getExistingDirectory(this, {}, ui->editDir->text());

    if (!dir.isEmpty())
    {
        ui->editDir->setText(QDir::toNativeSeparators(dir));
    }
}


void OptionsDialog::randomizeSecret()
{
    ui->editSecret->setText(Aria2c::generateToken());
}


void OptionsDialog::associate()
{
#ifdef Q_OS_WIN
    QSettings classes(QS(R"(HKEY_CURRENT_USER\SOFTWARE\Classes\qaria2.magnet)"), QSettings::NativeFormat);
    classes.setValue(QS("."), QS("URL:magnet"));
    classes.setValue(QS("Content Type"), QS("application/x-magnet"));
    classes.setValue(QS("URL Protocol"), QString());

    auto appPath = QDir::toNativeSeparators(qApp->applicationFilePath());

    classes.beginGroup(QS("DefaultIcon"));
    classes.setValue(QS("."), QString(QL('"') % appPath % QL("\",1")));
    classes.endGroup();

    classes.beginGroup(QS("shell"));
    classes.setValue(QS("."), QS("open"));
    classes.beginGroup(QS("open/command"));
    classes.setValue(QS("."), QString(QL('"') % appPath % QL(R"(" "%1")")));

    QSettings regApps(QS(R"(HKEY_CURRENT_USER\SOFTWARE\RegisteredApplications)"), QSettings::NativeFormat);
    regApps.setValue(QS("qaria2"), QS(R"(Software\%1\%2\Capabilities)").arg(qApp->organizationName(), qApp->applicationName()));

    QSettings settings;
    settings.beginGroup(QS("Capabilities"));
    settings.setValue(QS("ApplicationIcon"), QString(QL('"') % appPath % QL("\",0")));
    settings.setValue(QS("ApplicationName"), qApp->applicationDisplayName());
    settings.setValue(QS("ApplicationDescription"), tr("Overall downloader"));
    settings.beginGroup(QS("URLAssociations"));
    settings.setValue(QS("magnet"), QS("qaria2.magnet"));
#else
#error Unsupported platform
#endif
}
