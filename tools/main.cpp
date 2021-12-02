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
#include <QCoreApplication>


static QString ToCamelCase(const QString &s)
{
    QString camel;
    bool nextUpper = true;

    for (const auto &ch : s)
    {
        if (ch == QL('-'))
        {
            nextUpper = true;
        }
        else if (nextUpper)
        {
            camel.append(ch.toUpper());
            nextUpper = false;
        }
        else
        {
            camel.append(ch);
        }
    }

    return camel;
}


static bool isNumeric(const QString &value)
{
    static const QStringList numerics{
        QS("n"), QS("num"), QS("sec"), QS("pid"), QS("size"),
    };

    return numerics.contains(value);
}


static void parseOptions(QFile *input, const QString &outputFilename)
{
    QSaveFile output(outputFilename);
    output.setDirectWriteFallback(true);

    if (!output.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
    {
        qCritical() << "Failed to open output file:" << output.errorString();
        return;
    }

    input->reset();

    QTextStream in(input);
    in.setCodec("UTF-8");
    in.setAutoDetectUnicode(true);

    QTextStream out(&output);
    out.setCodec("UTF-8");
    out.setGenerateByteOrderMark(true);

    out << "/*! ***********************************************************************************************" << Qt::endl;
    out << " *" << Qt::endl;
    out << " * \\file        " << outputFilename << Qt::endl;
    out << " * \\brief       " << QS(u"OptionsBuilder 类头文件。") << Qt::endl;
    out << " * \\version     0.1" << Qt::endl;
    out << " * \\date        " << QDate::currentDate().toString(Qt::ISODate) << Qt::endl;
    out << " *" << Qt::endl;
    out << " * \\author      Roy QIU <karoyqiu@gmail.com>" << Qt::endl;
    out << QS(u" * \\copyright   © 2021 karoyqiu。") << Qt::endl;
    out << " *" << Qt::endl;
    out << " **************************************************************************************************/" << Qt::endl;
    out << "#pragma once" << Qt::endl;
    out << "#include <QVariantHash>" << Qt::endl << Qt::endl << Qt::endl;

    out << "class OptionsBuilder" << Qt::endl;
    out << "{" << Qt::endl;
    out << "public:" << Qt::endl;

    while (!in.atEnd())
    {
        auto line = in.readLine();

        if (line.endsWith(QL(" Options")))
        {
            out << "    //////////////////////////////////////////////////////////////////////////" << Qt::endl;
            out << "    //" << Qt::endl;
            out << "    // " << line << Qt::endl;
            out << "    //" << Qt::endl;
            out << "    //////////////////////////////////////////////////////////////////////////" << Qt::endl << Qt::endl;
        }
        else if (line.startsWith(QL(".. option:: ")))
        {
            static const QRegularExpression withValue(QS(R"(\-\-([a-z-]+)=<([A-Z]+)>)"));

            if (line.contains(withValue))
            {
                auto match = withValue.match(line);
                auto name = match.captured(1);

                if (name.startsWith(QL("on")))
                {
                    continue;
                }

                auto value = match.captured(2).toLower();

                line = in.readLine();
                Q_ASSERT(line.isEmpty());
                line = in.readLine().trimmed();

                out << "    /*!" << Qt::endl;

                while (!line.isEmpty())
                {
                    out << "     * " << line << Qt::endl;
                    line = in.readLine().trimmed();
                }

                out << "     */" << Qt::endl;
                out << "    void set" << ToCamelCase(name) << "(";

                if (isNumeric(value))
                {
                    out << "int " << value;
                }
                else
                {
                    out << "const QString &" << value;
                }

                out << ")" << Qt::endl;
                out << "    { hash_.insert(QS(\"" << name << "\"), " << value << "); }" << Qt::endl << Qt::endl;
            }
        }
    }

    out << "    const QVariantHash &options() const { return hash_; }" << Qt::endl << Qt::endl;
    out << "private:" << Qt::endl;
    out << "    QVariantHash hash_;" << Qt::endl;
    out << "};" << Qt::endl;

    output.commit();
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName(QS("qaria"));
    QCoreApplication::setOrganizationName(QS("karoyqiu"));
    QCoreApplication::setOrganizationDomain(QS("karoyqiu.google.com"));

    const auto args = QCoreApplication::arguments();
    QString filename;

    if (args.length() != 2)
    {
        filename = QS(R"(C:\Users\karoy\source\repos\qaria\tools\aria2c.rst.txt)");
        //return 1;
    }
    else
    {
        filename = args.last();
    }

    QFile file(filename);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qCritical() << "Failed to open input file:" << file.errorString();
        return 2;
    }

    QFileInfo info(file.fileName());
    auto dir = info.dir();

    parseOptions(&file, dir.absoluteFilePath(QS("aria2options.h")));

    return 0;
}
