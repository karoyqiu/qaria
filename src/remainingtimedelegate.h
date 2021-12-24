/*! ***********************************************************************************************
 *
 * \file        remainingtimedelegate.h
 * \brief       RemainingTimeDelegate 类头文件。
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


class RemainingTimeDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;

    virtual QString displayText(const QVariant &value, const QLocale &locale) const override;
};
