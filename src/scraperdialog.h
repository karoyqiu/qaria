/*! ***********************************************************************************************
 *
 * \file        scraperdialog.h
 * \brief       ScraperDialog 类头文件。
 *
 * \version     1.0
 * \date        2022-01-27
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2022 Roy QIU。
 *
 **************************************************************************************************/
#pragma once
#include <QDialog>

namespace Ui {
class ScraperDialog;
}


class ScraperDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScraperDialog(const QString &root, const QString &name, QWidget *parent = nullptr);
    virtual ~ScraperDialog();

protected:
    virtual void changeEvent(QEvent *e) override;

private:
    void readOutput();
    void finish(int code, QProcess::ExitStatus status);

private:
    Ui::ScraperDialog *ui;
};
