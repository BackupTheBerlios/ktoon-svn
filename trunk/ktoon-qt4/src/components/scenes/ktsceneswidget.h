/***************************************************************************
 *   Copyright (C) 2004 by Fernando Jose Roldan Correa                     *
 *   froldan@toonka.com                                                    *
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

#ifndef SCENES_H
#define SCENES_H

/**
 * @file scenes.h
 *   Include this file if you need the class KTScenesWidget
 */

#include "ktmodulewidgetbase.h"

#include "ktsceneslist.h"

#include <QGroupBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QList>


class Scene;

/**
 *   Class that handles the dialog box for scene management
 *
 * <b>Date of Creation: June 1 - 2004.</b>\n
 * This is a class that contains a QListView that shows the available scenes and provides
 * mechanisms for adding, editing, and removing the scenes or their attributes.
 */

class KTScenesWidget : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		enum Actions{InsertScene = 0 , RemoveScene, MoveSceneUp , MoveSceneDown};
		/**
		* Default Constructor
		*
		* Constructs a KTScenesWidget dialog box.
		* @param parent The parent widget
		*
		*/
		KTScenesWidget( QWidget *parent);
		/**
		*   Default Destructor
		*
		* Destroys the KTScenesWidget dialog box.
		*/
		~KTScenesWidget();

	private:
    //General Purpose Variables

		int scene_max_value;
		int number_of_scenes;
		QString current_name;

		QList<QPixmap> m_imgs;
		QGroupBox *m_buttonsPanel;
		QButtonGroup *m_buttonGroup;
// 		Q3VBox *containerTableScens;
    //Table that shows the available scenes
// 		Q3ListView *table_scenes;
		KTScenesList *m_tableScenes;
    //Static Text
		QLabel *m_textName;

    //Textfields
		QLineEdit *m_valueName;

	private:
		void setupButtons();
		void setupTableScenes();
		
	private slots:
		void actionButton(QAbstractButton *);
		void sceneDobleClick(QTreeWidgetItem * item, int col );
		
	signals:
		/**
		* This signal is emitted when a scene has been selected.
		* @param scene The scene position
		*/
		void sceneSelected( int scene );
		/**
		* This signal is emitted when a scene has been inserted.
		*/
		void sceneInserted(const QString &name, int index);
		/**
		* This signal is emitted when a scene has been removed.
		* @param scene The scene position
		*/
		void sceneRemoved( int scene );
		/**
		* This signal is emitted when a scene has been moved up.
		* @param scene The scene position	
		*/
		void sceneMovedUp( int scene );
		/**
		* This signal is emitted when a scene has been moved down.
		* @param scene The scene position
		*/
		void sceneMovedDown( int scene );
		void sceneRenamed( const QString &newName, int index);
		void esceneDobleClicked(int scenes);
// 		void changedScene();
		
	public slots:
		/**
		*   Inserts a new scene
		*
		* @sa removeScene()
		*/
		void insertScene();
		/**
		*   Removes the selected scene
		*
		* @sa insertScene()
		*/
		void removeScene();
		/**
		*   Changes the selected scene name according to the textfield's text
		*/
		void changeValueName();
		/**
		*   Updates the visualization values when a scene is selected
		*/
		void selectScene(const QString & name, int index);
		/**
		*   Moves the selected scene one step up
		*/
		void moveSceneUp();
		/**
		*   Moves the selected scene one step down
		*/
		void moveSceneDown();
		
		void applyAction(int action);

		

};

#endif
