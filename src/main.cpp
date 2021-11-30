/*! ***********************************************************************************************
 *
 * \file        main.cpp
 * \brief       主源文件。
 *
 * \version     1.0
 * \date        2021-09-17
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2021 karoyqiu。
 *
 **************************************************************************************************/
#include <QApplication>
#include <QLocale>
#include <QTranslator>

 //#include "maindialog.h"


int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

    QApplication a(argc, argv);
    QApplication::setApplicationName(QS("qaria"));
    QApplication::setOrganizationName(QS("karoyqiu"));
    QApplication::setOrganizationDomain(QS("karoyqiu.google.com"));

    QTranslator translator;
    QLocale locale;

    if (translator.load(locale, QS("qaria"), QS("_"), QS(":/i18n")))
    {
        a.installTranslator(&translator);
    }

    QApplication::setApplicationDisplayName(QApplication::translate("main", "qaria"));

    //MainDialog w;
    //w.show();

    return QApplication::exec();
}
