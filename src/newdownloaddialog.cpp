﻿/*! ***********************************************************************************************
 *
 * \file        newdownloaddialog.cpp
 * \brief       NewDownloadDialog 类源文件。
 *
 * \version     1.0
 * \date        2021-12-16
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#include "newdownloaddialog.h"
#include "ui_newdownloaddialog.h"
#include "aria2optionsbuilder.h"


NewDownloadDialog::NewDownloadDialog(QWidget *parent /*= nullptr*/)
    : QDialog(parent)
    , ui(new Ui::NewDownloadDialog)
{
    ui->setupUi(this);

    QSettings settings;
    ui->editDir->setText(settings.value(QS("dir")).toString());

    auto s = qApp->clipboard()->text();
    QUrl url(s);

    if (url.isValid() && !url.isRelative())
    {
        auto sch = url.scheme();

        if (sch == QL("http") || sch == QL("https") || sch == QL("ftp") || sch == QL("magnet"))
        {
            ui->editUrls->setPlainText(url.toString());
        }
    }
    else
    {
        static const QRegularExpression btih(QS("[a-z0-9]{40}"), QRegularExpression::CaseInsensitiveOption);
        auto match = btih.match(s);

        if (match.hasMatch())
        {
            ui->editUrls->setPlainText(QS("magnet:?xt=urn:btih:%1").arg(s));
        }
    }
}


NewDownloadDialog::~NewDownloadDialog()
{
    delete ui;
}


QList<QUrl> NewDownloadDialog::uris() const
{
    auto ss = ui->editUrls->toPlainText().split(QL('\n'), Qt::SkipEmptyParts);
    return QUrl::fromStringList(ss);
}


QVariantHash NewDownloadDialog::options() const
{
    OptionsBuilder builder;

    QDir dir(ui->editDir->text());
    builder.setDir(dir.absoluteFilePath(ui->comboCategory->currentText()));
    builder.setAllowOverwrite(true);
    builder.setCheckIntegrity(true);

    return builder.options();
}


void NewDownloadDialog::changeEvent(QEvent *e)
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
