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
        QS("n"), QS("num"), QS("sec"), QS("pid"),
    };

    return numerics.contains(value);
}


static QString valueType(const QString &value)
{
    if (value == QL("size"))
    {
        return QS("qint64 ");
    }

    if (isNumeric(value))
    {
        return QS("int ");
    }

    return QS("const QString &");
}


static void writeFunction(QTextStream &in, QTextStream &out, const QString &name,
                          const QString &valueType, const QString &value)
{
    if (name.startsWith(QS("on")))
    {
        return;
    }

    auto line = in.readLine();
    Q_ASSERT(line.isEmpty());
    line = in.readLine().trimmed();

    out << "    /*!" << Qt::endl;

    while (!line.isEmpty())
    {
        out << "     * " << line << Qt::endl;
        line = in.readLine().trimmed();
    }

    QString v = value;

    if (value == QL("interface"))
    {
        v = QS("itfc");
    }

    out << "     */" << Qt::endl;
    out << "    void set" << ToCamelCase(name) << "(" << valueType << v;

    if (valueType == QL("bool "))
    {
        out << " = true";
        v = QS(R"((%1 ? QL("true") : QL("false")))").arg(v);
    }
    else if (!valueType.contains(QS("QString")))
    {
        v = QS("QSS(%1)").arg(v);
    }

    out << ")" << Qt::endl;
    out << "    { hash_.insert(QS(\"" << name << "\"), " << v << "); }" << Qt::endl << Qt::endl;
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
    out << "#include <QVariantHash>" << Qt::endl << Qt::endl;

    out << R"(inline constexpr qint64 operator"" _K(quint64 value))" << Qt::endl;
    out << "{" << Qt::endl;
    out << "    return value * 1024;" << Qt::endl;
    out << "}" << Qt::endl << Qt::endl;

    out << R"(inline constexpr qint64 operator"" _M(quint64 value))" << Qt::endl;
    out << "{" << Qt::endl;
    out << "    return value * 1024 * 1024;" << Qt::endl;
    out << "}" << Qt::endl << Qt::endl;

    out << R"(inline constexpr qint64 operator"" _min(quint64 value))" << Qt::endl;
    out << "{" << Qt::endl;
    out << "    return value * 60;" << Qt::endl;
    out << "}" << Qt::endl << Qt::endl;

    out << R"(inline constexpr qint64 operator"" _h(quint64 value))" << Qt::endl;
    out << "{" << Qt::endl;
    out << "    return value * 60 * 60;" << Qt::endl;
    out << "}" << Qt::endl << Qt::endl;

    out << R"(inline constexpr qint64 operator"" _d(quint64 value))" << Qt::endl;
    out << "{" << Qt::endl;
    out << "    return value * 60 * 60 * 24;" << Qt::endl;
    out << "}" << Qt::endl << Qt::endl << Qt::endl;

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
            static const QRegularExpression withValue(QS(R"(--([a-z-]+)=<([A-Z]+)>)"));
            static const QRegularExpression withBoolean(QS(R"(--([a-z-]+)\s\[true\|false\])"));

            if (line.contains(withValue))
            {
                auto match = withValue.match(line);
                auto name = match.captured(1);
                auto value = match.captured(2).toLower();
                auto type = valueType(value);
                writeFunction(in, out, name, type, value);
            }
            else if (line.contains(withBoolean))
            {
                auto match = withBoolean.match(line);
                auto name = match.captured(1);
                writeFunction(in, out, name, QS("bool "), QS("value"));
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
        filename = QS(R"(C:\Users\karoy\source\repos\qaria2\tools\aria2c.rst.txt)");
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

    parseOptions(&file, dir.absoluteFilePath(QS("../src/aria2optionsbuilder.h")));

    return 0;
}
