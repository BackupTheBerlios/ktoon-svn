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

#ifndef TIMELINE_H
#define TIMELINE_H

/**
 * @file timeline.h
 * @brief Includethis file if you need the class Timeline
 */

#include <qdialog.h>
#include <qpopupmenu.h>
#include <qsplitter.h>
#include <qlabel.h>
#include <qtoolbutton.h>

#include "tllayermanager.h"
#include "soundwidget.h"
#include "ktdialogbase.h"

class Layer;
class KeyFrame;
class KToon;

/**
 * @brief Class that handles the timeline dialog box
 *
 * <b>Date of Creation: June 1 - 2004.</b>\n
 * This is a class that manages the timeline as it is in applications such as Flash. It has two main components:
 * The Layer Manager and the Frame Sequence Manager. See the documentation of each one for a further description.
 */
class Timeline : public KTDialogBase
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a Timeline dialog box.
     * @param parent The parent widget
     * @param style Flags passed to the QDialog constructor
     * @param in_assigned_menu The assigned menu bar popup menu
     * @param id_assigned_item The assigned menu item from the assigned menu
     * @param assig_tb_button The assigned tool bar button
     */
    Timeline( QWidget *parent, WFlags style, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button );
    /**
     * @brief Default Destructor
     *
     * Destroys the Timeline dialog box.
     */
    ~Timeline();

    /**
     * @brief Gets the frame sequence manager component
     *
     * @return A pointer to this timeline's frame sequence manager
     */
    TLFrameSequenceManager *frameSequenceManager();
    /**
     * @brief Gets the layer manager component
     *
     * @return A pointer to this timeline's layer manager
     */
    TLLayerManager *layerManager();
    /**
     * @brief Loads the specified layers and keyframes
     *
     * @param layers The list of layers. They also provide the keyframe information
     */
    void loadLayersAndKeyframes( QPtrList<Layer> layers );
    /**
     * @brief Loads the specified sound into a special widget
     *
     * @param file_name The sound file name
     */
    void loadSound( const QString &file_name );
    /**
     * @brief Exports the timeline frames to a secuence of images of format \a format
     *
     * @param file_name The file name to export
     * @param iter_begin Because each image from the sequence will keep the same name, it is added a number that indicates
     *        its order within the sequence. This argument is to know from which number the first file is going to start
     * @param format The image format
     * @return The number of the last image generated if the animation was exported successfully, otherwise returns 0
     */
    int exportAnimation( const QString &file_name, int iter_begin, const QString &format );

private:
    //General Purpose Variables
    QWidget *parent_widget;
    QPopupMenu *assigned_menu;
    int assigned_item;
    QToolButton *assigned_tb_button;
    KToon *k_toon;

    //Main Components
    QSplitter *splitter;
    TLLayerManager *layer_manager;
    TLFrameSequenceManager *frame_sequence_manager;
    QLabel *sound_label;
    QFrame *sound_widget_container;
    SoundWidget *sound_widget;

signals:
    /**
     * This signal is emitted when the insert layer button from the layer manager has been clicked.
     */
    void insertLayerClicked();
    /**
     * This signal is emitted when the remove layer button from the layer manager has been clicked.
     */
    void removeLayerClicked();
    /**
     * This signal is emitted when a layer has been moved up.
     */
    void layerMovedUp();
    /**
     * This signal is emitted when a layer has been moved down.
     */
    void layerMovedDown();

protected slots:
    /**
     * @brief Changes the sound label width after the layer manager's width has changed
     *
     * @param width The new width
     */
    void changeSoundLabelWidth( int width );
    /**
     * @brief Changes the sound widget container width after the frame sequence manager's width has changed
     *
     * @param width The new width
     */
    void changeSoundContainerWidth( int width );

public slots:
    /**
     * @brief Updates the current offset time according to the selected offset
     *
     * @param offset The current offset
     */
    void slotUpdateCurrentTime( int offset );
    /**
     * @brief Updates the total animation time according to the ruler maximum offset
     *
     * @param max_offset The ruler maximum offset
     */
    void slotUpdateTotalTime( int max_offset );

protected:
    /**
     * @brief Event for dialog box closing control
     *
     * It updates some main window features as the assigned menu item from the assigned menu.
     * Reimplemented from QWidget.
     * @param close_event The input event
     */
    void closeEvent( QCloseEvent *close_event );
    /**
     * @brief Event for dialog box resizing control
     *
     * Reimplemented from QWidget.
     * @param resize_event The input event
     */
    void resizeEvent( QResizeEvent *resize_event );
    /**
     * @brief Event for dialog box key pressing control
     *
     * Currently, it is implemented the F5, SHIFT+F5, CTRL+M and SHIFT+M key operations.
     * Reimplemented from QWidget.
     * @param key_event The input event
     */
    void keyPressEvent( QKeyEvent *key_event );

};

#endif
