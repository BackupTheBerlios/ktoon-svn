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

#ifndef KTOON_H
#define KTOON_H

/**
 * @author Fernando Roldan
 * @file ktoon.h
 *  Include this file if you need the class KToon
 */

#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qworkspace.h>
#include <qcursor.h>
#include <qpixmap.h>
#include <qaction.h>

#include "ktapplication.h"

#include "tools.h"
// #include "drawingarea.h"
#include "properties.h"
#include "preferences.h"
#include "scenes.h"
#include "brushes.h"
#include "colorpalette.h"
#include "exposuresheet.h"
#include "library.h"
#include "timeline.h"
// #include "glrendercamerapreview.h"
#include "gltopcameraview.h"
#include "glsidecameraview.h"
#include "status.h"
#include "about.h"
#include "document.h"
#include "import.h"
#include "export.h"
// #include "drawingareaqt.h"
// #include "rendercamerapreviewqt.h"
#include "ktdialogmenu.h"

#include "ktcamerapreview.h"


/**
 *  Class that handles the main window with all its elements
 *
 * <b>Date of Creation: May 13 - 2004.</b>\n
 * The most important class. It manages all the dialog boxes within the illustration and animation modes, and it
 * provides a menu bar and a tool bar for special operations.
 */
class KToon: public QMainWindow
{
	Q_OBJECT

    	public:
		/**
		*  Default Constructor
		*
		* Constructs a KToon object.
		*/
		KToon();
		/**
		*  Default Destructor
		*
		* Destroys the ValueSelector object.
		*/
		~KToon();

		/**
		*  Gets the current timeline dialog
		*
		* @return A pointer to the current timeline dialog
		*/
		Timeline *timeline();
	
		/**
		*  Gets the current exposure sheet dialog
		*
		* @return A pointer to the current exposure sheet dialog
		*/
		ExposureSheet *exposureSheet();

		/**
		*  Gets the render camera preview dialog
		*
		* @return A pointer to the render camera preview dialog
		*/
		GLRenderCameraPreview *renderCameraPreview();
		
		/**
		*  Loads the specified image into the drawing area
		*
		* @param file_name The image file name
		* @param from_load A boolean value that indicates if this function was called from a load operation
		*/
		void loadImage( const QString &file_name, bool from_load = false );
		/**
		*  Loads the specified image sequence into the drawing area
		*
		* @param file_name The image sequence file name
		* @param from_load A boolean value that indicates if this function was called from a load operation
		*/
		void loadImageSequence( const QString &file_name, bool from_load = false );
		/**
		*  Loads the specified graphic library
		*
		* @param file_name The library file name
		* @param from_load A boolean value that indicates if this function was called from a load operation
		*/
		void loadLibrary( const QString &file_name, bool from_load = false );
		/**
		*  Loads the specified color palette
		*
		* @param file_name The color palette file name
		* @param from_load A boolean value that indicates if this function was called from a load operation
		*/
		void loadPalette( const QString &file_name, bool from_load = false );
		/**
		*  Loads the specified brushes
		*
		* @param file_name The brushes file name
		* @param from_load A boolean value that indicates if this function was called from a load operation
		*/
		void loadBrushes( const QString &file_name, bool from_load = false );
		/**
		*  Loads the specified sound into a timeline's special widget
		*
		* @param file_name The sound file name
		* @param from_load A boolean value that indicates if this function was called from a load operation
		*/
		void loadSound( const QString &file_name, bool from_load = false );
		/**
		*  Creates a graphic component from an XML tag
		*
		* @param graphic_tag The XML tag
		* @return A new graphic component
		*/
		GLGraphicComponent *createGraphic( const QDomElement &graphic_tag );
		/**
		*  Creates a folder object from an XML tag
		*
		* @param tag The XML tag
		* @return A new folder
		*/
		Folder *createFolder( const QDomElement &tag );
		
		//----- Public Members for exposure sheet and timeline -----
		
		/**
		*  Gets the exposure sheet's default color
		*
		* @return The exposure sheet's default color
		*/
		QColor *esDefaultColor();
		/**
		*  Gets the exposure sheet's use color
		*
		* @return The exposure sheet's use color
		*/
		QColor *esUseColor();
		/**
		*  Gets the exposure sheet's selection color
		*
		* @return The exposure sheet's selection color
		*/
		QColor *esSelectionColor();
		/**
		*  Gets the exposure sheet's select and use color
		*
		* @return The exposure sheet's select and use color
		*/
		QColor *esSelectAndUseColor();
		/**
		*  Gets the exposure sheet's text color when selected
		*
		* @return The exposure sheet's text color when selected
		*/
		QColor *esTextColorWhenSelected();
		/**
		*  Gets the exposure sheet's default text color
		*
		* @return The exposure sheet's default text color
		*/
		QColor *esDefaultTextColor();
		/**
		*  Gets the exposure sheet's lock color
		*
		* @return The exposure sheet's lock color
		*/
		QColor *esLockColor();
		/**
		*  Gets the exposure sheet's lock and select color
		*
		* @return The exposure sheet's lock and select color
		*/
		QColor *esLockAndSelectColor();
		/**
		*  Gets the exposure sheet's drawing color
		*
		* @return The exposure sheet's drawing color
		*/
		QColor *esHasDrawingColor();
		/**
		*  Gets the timeline's default color
		*
		* @return The timeline's default color
		*/
		QColor *tlDefaultColor();
		/**
		*  Gets the timeline's selection color
		*
		* @return The timeline's selection color
		*/
		QColor *tlSelectionColor();
		/**
		*  Gets the timeline's special color
		*
		* @return The timeline's special color
		*/
		QColor *tlSpecialColor();
		/**
		*  Gets the timeline's offset color
		*
		* @return The timeline's offset color
		*/
		QColor *tlOffsetColor();
		/**
		*  Gets the timeline's drag offset color
		*
		* @return The timeline's drag offset color
		*/
		QColor *tlDragOffsetColor();
		/**
		*  Gets the timeline's border color
		*
		* @return The timeline's border color
		*/
		QColor *tlBorderColor();
		/**
		*  Gets the timeline's use border color
		*
		* @return The timeline's use border color
		*/
		QColor *tlUseBorderColor();
		/**
		*  Gets the timeline's motion color
		*
		* @return The timeline's motion color
		*/
		QColor *tlMotionColor();
		/**
		*  Gets the timeline's drawing color
		*
		* @return The timeline's drawing color
		*/
		QColor *tlDrawingColor();

    	private:
		/**
		 * Setup the applications colors.
		 */
		void setupColors();
		/**
		 * Setup icons
		 */
		void setupIcons();
	
		/**
		 * Setup cursors
		 */
		void setupCursors();
	
		/**
		* Setup the actions in the toolbar
		*/
		void setupToolBarActions();
		
		/**
		 * Setup menu
		 */
		void setupMenu();
		
		/**
		 * Setup dialogs
		 */
		void setupDialogs();
	
    	protected:
	    /**
	     *  Event for main window closing control
	     *
	     * Reimplemented from QWidget.
	     * @param close_event The input event
	     */
	    void closeEvent( QCloseEvent *close_event );
	    /**
	     *  Creates the application GUI according to the information of the data classes
	     */
	    void createGUI();
	    /**
	     *  Updates the open recent menu item names according to the @a recent_names list of file names
	     */
	    void updateOpenRecentMenu();

	public slots:

	    //------------- File Menu ------------------
	
	    /**
	     *  Opens a new document
	     */
	    void slotNewDocument();
	    /**
	     *  Shows a dialog box in order to select the path of the file to open and calls the load function
	     */
	    void slotChoose();
	    /**
	     *  Loads the document of name \a in_file_name
	     *
	     * @param in_file_name The name of the file to load
	     */
	    void slotLoadDocument( const QString &in_file_name );
	    /**
	     *  Opens a recent document
	     *
	     * @param mi The menu item identifier
	     */
	    void slotOpenRecent( int mi );
	    /**
	     *  Saves the active document into the current location
	     */
	    void slotSave();
	    /**
	     *  Shows a dialog box in order to save the active document with a new name and location
	     */
	    void slotSaveAs();
	    /**
	     *  Opens a dialog box to import from files of compatible types
	     */
	    void slotImport();
	    /**
	     *  Opens a dialog box to export to files of compatible types
	     */
	    void slotExport();
	    /**
	     *  Opens a dialog box in order to view or modify the properties of the current document
	     */
	    void slotProperties();
	
	    //-------------- Edit Menu -----------------
	
	    /**
	     *  Undoes the last action
	     */
	    void slotUndo();
	    /**
	     *  Redoes an action (after executing an "undo" action)
	     */
	    void slotRedo();
	    /**
	     *  Cuts the selection and puts it onto the clipboard
	     */
	    void slotCut();
	    /**
	     *  Copies the selection and puts it onto the clipboard
	     */
	    void slotCopy();
	    /**
	     *  Pastes the clipboard into the drawing area
	     */
	    void slotPaste();
	    /**
	     *  Pastes the clipboard into the drawing area, in the location where the copied object was
	     */
	    void slotPasteInPlace();
	    /**
	     *  Deletes the selected objects
	     */
	    void slotDelete();
	    /**
	     *  Selects every object in the drawing area
	     */
	    void slotSelectAll();
	    /**
	     *  Opens a dialog box where it is possible to set options for the application
	     */
	    void slotPreferences();
	
	    //----------------- View Menu ----------------
	
	    /**
	     *  Performs a "Zoom In" on the drawing area
	     */
	    void slotZoomIn();
	    /**
	     *  Performs a "Zoom Out" on the drawing area
	     */
	    void slotZoomOut();
	    /**
	     *  Fits the drawing area into the application in a way that it is possible to view the whole of it
	     */
	    void slotFit();
	    /**
	     *  Shows a 12 Field Grid
	     */
	    void slotSeeGrid12();
	    /**
	     *  Shows a 16 Field Grid
	     */
	    void slotSeeGrid16();
	    /**
	     *  Shows a Grid with a 3 Field subgrid
	     */
	    void slotSeeSubgrid();
	    /**
	     *  Shows the drawing area without a grid
	     */
	    void slotNoGrid();
	    /**
	     *  Disable previous Onion Skins
	     */
	    void slotNoPreviousOnionSkin();
	    /**
	     *  See only the previous Onion Skin
	     */
	    void slotPreviousOnionSkin();
	    /**
	     *  See the previous two Onion Skins
	     */
	    void slotPrevious2OnionSkin();
	    /**
	     *  See the previous three Onion Skins
	     */
	    void slotPrevious3OnionSkin();
	    /**
	     *  Disable next Onion Skins
	     */
	    void slotNoNextOnionSkin();
	    /**
	     *  See only the next Onion Skin
	     */
	    void slotNextOnionSkin();
	    /**
	     *  See the next two Onion Skins
	     */
	    void slotNext2OnionSkin();
	    /**
	     *  See the next three Onion Skins
	     */
	    void slotNext3OnionSkin();
	    /**
	     *  Rotates the scene 30 degrees clockwise
	     */
	    void slotRotateClockwise();
	    /**
	     *  Rotates the scene 30 degrees counterclockwise
	     */
	    void slotRotateCounterClockwise();
	    /**
	     *  Restores any previous rotations performed to the scene
	     */
	    void slotRestoreRotation();
	     /**
	     *  Shows or hides the guidelines for NTSC zone
	     */
	    void slotSeeNTSC();
	   /**
	     *  Sends the grid to the front or to the back of the drawing area
	     */
	    void slotFrontBackGrid();
	    /**
	     *  Activates or deactivates the light table
	     */
	    void slotLightTable();
	
	    //--------------- Insert Menu ------------------
	
	    /**
	     *  Inserts a Layer into the exposure sheet and timeline
	     */
	    void slotInsertLayer();
	    /**
	     *  Inserts a Frame into the timeline
	     */
	    void slotInsertFrame();
	    /**
	     *  Removes a Frame from the timeline
	     */
	    void slotRemoveFrame();
	    /**
	     *  Inserts a KeyFrame into the exposure sheet and timeline
	     */
	    void slotInsertKeyFrame();
	
	    //---------------- Tools Menu ------------------
	
	    void slotSelectTool(int toolId);
		      
	    /**
	     *  Makes a group from the selected objects of the drawing area
	     */
	    void slotGroup();
	    
	    /**
	     *  Ungroups the previous grouped elements
	     */
	    void slotUngroup();
	    /**
	     *  Puts the selected object over all the objects in the drawing area
	     */
	    void slotBringToFront();
	    /**
	     *  Sends the selected object to the drawing area's background
	     */
	    void slotSendToBack();
	    /**
	     *  Moves the selected object one step forward into the drawing area
	     */
	    void slotOneStepForward();
	    /**
	     *  Moves the selected object one step backward into the drawing area
	     */
	    void slotOneStepBackward();
	    /**
	     *  Aligns the selection to the drawing area's left border
	     */
	    void slotAlignLeft();
	    /**
	     *  Centers the selection vertically into the drawing area
	     */
	    void slotCenterVertically();
	    /**
	     *  Aligns the selection to the drawing area's right border
	     */
	    void slotAlignRight();
	    /**
	     *  Aligns the selection to the drawing area's top border
	     */
	    void slotAlignTop();
	    /**
	     *  Centers the selection horizontally into the drawing area
	     */
	    void slotCenterHorizontally();
	    /**
	     *  Aligns the selection to the drawing area's bottom border
	     */
	    void slotAlignBottom();
	    /**
	     *  Reflects the selection in the drawing area - horizontal axis
	     */
	    void slotFlipHorizontally();
	    /**
	     *  Reflects the selection in the drawing area - vertical axis
	     */
	    void slotFlipVertically();
	    /**
	     *  Rotates the selected elements 90 degrees to the right
	     */
	    void slotRotateCW90();
	    /**
	     *  Rotates the selected elements 90 degrees to the left
	     */
	    void slotRotateCCW90();
	    /**
	     *  Rotates the selected elements 180 degrees
	     */
	    void slotRotate180();
	    /**
	     *  Activates the Selection Tool
	     */
	    void slotNormalSelection();
	    /**
	     *  Activates the Node Selection Tool
	     */
	    void slotContourSelection();
	    /**
	     *  Activates a special Tool for transformations
	     */
	    void slotPerspectiveSelection();
	    /**
	     *  Activates the Brush Tool
	     */
	    void slotBrush();
	    /**
	     *  Activates the Pencil Tool
	     */
	    void slotPencil();
	    /**
	     *  Activates the Pen Tool
	     */
	    void slotPen();
	    /**
	     *  Activates the Line Tool
	     */
	    void slotLine();
	    /**
	     *  Activates the Rectangle Tool
	     */
	    void slotRectangle();
	    /**
	     *  Activates the Ellipse Tool
	     */
	    void slotEllipse();
	    /**
	     *  Activates the Contour Fill Tool
	     */
	    void slotContourFill();
	    /**
	     *   Activates the Fill Tool
	     */
	    void slotFill();
	    /**
	     *  Activates the Remove Fill Tool
	     */
	    void slotRemoveFill();
	    /**
	     *  Activates the Dropper Tool
	     */
	    void slotDropper();
	    /**
	     *  Activates the Eraser Tool
	     */
	    void slotEraser();
	    /**
	     *  Activates the Slicer Tool
	     */
	    void slotSlicer();
	    /**
	     *  Activates the Magnifying Glass Tool
	     */
	    void slotMagnifyingGlass();
	    /**
	     *  Activates the Hand Tool
	     */
	    void slotHand();
	
	    //------------------ Control Menu -----------------
	
	    /**
	     *  Plays the animation from the current animation offset
	     */
	    void slotPlayStop();
	    /**
	     *  Sets the animation offset to be the first
	     */
	    void slotRewind();
	    /**
	     *  Sets the animation offset to be the last
	     */
	    void slotGoToEnd();
	    /**
	     *  Steps one frame forward in the animation offset
	     */
	    void slotStepForward();
	    /**
	     *  Steps one frame backward in the animation offset
	     */
	    void slotStepBackward();
	
	    //------------------ Window Menu ------------------
	
	    /**
	     *  Turns the GUI to the illustration mode
	     */
	    void slotSeeIllustration();
	    /**
	     *  Turns the GUI to the animation mode
	     */
	    void slotSeeAnimation();
	    /**
	     *  Shows or hides the "Tools" dialog box
	     */
	    void slotWindowTools();
	    /**
	     *  Shows or hides the "Brushes" dialog box
	     */
	    void slotWindowBrushes();
	    /**
	     *  Shows or hides the "Scenes" dialog box
	     */
	    void slotWindowScenes();
	    /**
	     *  Shows or hides the "ColorPalette" dialog box
	     */
	    void slotWindowColorPalette();
	    /**
	     *  Shows or hides the "ExposureSheet" dialog box
	     */
	    void slotWindowExposureSheet();
	    /**
	     *  Shows or hides the "Library" dialog box
	     */
	    void slotWindowLibrary();
	    /**
	     *  Shows or hides the "DrawingArea" widget
	     */
	    void slotWindowDrawingArea();
	    /**
	     *  Shows or hides the "Timeline" dialog box
	     */
	    void slotWindowTimeline();
	    /**
	     *  Shows or hides the "Render Preview" dialog box
	     */
	    void slotWindowRenderCameraPreview();
	    /**
	     *  Shows or hides the "Top Camera View" dialog box
	     */
	    void slotWindowTopCameraView();
	    /**
	     *  Shows or hides the "Side Camera View" dialog box
	     */
	    void slotWindowSideCameraView();
	
	    //----------------- Help Menu -------------------
	
	    /**
	     *  Executes the K-Toon's help file
	     */
	    void slotContents();
	    /**
	     *  Shows the dialog with information about the autors and the application
	     */
	    void slotAbout();
	    /**
	     *  Shows the dialog with information about OpenGL
	     */
	    void slotAboutOpenGL();
	
	    //---------------- General Slots ------------------
	
	    /**
	     *  Performs a drawing area close operation
	     */
	    void slotCloseDrawingArea();
	    /**
	     *  Show a status bar message after a menu item highlight
	     *
	     * @param mi The highlighted menu item identifier
	     */
	    void slotStatusBarMessage( int mi );
	    /**
	     *  Sets the drawing area's cursor to be the current cursor
	     */
	    void slotActivateCursor();
	
	    //-------- Slots for scene synchronization ---------
	
	    /**
	     *  Slot for synchronization of the select scene operation
	     */
	    void slotSelectSync( int sp );
	    /**
	     *  Slot for synchronization of the insert scene operation
	     */
	    void slotInsertSync();
	    /**
	     *  Slot for synchronization of the remove scene operation
	     */
	    void slotRemoveSync( int sp );
	    /**
	     *  Slot for synchronization of the move scene up operation
	     */
	    void slotMoveUpSync( int sp );
	    /**
	     *  Slot for synchronization of the move scene down operation
	     */
	    void slotMoveDownSync( int sp );

	private:
	    //General purpose variables
	    QString file_name;
	    int document_max_value;
	    QPtrList<ExposureSheet> list_of_es;
	    QPtrList<Timeline> list_of_tl;
	
	    //Icons
	    QPixmap icon_new, icon_open, icon_save, icon_import, icon_export, icon_undo, icon_redo, icon_zoom_in,
		    icon_zoom_out, icon_fit, icon_grid12, icon_grid16, icon_subgrid, icon_no_grid, icon_close,
		    icon_cut, icon_copy, icon_paste, icon_perspective;
	
	    QPixmap icon_ani_mode, icon_bring_to_front, icon_brushes, icon_camera_preview, icon_color_palette,
		    icon_drawing_area, icon_exposure_sheet, icon_group, icon_illus_mode, icon_library, icon_light_table,
		    icon_front_back_grid, icon_ntsc, icon_one_backward, icon_one_forward, icon_properties, icon_scenes,
		    icon_send_to_back, icon_side_camera, icon_time_line, icon_top_camera, icon_tools, icon_ungroup,
		    icon_no_previous, icon_previous, icon_previous2, icon_previous3,
		    icon_no_next, icon_next, icon_next2, icon_next3, icon_align_l, icon_align_r, icon_align_t, icon_align_b,
		    icon_align_cv, icon_align_ch;
	
	    QPixmap icon_selection, icon_nodes, icon_brush, icon_pencil, icon_line, icon_pen, icon_rectangle,
		    icon_ellipse, icon_eraser, icon_slicer, icon_fill, icon_contour_fill, icon_remove_fill,
		    icon_dropper, icon_magnifying_glass, icon_hand;
	
	    //Cursors
	    QPixmap i_cursor_selection, i_cursor_nodes, i_cursor_brush, i_cursor_pencil, i_cursor_pen, i_cursor_rectangle,
		    i_cursor_ellipse, i_cursor_eraser, i_cursor_slicer, i_cursor_fill, i_cursor_remove_fill,
		    i_cursor_magnifying_glass, i_cursor_hand, i_cursor_dropper, i_cursor_contour_fill;
	
	    QCursor *cursor_line, *cursor_selection, *cursor_nodes, *cursor_brush, *cursor_pencil, *cursor_pen,
		    *cursor_rectangle, *cursor_ellipse, *cursor_eraser, *cursor_slicer, *cursor_fill,
		    *cursor_remove_fill, *cursor_magnifying_glass, *cursor_hand, *cursor_dropper, *cursor_contour_fill;
	
	    //Menus and identifiers for Menu Items
	    QPopupMenu *file, *open_recent;
	    int id_file;
	    int id_file_new;
	    int id_file_open;
	    QStringList recent_names;  //Contains the list of file mames from the most recent used files
	    int id_file_save;
	    int id_file_save_as;
	    int id_file_close;
	    int id_file_properties;
	    int id_file_import;
	    int id_file_export;
	    int id_file_exit;
	
	    QPopupMenu *edit;
	    int id_edit;
	    int id_edit_undo;
	    int id_edit_redo;
	    int id_edit_cut;
	    int id_edit_copy;
	    int id_edit_paste;
	    int id_edit_paste_in_place;
	    int id_edit_delete;
	    int id_edit_select_all;
	    int id_edit_preferences;
	
	    QPopupMenu *view, *grid, *onion_skin;
	    int id_view;
	    int id_view_zoom_in;
	    int id_view_zoom_out;
	    int id_view_fit;
	    int id_view_grid12;
	    int id_view_grid16;
	    int id_view_subgrid;
	    int id_view_no_grid;
	    int id_view_NTSC_zone;
	    int id_view_front_back_grid;
	    int id_view_light_table;
	    int id_view_previous_none;
	    int id_view_previous;
	    int id_view_previous2;
	    int id_view_previous3;
	    int previous_checked; //Variable that indicates which of the above 4 menu items is checked
	    int id_view_next_none;
	    int id_view_next;
	    int id_view_next2;
	    int id_view_next3;
	    int next_checked; //Variable that indicates which of the above 4 menu items is checked
	    int id_view_rotate_clockwise;
	    int id_view_rotate_counterclockwise;
	    int id_view_restore_rotation;
	
	    QPopupMenu *insert;
	    int id_insert;
	    int id_insert_layer;
	    int id_insert_frame;
	    int id_insert_remove_frame;
	    int id_insert_keyframe;
	    int id_insert_scene;
	    int id_insert_remove_scene;
	
	    QPopupMenu *tools;
	    int id_tools;
	    int id_tools_normal_selection;
	    int id_tools_contour_selection;
	    int id_tools_brush;
	    int id_tools_pencil;
	    int id_tools_pen;
	    int id_tools_line;
	    int id_tools_rectangle;
	    int id_tools_ellipse;
	    int id_tools_fill;
	    int id_tools_remove_fill;
	    int id_tools_contour_fill;
	    int id_tools_dropper;
	    int id_tools_eraser;
	    int id_tools_slicer;
	    int id_tools_magnifying_glass;
	    int id_tools_hand;
	    int id_tools_group;
	    int id_tools_ungroup;
	    int id_tools_bring_front;
	    int id_tools_send_back;
	    int id_tools_one_step_forward;
	    int id_tools_one_step_backward;
	    int id_tools_left;
	    int id_tools_center_vertically;
	    int id_tools_right;
	    int id_tools_top;
	    int id_tools_center_horizontally;
	    int id_tools_bottom;
	    int id_tools_flip_horizontally;
	    int id_tools_flip_vertically;
	    int id_tools_rotate_cw90;
	    int id_tools_rotate_ccw90;
	    int id_tools_rotate180;
	    int id_tools_perspective;
	
	    QPopupMenu *control;
	    int id_control;
	    int id_control_play;
	    int id_control_rewind;
	    int id_control_go_to_end;
	    int id_control_step_forward;
	    int id_control_step_backward;
	
	    QPopupMenu *window;
	    int id_window;
// 	    int id_window_illustration;
	    int id_window_animation;
	    int id_window_tools;
	    int id_window_scenes;
	    int id_window_color_palette;
	    int id_window_exposure_sheet;
	    int id_window_library;
	    int id_window_drawing_area;
	    int id_window_timeline;
	    int id_window_render_camera_preview;
	    int id_window_top_camera_view;
	    int id_window_side_camera_view;
	
	    QPopupMenu *help;
	    int id_help;
	    int id_help_contents;
	    int id_help_about;
	    int id_help_about_qt;
	    int id_help_about_opengl;
	
	    //Tool Bar and Tool Bar buttons
	    QToolBar *tool_bar, *tool_bar2;
	    QToolButton *file_new;
	    QToolButton *file_open;
	    QToolButton *file_save;
	    QToolButton *edit_undo;
	    QToolButton *edit_redo;
	    QToolButton *edit_cut;
	    QToolButton *edit_copy;
	    QToolButton *edit_paste;
	    QToolButton *view_no_grid;
	    QToolButton *view_grid12;
	    QToolButton *view_grid16;
	    QToolButton *view_subgrid;
	    QToolButton *view_no_previous;
	    QToolButton *view_previous;
	    QToolButton *view_previous2;
	    QToolButton *view_previous3;
	    QToolButton *previous_toggled;
	    QToolButton *view_no_next;
	    QToolButton *view_next;
	    QToolButton *view_next2;
	    QToolButton *view_next3;
	    QToolButton *next_toggled;
	    QToolButton *view_front_back_grid;
	    QToolButton *view_ntsc;
	    QToolButton *view_light_table;
	    
	    QAction *window_illustration;
	    QToolButton *window_animation;
	    QToolButton *window_tools;
	    
	    //QToolButton *window_brushes;
	    QAction *window_brushes;
	    
	    QToolButton *window_scenes;
	    QToolButton *window_color_palette;
	    QToolButton *window_exposure_sheet;
	    QToolButton *window_library;
	    QToolButton *window_drawing_area;
	    QToolButton *window_timeline;
	    QToolButton *window_render_camera_preview;
	    QToolButton *window_top_camera_view;
	    QToolButton *window_side_camera_view;
	
	    //Main Panel that manages the documents
	    QWorkspace *main_panel;
	
	    //Non Modal Dialog Boxes for environment interaction
	    Tools *tools_dialog;
	    Brushes *brushes_dialog;
	    Scenes *scenes_dialog;
	    ColorPalette *color_palette_dialog;
	    ExposureSheet *exposure_sheet_dialog;
	    Library *library_dialog;
	    Timeline *timeline_dialog;
// 	    GLRenderCameraPreview *render_camera_preview;
	    KTCameraPreview *m_cameraPreview;
	    GLSideCameraView *side_camera_view;
	    GLTopCameraView *top_camera_view;
	    About *about_dialog;
	
	    //Modal Dialog Boxes for several settings
	    Properties *properties_dialog;
	    Preferences *preferences_dialog;
	    Import *import_dialog;
	    Export *export_dialog;

// 	    RenderCameraPreviewQt *render_camera_preview_qt;
	    
	    //Some colors needed by Exposure Sheet and Timeline (They are created here for efficiency)
	    QColor *es_default_color, *es_use_color, *es_selection_color, *es_select_and_use_color, 
		   *es_text_color_when_selected, *es_default_text_color, *es_lock_color, *es_lock_and_select_color, 
		   *es_has_drawing_color;
	    QColor *tl_default_color, *tl_special_color, *tl_selection_color, *tl_offset_color, *tl_drag_offset_color, 
		   *tl_border_color, *tl_use_border_color, *tl_motion_color, *tl_drawing_color;
};

#endif
