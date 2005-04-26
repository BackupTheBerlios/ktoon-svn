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
 * @brief Include this file if you need the class Scenes
 */

#include <qdialog.h>
#include <qpushbutton.h>
#include <qpopupmenu.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtoolbutton.h>

class Ktoon;
class Scene;

/**
 * @brief Class that handles the dialog box for scene management
 *
 * <b>Date of Creation: June 1 - 2004.</b>\n
 * This is a class that contains a QListView that shows the available scenes and provides
 * mechanisms for adding, editing, and removing the scenes or their attributes.
 */
class Scenes : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a Scenes dialog box.
     * @param parent The parent widget
     * @param style Flags passed to the QDialog constructor
     * @param in_assigned_menu The assigned menu bar popup menu
     * @param id_assigned_item The assigned menu item from the assigned menu
     * @param assig_tb_button The assigned tool bar button
     */
    Scenes( QWidget *parent, WFlags style, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button );
    /**
     * @brief Default Destructor
     *
     * Destroys the Scenes dialog box.
     */
    ~Scenes();

    /**
     * @brief Loads the specified scenes
     *
     * @param scenes The list of scenes
     */
    void loadScenes( QPtrList<Scene> scenes );
    /**
     * @brief Selects the first scene
     */
    void selectFirstScene();
    /**
     * @brief Exports the timeline frames to a secuence of images of format \a format
     *
     * @param file_name The file name to export
     * @param format The image format
     * @return The number of images generated if the animation was exported successfully, otherwise returns 0
     */
    int exportAnimation( const QString &file_name, const QString &format );

private:
    //General Purpose Variables
    QWidget *parent_widget;
    QPopupMenu *assigned_menu;
    int assigned_item;
    QToolButton *assigned_tb_button;
    int scene_max_value;
    int number_of_scenes;
    QString current_name;
    Ktoon *k_toon;

    //Icons
    QPixmap i_insert_scene, i_remove_scene, i_move_scene_up, i_move_scene_down;

    //Buttons
    QPushButton *insert_scene, *remove_scene, *move_scene_up, *move_scene_down;

    //Table that shows the available scenes
    QListView *table_scenes;

    //Static Text
    QLabel *text_name;

    //Textfields
    QLineEdit *value_name;

signals:
    /**
     * This signal is emitted when a scene has been selected.
     * @param scene The scene position
     */
    void sceneSelected( int scene );
    /**
     * This signal is emitted when a scene has been inserted.
     */
    void sceneInserted();
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

public slots:
    /**
     * @brief Inserts a new scene
     *
     * @sa slotRemoveScene()
     */
    void slotInsertScene();
    /**
     * @brief Removes the selected scene
     *
     * @sa slotInsertScene()
     */
    void slotRemoveScene();
    /**
     * @brief Changes the selected scene name according to the textfield's text
     */
    void slotChangeValueName();
    /**
     * @brief Updates the visualization values when a scene is selected
     */
    void slotSelectScene();
    /**
     * @brief Moves the selected scene one step up
     */
    void slotMoveSceneUp();
    /**
     * @brief Moves the selected scene one step down
     */
    void slotMoveSceneDown();

protected:
    /**
     * @brief Event for dialog box closing control
     *
     * It updates some main window features as the assigned menu item from the assigned menu.
     * Reimplemented from QWidget.
     * @param close_event The input event
     */
    void closeEvent( QCloseEvent *close_event );

};

#endif
