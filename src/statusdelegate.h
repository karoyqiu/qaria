/*! ***********************************************************************************************
 *
 * \file        statusdelegate.h
 * \brief       StatusDelegate 类头文件。
 *
 * \version     1.0
 * \date        2021-12-06
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#pragma once
#include <QStyledItemDelegate>


class StatusDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit StatusDelegate(QObject *parent = nullptr);

    virtual QString displayText(const QVariant &value, const QLocale &locale) const override;
};
