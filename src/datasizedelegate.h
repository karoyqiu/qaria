/*! ***********************************************************************************************
 *
 * \file        datasizedelegate.h
 * \brief       DataSizeDelegate 类头文件。
 *
 * \version     1.0
 * \date        2021-12-01
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#pragma once
#include <QStyledItemDelegate>


class DataSizeDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit DataSizeDelegate(QObject *parent = nullptr);
    explicit DataSizeDelegate(const QString &suffix, QObject *parent = nullptr);

    virtual QString displayText(const QVariant &value, const QLocale &locale) const override;

private:
    QString suffix_;
};
