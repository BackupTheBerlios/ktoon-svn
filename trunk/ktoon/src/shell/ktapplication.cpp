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

#include <qdesktopwidget.h>
#include <QRect>

#include "ktapplication.h"
#include "configwizard.h"

// Including some KOM headers
#include <kcore/kapplicationproperties.h>
#include <kcore/kdebug.h>

/**
 * Support Class for main.cpp
 * This class contains some of the basic methods required when KToon is launched
 * @author David Cuadrado <krawek@toonka.com>
*/

KTApplication::KTApplication(int &argc, char **argv) : KApplication(argc, argv)
{
    setApplicationName("ktoon");
}

KTApplication::~KTApplication()
{
}

bool KTApplication::firstRun()
{
    QDesktopWidget *screen = QApplication::desktop();
    int screenW = screen->width();     // returns desktop width
    int screenH = screen->height();    // returns desktop height

    ConfigWizard *firstDialog = new ConfigWizard();
    QRect size = firstDialog->frameGeometry();
    int configH = size.height();
    int configW = size.width(); 
    firstDialog->move((screenW-configW)/2, (screenH-configH)/2);

    QApplication::setActiveWindow(firstDialog);

    if (firstDialog->exec() != QDialog::Rejected) {
        kAppProp->setHomeDir(firstDialog->home());
        createCache(firstDialog->cache());

        KCONFIG->beginGroup("General");
        KCONFIG->setValue("Home", HOME_DIR);
        KCONFIG->setValue("Cache", CACHE_DIR);
        KCONFIG->sync();

        delete firstDialog;

        return true;
    }
    delete firstDialog;

    return false;
}

void KTApplication::createCache(const QString &cacheDir)
{
    QDir cache(cacheDir);
    if (! cache.exists()) {
        #ifdef K_DEBUG
               kDebug() << tr("Initializing repository %1").arg(cacheDir);
        #endif

       if (! cache.mkdir(cacheDir)) {
           #ifdef K_DEBUG
                  kError() << tr("Can not create the projects repository");
           #endif
       }
    }

    kAppProp->setCacheDir(cacheDir);
}
