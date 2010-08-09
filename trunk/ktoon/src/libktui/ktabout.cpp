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

#include <QFile>
#include <QTextStream>
#include <QImage>

#include <QPixmap>
#include <QLabel>
#include <QScrollArea>
#include <QTextBrowser>

#include "ktabout.h"
#include <kcore/kglobal.h>
#include <kgui/kimageeffect.h>

#include <kcore/kdebug.h>

#define DEBUG_ABOUT 1

/**
 * This class defines the About dialog of KToon.
 * Here is where the dialog tabs are setted.
 * @author David Cuadrado <krawek@toonka.com>
*/

KTAbout::KTAbout(QWidget *parent) : KTabDialog(Cancel, parent)
{
    setWindowTitle(tr("About") + QString(" KToon"));
    setFixedSize(525, 458);

    QString lang = "en";
    if (QString(QLocale::system().name()).length() > 1)
        lang = QString(QLocale::system().name()).left(2);

    Qt::WindowFlags flags = 0;
    flags = Qt::Dialog;
    flags |= Qt::CustomizeWindowHint;
    setWindowFlags(flags);

    //1: Credits
    QFile creditsFile(DATA_DIR+"/credits.txt");
    QString creditsText;
    if (creditsFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&creditsFile);
        while (!stream.atEnd()) {
               QString line = stream.readLine();
               creditsText += line + "\n";
        }
        creditsFile.close();
    } else {
        #ifdef K_DEBUG
               kError() << "Error while trying to read " << creditsFile.fileName();
        #endif
    }

    QImage credits = QImage(THEME_DIR + "/images/credits.png");

    KImageEffect::fade(credits,0.25, palette().background().color());

    m_credits = new KAnimWidget(QPixmap::fromImage(credits), creditsText);
    addTab(m_credits, tr("Credits"));

    QPalette pal = m_credits->palette();
    pal.setColor(QPalette::Foreground, QColor(50, 50, 50, 255));

    m_credits->setPalette(pal);
    m_credits->setFont(QFont("verdana", 24));

    // 2: Sponsors 

    QTextBrowser *sponsorsText = new QTextBrowser;
    sponsorsText->setOpenExternalLinks(true);

    sponsorsText->setSource(SHARE_DIR + "data/help/" + lang + "/sponsors.html");
    sponsorsText->moveCursor(QTextCursor::Start);

    addTab(sponsorsText, tr("Sponsors"));

    // 3: Laboratoon

    QTextBrowser *labtoonText = new QTextBrowser;
    labtoonText->setOpenExternalLinks(true);

    labtoonText->setSource(SHARE_DIR + "data/help/" + lang + "/labtoon_short.html");
    labtoonText->moveCursor(QTextCursor::Start);

    addTab(labtoonText, tr("Labtoon"));

    // 4: Licence

    QTextBrowser *licenseText = new QTextBrowser;
    licenseText->setOpenExternalLinks(true);

    /* QFile licenceFile(DATA_DIR + "/license.html");
    QFile licenseFile(SHARE_DIR + "data/help/" + lang + "/philosophy.html");
    QString line = "";
    if (licenseFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&licenseFile);
        while (! stream.atEnd()) {
               line += stream.readLine();
               //licenceText->append(line);
        }
        licenseFile.close();
    }
    licenseText->setHtml(line);
    */

    licenseText->setSource(SHARE_DIR + "data/help/" + lang + "/philosophy.html");
    licenseText->moveCursor(QTextCursor::Start);

    addTab(licenseText, tr("License Agreement"));
    setButtonText(Cancel, tr("Close"));
}

KTAbout::~KTAbout()
{
}
