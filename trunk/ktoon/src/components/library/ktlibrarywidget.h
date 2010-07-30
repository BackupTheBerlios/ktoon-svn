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

#ifndef KTLIBRARYWIDGET_H
#define KTLIBRARYWIDGET_H

#include <ktmodulewidgetbase.h>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QDir>

#include "ktitempreview.h"
#include <kgui/kimagebutton.h>

#include "ktgctable.h"

class KTLibrary;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTLibraryWidget : public KTModuleWidgetBase
{
    Q_OBJECT
    public:
        KTLibraryWidget(QWidget *parent = 0);
        ~KTLibraryWidget();

        void setLibrary(const KTLibrary *library);

    protected:
        virtual void libraryResponse(KTLibraryResponse *response);
        virtual void frameResponse(KTFrameResponse *response);

    private slots:
        void addFolder(const QString &name);
        void previewItem(QTreeWidgetItem *, int);
        void emitSelectedComponent();
        void removeCurrentGraphic();
        void renameObject( QTreeWidgetItem* item);

    public slots:
        void importBitmap();
        void importSvg();
        void importBitmapArray();
        void importSound();

    signals:
        void requestCurrentGraphic();

    private:
        struct Private;
        Private *const k;
};

#endif
