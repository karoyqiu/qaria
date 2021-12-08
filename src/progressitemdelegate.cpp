/*! ***********************************************************************************************
 *
 * \file        progressitemdelegate.cpp
 * \brief       ProgressItemDelegate 类源文件。
 *
 * \version     1.0
 * \date        2021-11-30
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#include "progressitemdelegate.h"


void ProgressItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    auto value = index.data().toInt();

    QStyleOptionProgressBar opt;
    opt.rect = option.rect;
    opt.minimum = 0;
    opt.maximum = 100;
    opt.progress = value;
    opt.text = QS("%1%2").arg(value).arg(option.locale.percent());
    opt.textVisible = true;
    opt.textAlignment = Qt::AlignCenter;

    qApp->style()->drawControl(QStyle::CE_ProgressBar, &opt, painter);
}
