/*! ***********************************************************************************************
 *
 * \file        mainwindow.h
 * \brief       MainWindow 类头文件。
 *
 * \version     1.0
 * \date        2021-11-30
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#pragma once
#include <QWidget>

namespace Ui {
class MainWindow;
}

class Aria2c;
class DownloadTableModel;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

protected:
    virtual void changeEvent(QEvent *e) override;
    virtual void closeEvent(QCloseEvent *e) override;

private:
    void loadSettings();
    void saveSettings() const;

    void addUri();
    void remove();
    void resume();
    void showOptions();

    void downloadTrackers();
    void handleTrackers();

    void handleAdded(const QModelIndex &parent, int first, int last);
    void edit(const QModelIndex &idx);
    void showFiles(const QModelIndex &idx);

private:
    Ui::MainWindow *ui;
    Aria2c *aria2c_;
    DownloadTableModel *model_;
};
