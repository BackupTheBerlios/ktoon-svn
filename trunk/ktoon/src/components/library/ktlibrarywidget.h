/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
 *                                                                         *
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

#ifndef KTLIBRARYWIDGET_H
#define KTLIBRARYWIDGET_H

#include <ktmodulewidgetbase.h>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QDir>

#include "ktitempreview.h"
#include "dimagebutton.h"

#include "ktgctable.h"

class KTLibrary;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTLibraryWidget : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTLibraryWidget(const KTLibrary *library, QWidget *parent = 0);
		~KTLibraryWidget();
		void addBitmap(const QString &path);
		
	private:
		void setup();
		
	protected:
		virtual void libraryResponse(KTLibraryResponse *response);
		
	private slots:
		void addFolder(const QString &name);
		void previewItem(QTreeWidgetItem *, int);
		void emitSelectedComponent();
		void removeCurrentGraphic();
		void renameObject( QTreeWidgetItem* item);
		
		
	public slots:
		void importBitmap();
		
	signals:
		void requestCurrentGraphic();
	
	private:
		const KTLibrary *m_library;
		KTItemPreview *m_display;
		KTGCTable *m_libraryTree;
		int m_childCount;
		QDir m_libraryDir;
};

#endif
