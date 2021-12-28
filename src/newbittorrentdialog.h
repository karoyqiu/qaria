/*! ***********************************************************************************************
 *
 * \file        newbittorrentdialog.h
 * \brief       NewBitTorrentDialog 类头文件。
 *
 * \version     1.0
 * \date        2021-12-08
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#pragma once
#include <QDialog>

#include "aria2c.h"

namespace Ui {
class NewBitTorrentDialog;
}

class OptionsBuilder;


class NewBitTorrentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewBitTorrentDialog(const DownloadItem &download, QWidget *parent = nullptr);
    virtual ~NewBitTorrentDialog();

    QString selectedFiles() const;

protected:
    virtual void changeEvent(QEvent *e) override;

private:
    QTreeWidgetItem *addSegmentTo(QTreeWidgetItem *parent, const QString &seg);

    void calcSize();
    qint64 calcSize(QTreeWidgetItem *parent);

    void handleAllExtChecked(bool on);
    void handleExtChanged(QListWidgetItem *item);

private:
    Ui::NewBitTorrentDialog *ui;
};
