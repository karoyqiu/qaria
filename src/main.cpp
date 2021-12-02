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

#include "mainwindow.h"


static inline int calc(int a, int b, bool add)
{
    return add ? a + b : a - b;
}


static void makeQuestions()
{
    int count = 0;
    auto *r = QRandomGenerator::global();

    while (count < 96)
    {
        int a = r->bounded(10);
        int b = r->bounded(10);
        bool op1 = (r->generate() & 1) == 0;
        int r1 = calc(a, b, op1);

        if (r1 < 0 || r1 > 10)
        {
            continue;
        }

        int c = r->bounded(10);
        bool op2 = (r->generate() & 1) == 0;
        int r2 = calc(r1, c, op2);

        if (r2 >= 0 && r2 <= 10)
        {
            qDebug() << a << (op1 ? "+" : "-")
                << b << (op2 ? "+" : "-")
                << c << "=";
            count++;
        }
    }
}


int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

    QApplication a(argc, argv);
    QApplication::setApplicationName(QS("qaria2"));
    QApplication::setOrganizationName(QS("karoyqiu"));
    QApplication::setOrganizationDomain(QS("karoyqiu.google.com"));

    QTranslator translator;
    QLocale locale;

    if (translator.load(locale, QS("qaria2"), QS("_"), QS(":/i18n")))
    {
        a.installTranslator(&translator);
    }

    QApplication::setApplicationDisplayName(QApplication::translate("main", "qaria2"));

    makeQuestions();

    MainWindow w;
    w.show();

    return QApplication::exec();
}
