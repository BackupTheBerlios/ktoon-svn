/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#ifndef KTEXPORTWIDGET_H
#define KTEXPORTWIDGET_H

#include <ktmodulewidgetbase.h>
#include "ktprojectmanager.h"

#include <QListWidget>
#include <QHash>

#include "exportinterface.h"


class QButtonGroup;
class QLineEdit;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTExportWidget : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTExportWidget(const KTProjectManager *manager, QWidget *parent = 0);
		~KTExportWidget();
		void addFormats(ExportInterface::Formats formats);
		
		QString fileToExport() const;
		
	private:
		enum ButtonId
		{
			AllScenes = 0,
			CurrentScene,
			SceneRange
		};
		
	public slots:
		void exportIt();
		
	private:
		void setupToExport(QBoxLayout *mainLayout);
		void setupExportBox(QBoxLayout *mainLayout);
		
	private slots:
		void makeAction(int buttonId);
		void selectExporter(QListWidgetItem *item);
		void selectFormat(QListWidgetItem *item);
		
		void chooseFile();
		
	private:
		void loadPlugins();
		QList<KTScene *> scenesToExport();
		
	private:
		QButtonGroup *m_buttons;
		
		const KTProjectManager *m_manager;
		
		QListWidget *m_exporterList;
		QListWidget *m_formatList;
		
		QHash<QListWidgetItem *, ExportInterface *> m_plugins;
		QHash<QListWidgetItem *, ExportInterface::Format > m_formats;
		
		ExportInterface *m_currentExporter;
		ExportInterface::Format m_currentFormat;
		
		QLineEdit *m_fromScene, *m_toScene;
		QLineEdit *m_filePath;
};

#endif
