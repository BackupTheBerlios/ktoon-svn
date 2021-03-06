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

#include <QPainter>
#include <QDialog>
#include <QPushButton>
#include <QApplication>
#include <QFile>
#include <QLabel>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QtDebug>
#include <QProcess>
#include <signal.h>

#include "ktcrashwidget.h"
#include "ktcrashhandler.h"

#include <kcore/kconfig.h>
#include <kcore/kdebug.h>

#include <unistd.h>

class TextArea : public QTextBrowser
{
    Q_OBJECT
    public:
        TextArea();
        ~TextArea();

    public slots:
        void setSource(const QUrl & name);
};

TextArea::TextArea()
{
    setOpenExternalLinks(true);
}

TextArea::~ TextArea()
{
}

void TextArea::setSource(const QUrl &name)
{
    if (name.scheme() == "http") {
        KCONFIG->beginGroup("General");
        QString browser = KCONFIG->value("Browser").toString();
        if (!browser.isEmpty())
            QProcess::startDetached(browser, QStringList() << name.toString());
    } else {
            QTextBrowser::setSource(name);
    }
}

#include "ktcrashwidget.moc"

KTCrashWidget::KTCrashWidget(int sig) : QDialog(0), m_sig(sig)
{
    setModal(true);

    setWindowTitle(CHANDLER->title());

    m_layout = new QVBoxLayout(this);
    m_tabber = new QTabWidget(this);
    m_layout->addWidget(m_tabber);

    QWidget *page1 = new QWidget;
    QVBoxLayout *page1layout = new QVBoxLayout(page1);
    QLabel *message = new QLabel("<font color="+CHANDLER->messageColor().name()+">"+ CHANDLER->message()+"</color>");

    page1layout->addWidget(message);

    QHBoxLayout *hbox = new QHBoxLayout;

    QString text = CHANDLER->defaultText();
    QImage img(CHANDLER->defaultImage());

    if (CHANDLER->containsSignalEntry(sig)) {
        text = CHANDLER->signalText(sig);
        img = QImage(CHANDLER->signalImage(sig));
    }

    QLabel *sigImg = new QLabel;
    sigImg->setPixmap(QPixmap::fromImage(img));

    hbox->addWidget(sigImg);

    TextArea *sigText = new TextArea();
    sigText->setHtml(text);
    hbox->addWidget(sigText);

    page1layout->addLayout(hbox);

    m_tabber->addTab(page1, tr("What's happening?"));

    QPushButton *launch = new QPushButton(CHANDLER->launchButtonLabel(),this);
    connect(launch, SIGNAL(clicked()), SLOT(restart()));
    m_layout->addWidget(launch);

    QPushButton *end = new QPushButton(CHANDLER->closeButtonLabel(),this);
    connect(end, SIGNAL(clicked()), SLOT(exit()));
    m_layout->addWidget(end);

    setLayout(m_layout);
}

KTCrashWidget::~KTCrashWidget()
{
}

void KTCrashWidget::setPid(int pid)
{
    m_pid = pid;
}

void KTCrashWidget::addBacktracePage(const QString &execInfo, const QString &backtrace)
{
    #ifdef K_DEBUG
        K_FUNCINFO << execInfo << " " << backtrace;
    #endif

    QWidget *btPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(btPage);
    layout->addWidget(new QLabel(tr("Executable information")));

    TextArea *fileInfo = new TextArea;
    fileInfo->setHtml(execInfo);

    layout->addWidget(fileInfo);
    layout->addWidget(new QLabel(tr("Backtrace")));

    TextArea *btInfo = new TextArea;
    btInfo->setHtml(backtrace);

    layout->addWidget(btInfo);

    m_tabber->addTab(btPage, tr("Backtrace"));
}

void KTCrashWidget::restart()
{
   QString path = QString::fromLocal8Bit(::getenv("KTOON_BIN")) + "/ktoon &";
   QByteArray ba = path.toAscii();
   system(ba.data());
   kill(m_pid, 9);
}

void KTCrashWidget::exit()
{
   kill(m_pid, 9);
}

