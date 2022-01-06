/*! ***********************************************************************************************
 *
 * \file        newdownloaddialog.h
 * \brief       NewDownloadDialog 类头文件。
 *
 * \version     1.0
 * \date        2021-12-16
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#pragma once
#include <QDialog>

namespace Ui {
class NewDownloadDialog;
}


class NewDownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDownloadDialog(QWidget *parent = nullptr);
    virtual ~NewDownloadDialog();

    void setUris(const QStringList &value);
    QList<QUrl> uris() const;
    QVariantHash options() const;

protected:
    virtual void changeEvent(QEvent *e) override;

private:
    void magnetToTorrent();

private:
    Ui::NewDownloadDialog *ui;
};
