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
#include <rep_qaria2_replica.h>

#include "mainwindow.h"


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


    auto uris = a.arguments();
    Q_ASSERT(!uris.isEmpty());
    uris.removeFirst();


    QRemoteObjectNode node;
    node.connectToNode(QS("local:qaria2"));

    QScopedPointer<QAria2Replica> replica(node.acquire<QAria2Replica>());
    auto connected = replica->waitForSource(500);

    if (connected)
    {
        if (uris.isEmpty())
        {
            qWarning() << "Not first instance.";
            return 1;
        }

        replica->addUris(uris);
        return 0;
    }

    MainWindow w;
    w.show();

    if (!uris.isEmpty())
    {
        w.addUris(uris);
    }

    return QApplication::exec();
}
