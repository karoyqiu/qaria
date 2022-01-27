/*! ***********************************************************************************************
 *
 * \file        scraperdialog.cpp
 * \brief       ScraperDialog 类源文件。
 *
 * \version     1.0
 * \date        2022-01-27
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2022 Roy QIU。
 *
 **************************************************************************************************/
#include "scraperdialog.h"
#include "ui_scraperdialog.h"


ScraperDialog::ScraperDialog(const QString &root, const QString &name, QWidget *parent /*= nullptr*/)
    : QDialog(parent)
    , ui(new Ui::ScraperDialog)
{
    ui->setupUi(this);

    QSettings settings;
    auto mdc = settings.value(QS("mdc")).toString();

    if (mdc.isEmpty())
    {
        QMessageBox::critical(this, {}, tr("No Movie Data Capture."));
        QTimer::singleShot(0, this, &QDialog::reject);
        return;
    }


    auto mono = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    ui->editOutput->setFont(mono);
    ui->editOutput->appendPlainText(tr("Scraping %1...\n").arg(name));

    auto *proc = new QProcess(this);
    proc->setProcessChannelMode(QProcess::MergedChannels);
    proc->setWorkingDirectory(root);
    connect(proc, &QProcess::readyReadStandardOutput, this, &ScraperDialog::readOutput);
    connect(proc, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &ScraperDialog::finish);

    QString dir = root % QDir::separator() % name;
    proc->start(mdc, { QS("-p"), dir, QS("--log-dir="), QS("-a") }, QIODevice::ReadOnly | QIODevice::Unbuffered);
}


ScraperDialog::~ScraperDialog()
{
    delete ui;
}


void ScraperDialog::changeEvent(QEvent *e)
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


void ScraperDialog::readOutput()
{
    auto *proc = qobject_cast<QProcess *>(sender());
    auto txt = proc->readAllStandardOutput();
    ui->editOutput->appendPlainText(QString::fromLocal8Bit(txt));
}


void ScraperDialog::finish(int code, QProcess::ExitStatus status)
{
    ui->editOutput->appendPlainText(tr("\nScraping finished with code %1 (%2)").arg(code).arg(status));
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
}
