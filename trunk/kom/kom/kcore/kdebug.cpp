/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *                                                                         *
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/


#if !defined(K_NODEBUG)
#include "kdebug.h"

#include <QFile>
#include <QString>
#include <QDateTime>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QStringList>
#include <QVariant>
#include <QSize>
#include <QEvent>
#include <QTimer>

#ifdef QT_GUI_LIB

#include <QRegion>
#include <QPen>
#include <QBrush>
#include <QImage>
#include <QIcon>
#include <QPixmap>
#include <QWidget>
#include <QMessageBox>
#include <QTextBrowser>
#include <QSyntaxHighlighter>
#include <QMatrix>

#endif

#include <QSettings>

#if defined(Q_OS_UNIX)
# define SHOW_ERROR "*** \033[0;31m%s\033[0;0m ***\n"
# define SHOW_WARNING "-> \033[10;33m%s\033[0;0m\n"
# define SHOW_FATAL "***** \033[0;35m%s\033[0;0m *****\n"
#else
# define SHOW_ERROR "*** %s ***\n"
# define SHOW_WARNING "-> %s\n"
# define SHOW_FATAL "***** %s *****\n"
#endif

#ifdef QT_GUI_LIB
       static QTextBrowser *debugBrowser = 0;
#endif


#ifdef Q_OS_UNIX

static class Colors
{
    public:
        Colors()
          {
             m_psb << "\033[90m" << "\033[91m" << "\033[92m" << "\033[93m" << "\033[94m" << "\033[95m" << "\033[96m" << "\033[97m" << "\033[31m" << "\033[32m" << "\033[33m" << "\033[34m" << "\033[35m" << "\033[36m" << "\033[44m" << "\033[38m" << "\033[43m";

             qsrand(::time(0));
          }

        ~Colors() {}

        QString colorize(const QString &area)
          {
                if (! m_colors.contains(area))
                    m_colors[area] = m_psb[qrand() % m_psb.size()];

                return QString("%1%2\033[0m").arg(m_colors[area]).arg(area);
          }

    private:
        QMap<QString, QString> m_colors;
        QVector<QString> m_psb;
} colors;

#endif

static class ConfigReader
{
    public:
        ConfigReader();
        ~ConfigReader();

        QStringList areas;

        bool colorize;
        bool showArea;
        bool showAll;
        bool forceDisableGUI;
        DebugOutput defaultOutput;
} configReader;

ConfigReader::ConfigReader()
{
    QSettings settings("kdebug");
    settings.beginGroup("Iface");

    areas = settings.value("areas", QStringList()).toStringList();
    showArea = settings.value("show_area", false).toBool();
    showAll = settings.value("show_all", true).toBool();

    defaultOutput= DebugOutput(settings.value("default", KShellOutput).toInt());
    forceDisableGUI = false;
    colorize = false;
    
#ifdef Q_OS_UNIX
    QString terminal = QString::fromLocal8Bit(::getenv("TERM"));
    if (terminal == "linux" || terminal == "xterm")
        colorize = true;
#endif
}

ConfigReader::~ConfigReader()
{
    QSettings settings("kdebug");
    settings.beginGroup("Iface");

    if (areas.isEmpty())
        settings.setValue("areas", "");
    else
        settings.setValue("areas", areas);

    settings.setValue("show_area", showArea);
    settings.setValue("show_all", showAll);
    settings.setValue("default", defaultOutput);

    if (debugBrowser) {
        if (debugBrowser->parentWidget() == 0 )
            delete debugBrowser;
    }
}

#ifdef QT_GUI_LIB

class DebugBrowserHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT;
    public:
        DebugBrowserHighlighter(QTextDocument *doc);
        ~DebugBrowserHighlighter() {};

    protected:
        virtual void highlightBlock(const QString & text);

    private:
        QMap<QString, QColor> m_colors;
};

#include "kdebug.moc"

DebugBrowserHighlighter::DebugBrowserHighlighter(QTextDocument *doc) : QSyntaxHighlighter(doc)
{
    QVector<int> colorIndexes = QVector<int>() << 7 << 13 << 8 << 14 <<
                 9 <<
                15 <<
                10 <<
                16 <<
                11 <<
                17 <<
                18;

    int count = 0;
    foreach(QString area, configReader.areas)
            m_colors.insert(area, QColor(Qt::GlobalColor(colorIndexes[count++ % colorIndexes.count()])) );
}

void DebugBrowserHighlighter::highlightBlock(const QString &text)
{
    int sepIndex = text.indexOf(":");

    if (sepIndex < 0) 
        return;

    QString area = text.left(sepIndex);

    if (!m_colors.contains(area)) 
        return;

    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);
    format.setForeground(m_colors[area]);

    setFormat(0, sepIndex, format);
}

#endif // QT_GUI_LIB

static void kDebugOutput(DebugType t, DebugOutput o, const char *data)
{
    if ((o == KBoxOutput) || (o == KBrowserOutput && configReader.forceDisableGUI)) {
        o = KShellOutput;
        configReader.defaultOutput = KShellOutput;
    }

    char const *output = "%s\n";

    if (configReader.colorize) {
        switch(t) {
               case KDebugMsg:
                    {
                      // output = "%s\n";
                    }
               break;
               case KWarningMsg:
                    {
                      output = SHOW_WARNING;
                    }
               break;
               case KErrorMsg:
                    {
                      output = SHOW_ERROR;
                    }
               break;
               case KFatalMsg:
                    {
                      output = SHOW_FATAL;
                    }
               break;
        }
    }

    switch (o) {
            case KShellOutput:
               {
                 fprintf(stderr, output, data);
               }
               break;
            case KFileOutput:
               {
                 QFile outFile("kdebug.log");

                 if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                     outFile.write(data, strlen(data));
                     outFile.close();
                 }
               }
            break;
            #ifdef QT_GUI_LIB
            case KBoxOutput:
               {
                    switch (t) {
                            case KDebugMsg:
                               {
                                   QMessageBox::information(0, QObject::tr("Information"), data,  QMessageBox::Ok);
                               }
                            break;
                            case KWarningMsg:
                               {
                                   QMessageBox::warning(0, QObject::tr("Warning"), data);
                               }
                            break;
                            case KErrorMsg:
                               {
                                   QMessageBox::critical(0, QObject::tr("Error"), data);
                               }
                            break;
                            case KFatalMsg:
                               {
                                   QMessageBox::critical(0, QObject::tr("Critical"), data);
                               }
                            break;
                    }
               }
            break;
            case KBrowserOutput:
               {
                    KDebug::browser()->append(QString(data));
               }
            break;
            #endif
            default: 
            break;
    }
}

KDebug::KDebug(DebugType t, const QString &area, DebugOutput o) : m_type(t), m_output(o), m_area(area)
{
    streamer = new Streamer();

    if (configReader.showArea && !m_area.isEmpty()) {
        QString init = "";

        if (configReader.colorize)
            init = colors.colorize(m_area);
        else
            init = m_area;

        *streamer << init << ": ";
    }

    if (m_output == KDefault)
        m_output = configReader.defaultOutput;
};

KDebug::KDebug(const KDebug & k) : streamer(k.streamer), m_type(k.m_type), m_output(k.m_output), m_area(k.m_area)
{
}

KDebug::~KDebug()
{
    if ((m_area.isEmpty() && configReader.showAll) || configReader.areas.contains(m_area))
        ::kDebugOutput(m_type, m_output, streamer->buffer.toLocal8Bit().data());

    delete streamer;
}

void KDebug::setForceDisableGUI()
{
    configReader.forceDisableGUI = true;
}

KDebug& KDebug::operator << (const QDateTime& time) 
{
    *this << time.toString();

    return *this;
}

KDebug& KDebug::operator << (const QDate& date) 
{
    *this << date.toString();

    return *this;
}

KDebug& KDebug::operator << (const QTime& time) 
{
    *this << time.toString();

    return *this;
}

KDebug& KDebug::operator << (const QPoint & p)  
{
    *this << "(" << p.x() << ", " << p.y() << ")";

    return *this;
}

KDebug& KDebug::operator << (const QPointF & p)  
{
    *this << "(" << p.x() << ", " << p.y() << ")";

    return *this;
}

KDebug& KDebug::operator << (const QSize & s)  
{
    *this << "[" << s.width() << "x" << s.height() << "]";

    return *this;
}

KDebug& KDebug::operator << (const QRect & r)  
{
    *this << "[" << r.x() << "," << r.y() << " - " << r.width() << "x" << r.height() << "]";

    return *this;
}

KDebug& KDebug::operator << (const QStringList & l) 
{
    *this << "(";
    *this << l.join(",");
    *this << ")";

    return *this;
}


KDebug& KDebug::operator << (const QVariant & v) 
{
    *this << "[variant: ";
    *this << v.typeName();
    *this << " toString=";
    *this << v.toString();
    *this << "]";

    return *this;
}

KDebug& KDebug::operator << (const QEvent* e)
{
    *this << "[Event " << e->type() << "]";

    return *this;
}

#ifdef QT_GUI_LIB
KDebug& KDebug::operator<<( const QPixmap& p ) 
{
    *this << "(" << p.width() << ", " << p.height() << ")";

    return *this;
}

KDebug& KDebug::operator<<( const QIcon& p )
{
    *this << "(" << p.pixmap(QSize() ).width() << ", " << p.pixmap(QSize()).height() << ")";

    return *this;
}

KDebug& KDebug::operator << (const QImage& p) 
{
    *this << "(" << p.width() << ", " << p.height() << ")";

    return *this;
}

KDebug& KDebug::operator << (const QRegion & reg) 
{
    *this<< "[ ";

    QVector<QRect> rs = reg.rects();

    for (int i=0;i<rs.size();++i)
         *this << QString("[%1,%2 - %3x%4] ").arg(rs[i].x()).arg(rs[i].y()).arg(rs[i].width()).arg(rs[i].height()) ;

    *this <<"]";

    return *this;
}

KDebug& KDebug::operator << (const QColor & c) 
{
    if (c.isValid())
        *this << c.name();
    else
        *this << "(invalid/default)";

    return *this;
}

KDebug& KDebug::operator << (const QPen & p) 
{
    static const char* const s_penStyles[] = {
           "NoPen", "SolidLine", "DashLine", "DotLine", "DashDotLine",
           "DashDotDotLine" };

    static const char* const s_capStyles[] = { "FlatCap", "SquareCap", "RoundCap" };
    *this << "[ style:";
    *this << s_penStyles[ p.style() ];
    *this << " width:";
    *this << p.width();
    *this << " color:";

    if (p.color().isValid())
        *this << p.color().name();
    else
        *this <<"(invalid/default)";

    if (p.width() > 0) { // cap style doesn't matter, otherwise
        *this << " capstyle:";
        *this << s_capStyles[ p.capStyle() >> 4 ];
        // join style omitted
    }

    *this <<" ]";

    return *this;
}

KDebug& KDebug::operator << (const QBrush & b)
{
    if (b.gradient()) {
        *this << b.gradient();
    } else {
        static const char* const s_brushStyles[] = {
               "NoBrush", "SolidPattern", "Dense1Pattern", "Dense2Pattern", "Dense3Pattern",
               "Dense4Pattern", "Dense5Pattern", "Dense6Pattern", "Dense7Pattern",
               "HorPattern", "VerPattern", "CrossPattern", "BDiagPattern", "FDiagPattern",
               "DiagCrossPattern" };

        *this <<"[ style: ";
        *this <<s_brushStyles[ b.style() ];
        *this <<" color: ";

        // can't use operator<<(str, b.color()) because that terminates a kdbgstream (flushes)
        if (b.color().isValid())
            *this <<b.color().name() ;
        else
            *this <<"(invalid/default)";

        if (! b.texture().isNull())
            *this <<" has a texture";
        *this <<" ]";
    }

    return *this;
}

KDebug& KDebug::operator << (const QWidget* t) 
{
    if (t)
        *this << "[Widget " + QString::fromLocal8Bit(t->metaObject ()->className()) + " geometry: " << t->width() << "x"<< t->height() << "]";
    else
        *this << "[Null Widget]";

    return *this; 
}

KDebug& KDebug::operator << (const QLinearGradient &g)
{
    *this << "QLinearGradient start=" << g.start() << " stop="<<g.finalStop();

    return *this;
}

KDebug& KDebug::operator << (const QRadialGradient &g)
{
    *this << "QRadialGradient center=" << g.center() << " focal="<<g.focalPoint() << " radius=" << g.radius();

    return *this;
}

KDebug& KDebug::operator << (const QConicalGradient &g)
{
    *this << "QConicalGradient center=" << g.center() << " angle="<<g.angle();

    return *this;
}

KDebug& KDebug::operator << (const QGradient *g)
{
    switch (g->type()) {
            case QGradient::LinearGradient:
               {
                 *this << static_cast<const QLinearGradient &>(*g);
               }
            break;
            case QGradient::RadialGradient:
               {
                 *this << static_cast<const QRadialGradient &>(*g);
               }
            break;
            case QGradient::ConicalGradient:
               {
                 *this << static_cast<const QConicalGradient &>(*g);
               }
            break;
            default: break;
    }

    return *this;
}

KDebug& KDebug::operator << (const QMatrix &m)
{
    *this << "\n";
    *this << "|" << m.m11() << "\t" << m.m12() << "\t" << 0 << "\t|\n";
    *this << "|" << m.m21() << "\t" << m.m22() << "\t" << 0<< "\t|\n";
    *this << "|" << m.dx() << "\t" << m.dy() << "\t" << 1<< "\t|\n";

    return *this;
}

void KDebug::resaltWidget(QWidget *w, const QColor &color)
{
    QPalette pal = w->palette();
    pal.setColor(QPalette::Background, color);
    w->setPalette(pal);
}

QTextBrowser *KDebug::browser()
{
    if (!debugBrowser) {
        debugBrowser = new QTextBrowser;
        new DebugBrowserHighlighter(debugBrowser->document());
    }

    return debugBrowser;
}


#endif // QT_GUI_LIB

#endif // K_NODEBUG
