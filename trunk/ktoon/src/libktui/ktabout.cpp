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
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
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

    QImage credits = QImage(DATA_DIR + "/images/credits.png");

    KImageEffect::fade(credits,0.25, palette().background().color());
  
    m_credits = new KAnimWidget(QPixmap::fromImage(credits), creditsText);
    addTab(m_credits, tr("Credits"));

    QPalette pal = m_credits->palette();
    pal.setColor(QPalette::Foreground, QColor(50, 50, 50, 255));

    m_credits->setPalette(pal);
    m_credits->setFont(QFont("verdana", 24));

    // 2: Ack

    QLabel *ack = new QLabel;

    QImage ackImg(DATA_DIR + "/images/sponsors.png");
    KImageEffect::fade(ackImg,0.2,palette().background().color()); 

    ack->setPixmap(QPixmap::fromImage(ackImg));

    addTab(ack, tr("Sponsors"));

    // 4: Toonka Films
    /*
    QLabel *toonka = new QLabel;
    QImage toonkaImg(DATA_DIR + "/images/toonka.png");

    KImageEffect::fade(toonkaImg,0.2,palette().background().color()); 

    toonka->setPixmap(QPixmap::fromImage(toonkaImg));
    addTab(toonka, "Toonka Films");
    */

    // 5: Laboratoon

    QLabel *laboratoon = new QLabel;

    QImage laboratoonImg(DATA_DIR + "/images/laboratoon.png");
    KImageEffect::fade(laboratoonImg,0.2,palette().background().color()); 

    laboratoon->setPixmap(QPixmap::fromImage(laboratoonImg));
    addTab(laboratoon, "Laboratoon");

    // 6: Licence

    QTextBrowser *licenceText = new QTextBrowser;
    QFile licenceFile(DATA_DIR + "/license.html");
    QString line = "";
    if (licenceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&licenceFile);
        while (! stream.atEnd()) {
               line += stream.readLine();
               //licenceText->append(line);
        }
        licenceFile.close();
    }
    licenceText->setHtml(line);
    licenceText->moveCursor(QTextCursor::Start);

    addTab(licenceText, tr("License Agreement"));
    setButtonText(Cancel, tr("Close"));
}

KTAbout::~KTAbout()
{
}
