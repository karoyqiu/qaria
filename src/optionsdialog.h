/*! ***********************************************************************************************
 *
 * \file        optionsdialog.h
 * \brief       OptionsDialog 类头文件。
 *
 * \version     1.0
 * \date        2021-11-30
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#pragma once
#include <QDialog>

namespace Ui {
class OptionsDialog;
}


class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = nullptr);
    virtual ~OptionsDialog();

    virtual void accept() override;

protected:
    virtual void changeEvent(QEvent *e) override;

private:
    void browseAria2c();
    void browseDir();
    void browseMdc();
    void randomizeSecret();
    void associate();

private:
    Ui::OptionsDialog *ui;
};
