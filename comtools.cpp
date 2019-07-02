#include "comtools.h"

#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>

QHash<QString, QString> QSPTools::file_list;
QString QSPTools::file_path;
bool QSPTools::useCaseInsensitiveFilePath = true;

QString QSPTools::GetHexColor(const QColor color)
{
    return QString("%1%2%3").arg(color.red(), 2, 16, QLatin1Char( '0' )).arg(color.green(), 2, 16, QLatin1Char( '0' )).arg(color.blue(), 2, 16, QLatin1Char( '0' ));
}

QString QSPTools::HtmlizeWhitespaces(const QString& str)
{
    QString::const_iterator i;
    QChar ch, quote;
    QString out;
    size_t j, linepos = 0;
    bool isLastSpace = true;
    for (i = str.begin(); i != str.end(); ++i)
    {
        ch = *i;
        if(ch == QChar('<'))
        {
            quote = 0;
            while (i != str.end())
            {
                ch = *i;
                if (quote.unicode())
                {
                    if (ch == QChar('\\'))
                    {
                        if (++i == str.end()) break;
                        ch = *i;
                        if (ch == quote)
                        {
                            if(ch == QChar('"'))
                            {
                                out.append( QString("&quot;") );
                            }
                            else if(ch == QChar('\''))
                            {
                                out.append( QString("&apos;") );
                            }
                            ++i;
                            continue;
                        }
                        out.append( QChar('\\') );
                    }
                    if(ch == QChar('&'))
                    {
                        out.append( QString("&amp;") );
                    }
                    else if(ch == QChar('\n'))
                    {
                        out.append( QString("%0A") );
                    }
                    else if(ch == QChar('<'))
                    {
                        out.append( QString("&lt;") );
                    }
                    else if (ch == QChar('>'))
                    {
                        out.append( QString("&gt;") );
                    }
                    else
                    {
                        if (ch == quote)
                            quote = 0;
                        out.append(ch);
                    }
                }
                else
                {
                    out.append(ch);
                    if (ch == QChar('>'))
                        break;
                    else if (ch == QChar('"') || ch == QChar('\''))
                        quote = ch;
                }
                ++i;
            }
            if (i == str.end()) return out;
            isLastSpace = true;
        }
        else if(ch == QChar(' '))
        {
            if (isLastSpace)
                out.append( QString("&ensp;") );
            else
                out.append( QChar(' ') );
            isLastSpace = !isLastSpace;
            ++linepos;
        }
        else if(ch == QChar('\r'))
        {

        }
        else if(ch == QChar('\n'))
        {
            out.append( QString("<br>") );
            isLastSpace = true;
            linepos = 0;
        }
        else if(ch == QChar('\t'))
        {
            for (j = 4 - linepos % 4; j > 0; --j)
            {
                if (isLastSpace)
                    out.append( QString("&emsp;") );
                else
                    out.append( QChar(' ') );
                isLastSpace = !isLastSpace;
            }
            linepos += 4 - linepos % 4;
        }
        else
        {
            out.append(ch);
            isLastSpace = false;
            ++linepos;
        }
    }
    return out;
}

QString QSPTools::ProceedAsPlain(const QString& str)
{
    QString::const_iterator i;
    QChar ch;
    QString out;
    for (i = str.begin(); i != str.end(); ++i)
    {
        ch = *i;
        if( ch == QChar('<'))
        {
            out.append( QString("&lt;") );
        }
        else if(ch == QChar('>'))
        {
            out.append( QString("&gt;") );
        }
        else if(ch == QChar('&'))
        {
            out.append( QString("&amp;") );
        }
        else
        {
            out.append(ch);
        }
    }
    return out;
}

QString QSPTools::GetAppPath()
{
    return QCoreApplication::applicationDirPath();
}

QString QSPTools::GetCaseInsensitiveFilePath(QString  searchDir, QString originalPath)
{
    QString new_name = originalPath.replace("\\", "/");
    if(new_name.startsWith("/"))
        new_name = new_name.remove(0, 1);
#ifndef _WIN32
    if(useCaseInsensitiveFilePath)
    {
        QDir itDir(searchDir);
        if(file_path != searchDir && !searchDir.isEmpty())
        {
            file_list.clear();
            QDirIterator it(searchDir, QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext())
            {
                it.next();
                file_list.insert(itDir.relativeFilePath(it.filePath()).toLower(), itDir.relativeFilePath(it.filePath()));
            }
            file_path = searchDir;
        }
        if (file_list.contains(new_name.toLower()))
            return itDir.relativeFilePath(file_list.value(new_name.toLower()));
    }
#endif
    return new_name;
}

QString QSPTools::GetCaseInsensitiveAbsoluteFilePath(QString  searchDir, QString originalPath)
{
    QString new_name = originalPath.replace("\\", "/");
#ifndef _WIN32
    if(useCaseInsensitiveFilePath)
    {
        QDir itDir(searchDir);
        if(originalPath.startsWith(searchDir))
            new_name = new_name.remove(0, searchDir.length());
        if(file_path != searchDir && !searchDir.isEmpty())
        {
            file_list.clear();
            QDirIterator it(searchDir, QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext())
            {
                it.next();
                file_list.insert(itDir.relativeFilePath(it.filePath()).toLower(), itDir.relativeFilePath(it.filePath()));
            }
            file_path = searchDir;
        }
        if (file_list.contains(new_name.toLower()))
            return itDir.absoluteFilePath(file_list.value(new_name.toLower()));
    }
#endif
    return new_name;
}

QString QSPTools::qspStrToQt(const QSP_CHAR *str)
{
    //return QString::fromWCharArray(str.Str, (int)(str.End - str.Str));
    if(str == 0)
        return QString("");
    else
        return QString::fromUtf16(str);
}

QColor QSPTools::wxtoQColor(int wxColor)
{
    QColor col;
    if(wxColor == 0)
    {
        col = Qt::black;
        return col;
    }
    col = QColor::fromRgba(wxColor);
    int red = col.red();
    col.setRed(col.blue());
    col.setBlue(red);
    return col;
}
