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

#ifndef KTSCENESWIDGET_H
#define KTSCENESWIDGET_H

#include "ktmodulewidgetbase.h"

#include "ktsceneslist.h"

#include <QGroupBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QList>

class KTScenesWidget : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTScenesWidget( QWidget *parent);
		~KTScenesWidget();

	private:
		QGroupBox *m_buttonsPanel;
		QButtonGroup *m_buttonGroup;
		KTScenesList *m_tableScenes;

		QLabel *m_textName;

		QLineEdit *m_valueName;

	private:
		void setupButtons();
		void setupTableScenes();
		
	private slots:
		void actionButton(QAbstractButton *);
		void sceneDobleClick(QTreeWidgetItem * item, int col );
		
	signals:
		void requestInsertScene();
		void requestRemoveScene();
		void requestMoveUpScene();
		void requestMoveDownScene();
		
		void changeCurrentScene(int id);
		void renameScene(const QString &name);
		
	public slots:
		void insertScene(const QString &name);
		void removeScene();
};

#endif
