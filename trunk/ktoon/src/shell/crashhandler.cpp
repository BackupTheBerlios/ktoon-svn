/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
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
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
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

#include <QString>
#include <QApplication>
#include <QDomDocument>
#include <QFile>
#include <QTemporaryFile>
#include <QProcess>
#include <QTranslator>

#include <csignal>
#include <cstdio>

extern "C"
{
#include <sys/types.h> //pid_t
#include <sys/wait.h>  //waitpid
#include <unistd.h>    //write, getpid
#include <stdio.h>
}

#include "crashhandler.h"
#include "crashwidget.h"
#include <kcore/kdebug.h>
#include <kcore/kglobal.h>

CrashHandler *CrashHandler::m_instance = 0;

void crashTrapper(int sig);

CrashHandler::CrashHandler() : m_verbose(false)
{
    m_program = QCoreApplication::applicationName();
    setTrapper(crashTrapper);

    m_config.title = QObject::tr("Fatal error");
    m_config.message = QObject::tr("%1 is crashing...").arg(m_program);
    m_config.buttonText = QObject::tr("Close");
    m_config.defaultText = QObject::tr("This is a general failure");
}

CrashHandler::~CrashHandler()
{
    if (m_instance) 
        delete m_instance;
}

CrashHandler *CrashHandler::instance()
{
    init();
    return m_instance;
}
			
void CrashHandler::init()
{
    if (m_instance == 0)
        m_instance = new CrashHandler();	
}

void CrashHandler::setTrapper (void (*trapper)(int))
{
#ifdef Q_OS_UNIX
    if (!trapper)
        trapper = SIG_DFL;

    sigset_t mask;
    sigemptyset(&mask);
    signal(SIGSEGV,trapper);
    sigaddset(&mask, SIGSEGV);
    signal(SIGFPE,trapper);
    signal(SIGILL,trapper);
    sigaddset(&mask, SIGILL);
    signal(SIGABRT, trapper);
    sigaddset(&mask, SIGABRT);
    signal(SIGBUS,trapper);
    signal(SIGIOT,trapper);
    sigprocmask(SIG_UNBLOCK, &mask, 0);
#endif
}

QString CrashHandler::program() const
{
    return m_program;
}

void CrashHandler::setProgram(const QString &prog)
{
    m_program = prog;
}

void CrashHandler::setImagePath(const QString &imagePath)
{
    m_imagePath = imagePath;
}

QString CrashHandler::imagePath() const
{
    return m_imagePath;
}

QString CrashHandler::title() const
{
    return m_config.title;
}

QString CrashHandler::message() const
{
    return m_config.message;
}

QColor CrashHandler::messageColor() const
{
    if (m_config.messageColor.isValid())
        return m_config.messageColor;

    return QApplication::palette().color(QPalette::Text);
}

QString CrashHandler::buttonText() const
{
    return m_config.buttonText;
}

QString CrashHandler::defaultText() const
{
    return m_config.defaultText;
}

QString CrashHandler::defaultImage() const
{
    return m_imagePath + "/" + m_config.defaultImage;
}

QString CrashHandler::signalText(int signal)
{
    return m_config.signalEntry[signal].first;
}

QString CrashHandler::signalImage(int signal)
{
    return m_imagePath + "/" + m_config.signalEntry[signal].second;
}

bool CrashHandler::containsSignalEntry(int signal)
{
    return m_config.signalEntry.contains(signal);
}

void CrashHandler::setConfig(const QString &filePath)
{
#ifdef K_DEBUG
       K_FUNCINFO;
       //SHOW_VAR(filePath);
#endif

    QDomDocument doc;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();
	
    QDomElement docElem = doc.documentElement();

    if (docElem.tagName() == "CrashHandler") {
        QDomNode n = docElem.firstChild();
        while (!n.isNull()) {
               QDomElement e = n.toElement();
               if (!e.isNull()) {
                   if (e.tagName() == "Title") {
                       m_config.title = e.attribute("text");
                   } else if (e.tagName() == "Message") {
                              m_config.message = e.attribute("text");
                              m_config.messageColor = QColor(e.attribute("color"));
                   } else if (e.tagName() == "Button") {
                              m_config.buttonText = e.attribute("text");
                   } else if (e.tagName() == "Default") {
                              m_config.defaultText = "<p align=\"justify\">" + e.attribute("text") + "</p>";
                              m_config.defaultImage = e.attribute("image");
                   } else if (e.tagName() == "Signal") {
                              int signalId = e.attribute("id").toInt();
                              m_config.signalEntry.insert(signalId, qMakePair(e.attribute("text"), e.attribute("image")));
                   }
               }
               n = n.nextSibling();
        }
    }
}

static QString runCommand(const QString &command)
{
    static const uint SIZE = 40960; //40 KiB
    static char buf[SIZE];
    QString result = "";

#ifdef K_DEBUG
    kDebug() << "Running command: " << command;
#endif

    FILE *process = ::popen(command.toLocal8Bit().data(), "r");

    while (fgets(buf, SIZE-1, process) != NULL) {
           result += buf;
    }

    ::pclose(process);

    result.replace(QString("#"), QString("<p></p>#"));
    result.replace(QString("]"), QString("]<p></p>"));

    return result;
}

void crashTrapper (int sig)
{

#ifdef K_DEBUG
    qDebug("\n*** Fatal error: %s is crashing with signal %d :(", CHANDLER->program().toLocal8Bit().data(), sig);

    if (sig == 6) {
        qDebug("Signal 6: The process itself has found that some essential pre-requisite");
        qDebug("for correct function is not available and voluntarily killing itself.");
    }

    if (sig == 11) {
        qDebug("Signal 11: Officially known as \"segmentation fault\", means that the program");
        qDebug("accessed a memory location that was not assigned. That's usually a bug in the program.");
    }
#endif

    CHANDLER->setTrapper(0); // Unactive crash handler

    bool isActive = true;

    QApplication* application = dynamic_cast<QApplication *>(QApplication::instance());

    isActive = false;
    int argc = 1;
    char *argv[] = { CHANDLER->program().toUtf8().data(), 0 };
    application = new QApplication(argc, argv);

    QString locale = QString(QLocale::system().name()).left(2);

    QTranslator *translator = new QTranslator;
    translator->load(SHARE_DIR + "data/translations/" + "ktoon_" + locale + ".qm");
    application->installTranslator(translator);

    const pid_t pid = ::fork();

    if (pid <= 0) {
        QString bt;
        QString execInfo;

        // so we can read stderr too
        ::dup2(fileno(stdout), fileno(stderr));

        QString gdb;
        gdb  = "gdb -nw -n -ex bt -batch";
        gdb += " " + HOME_DIR + "bin/ktoon.bin ";
        gdb += QString::number(::getppid());

        bt = runCommand(gdb);

        /// clean up
        bt.remove(QRegExp("\\(no debugging symbols found\\)"));
        bt = bt.simplified();

        execInfo = runCommand("file " + HOME_DIR + "bin/ktoon.bin");

        // Widget
        CrashWidget widget(sig);
        widget.addBacktracePage(execInfo, bt);
        widget.exec();

        if (!isActive)
            application->exec();

        ::exit(255);
    } else {
        // Process crashed!
        ::alarm( 0 );
        // wait for child to exit
        ::waitpid( pid, NULL, 0 );
    }

    exit(128);
}
