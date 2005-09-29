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

#include <q3mainwindow.h>
#include <q3toolbar.h>
#include <q3popupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <QWorkspace>
#include <qcursor.h>
#include <qpixmap.h>
#include <qaction.h>

#include <Q3ActionGroup>
#include <QCloseEvent>
#include <Q3PtrList>

#include "ktapplication.h"

#include "tools.h"
#include "properties.h"
#include "preferences.h"
#include "scenes.h"
#include "brushes.h"
#include "colorpalette.h"

#include "ktexposuresheet.h"

#include "library.h"
#include "kttimeline.h"

#include "gltopcameraview.h"
#include "glsidecameraview.h"
#include "status.h"
#include "about.h"
#include "document.h"
#include "import.h"
#include "export.h"

#include "ktcamerapreview.h"
#include "ktviewdocument.h"

/**
 *  Class that handles the main window with all its elements
 *
 * <b>Date of Creation: May 13 - 2004.</b>\n
 * The most important class. It manages all the dialog boxes within the illustration and animation modes, and it
 * provides a menu bar and a tool bar for special operations.
 */
class KToon : public Q3MainWindow
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
		
		void setPalette(const QPalette &);
	
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
		* Create the action
		*/
		void createActions();
		
		/**
		* Setup the actions in the toolbar
		*/
		void setupToolBar();
		
		/**
		 * Setup he actions in the menu
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
		int document_max_value;
		QString file_name;
// 		QPtrList<ExposureSheet> list_of_es;
		
		Q3PtrList<KTTimeLine> m_timeLineList;
		
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
		Q3PopupMenu *file, *open_recent;
 		int id_file;
		QStringList recent_names;  //Contains the list of file mames from the most recent used files
		Q3Action *file_save_as;
		Q3Action *file_close;
		Q3Action *file_properties;
		Q3Action *file_import;
		Q3Action *file_export;
		Q3Action *file_exit;
		
		Q3PopupMenu *edit;
		int id_edit;
		Q3Action *edit_paste_in_place;
		Q3Action *edit_delete;
		Q3Action *edit_select_all;
		Q3Action *edit_preferences;
		
		Q3PopupMenu *view, *grid, *onion_skin;
		int id_view;
		Q3Action *view_zoom_in;
		Q3Action *view_zoom_out;
		Q3Action *view_fit;
		Q3Action *view_rotate_clockwise;
		Q3Action *view_rotate_counterclockwise;
		Q3Action *view_restore_rotation;
		
		Q3PopupMenu *insert;
		int id_insert;
		Q3Action *insert_layer;
		Q3Action *insert_frame;
		Q3Action *insert_remove_frame;
		Q3Action *insert_keyframe;
		Q3Action *insert_scene;
		Q3Action *insert_remove_scene;
		
		Q3PopupMenu *tools;
		int id_tools;
		Q3Action *tools_normal_selection;
		Q3Action *tools_contour_selection;
		Q3Action *tools_brush;
		Q3Action *tools_pencil;
		Q3Action *tools_pen;
		Q3Action *tools_line;
		Q3Action *tools_rectangle;
		Q3Action *tools_ellipse;
		Q3Action *tools_fill;
		Q3Action *tools_remove_fill;
		Q3Action *tools_contour_fill;
		Q3Action *tools_dropper;
		Q3Action *tools_eraser;
		Q3Action *tools_slicer;
		Q3Action *tools_magnifying_glass;
		Q3Action *tools_hand;
		Q3Action *tools_group;
		Q3Action *tools_ungroup;
		Q3Action *tools_bring_front;
		Q3Action *tools_send_back;
		Q3Action *tools_one_step_forward;
		Q3Action *tools_one_step_backward;
		Q3Action *tools_left;
		Q3Action *tools_center_vertically;
		Q3Action *tools_right;
		Q3Action *tools_top;
		Q3Action *tools_center_horizontally;
		Q3Action *tools_bottom;
		Q3Action *tools_flip_horizontally;
		Q3Action *tools_flip_vertically;
		Q3Action *tools_rotate_cw90;
		Q3Action *tools_rotate_ccw90;
		Q3Action *tools_rotate180;
		Q3Action *tools_perspective;
		
		Q3ActionGroup *tools_selection;
		Q3ActionGroup *tools_draw;
		Q3ActionGroup *tools_fills;
		Q3ActionGroup *tools_erasers;
		Q3ActionGroup *tools_view;
		Q3ActionGroup *tools_order;
		Q3ActionGroup *tools_align;
		Q3ActionGroup *tools_transform;
		
		Q3PopupMenu *control;
		int id_control;
		Q3Action *control_play;
		Q3Action *control_rewind;
		Q3Action *control_go_to_end;
		Q3Action *control_step_forward;
		Q3Action *control_step_backward;
		
		Q3PopupMenu *window;
		int id_window;
		int id_window_timeline;
		
		Q3PopupMenu *help;
		int id_help;
		Q3Action *help_contents;
		Q3Action *help_about;
		Q3Action *help_about_qt;
		Q3Action *help_about_opengl;
		
		//Tool Bar and Tool Bar buttons
		Q3ToolBar *tool_bar, *tool_bar2/*, *tool_bar3*/;
		
		Q3Action *file_new;
		Q3Action *file_open;
		Q3Action *file_save;
		
		Q3Action *edit_undo;
		Q3Action *edit_redo;
		
		Q3Action *edit_cut;
		Q3Action *edit_copy;
		Q3Action *edit_paste;
		
		Q3Action *view_no_grid;
		Q3Action *view_grid12;
		Q3Action *view_grid16;
		Q3Action *view_subgrid;
		
		Q3Action *view_no_previous;
		Q3Action *view_previous;
		Q3Action *view_previous2;
		Q3Action *view_previous3;
		
		Q3Action *view_no_next;
		Q3Action *view_next;
		Q3Action *view_next2;
		Q3Action *view_next3;
		
		Q3Action *view_front_back_grid;
		Q3Action *view_ntsc;
		Q3Action *view_light_table;
		
		Q3Action *window_illustration;
		Q3Action *window_animation;
		Q3Action *window_tools;
		
		Q3Action *window_brushes;
		Q3Action *window_scenes;
		Q3Action *window_color_palette;
		Q3Action *window_exposure_sheet;
		
		Q3Action *window_library;
		Q3Action *window_drawing_area;
		
		QToolButton *window_timeline;
		
		Q3Action *window_render_camera_preview;
		Q3Action *window_top_camera_view;
		Q3Action *window_side_camera_view;
		
		//Main Panel that manages the documents
		QWorkspace *main_panel;
		
		//Non Modal Dialog Boxes for environment interaction
		Tools *tools_dialog;
		Brushes *brushes_dialog;
		Scenes *scenes_dialog;
		ColorPalette *color_palette_dialog;
		
// 		ExposureSheet *exposure_sheet_dialog;
		KTExposureSheet *m_ExposureSheetDialog;
#if 0
		KTViewDocument *m_viewDocument;
#endif	
		KTTimeLine *m_timeLine;
		
		Library *library_dialog;
	// 	GLRenderCameraPreview *render_camera_preview;
		KTCameraPreview *m_cameraPreview;
		GLSideCameraView *side_camera_view;
		GLTopCameraView *top_camera_view;
		About *about_dialog;
		
		//Modal Dialog Boxes for several settings
		Properties *properties_dialog;
		Preferences *preferences_dialog;
		Import *import_dialog;
		Export *export_dialog;
	
	// 	RenderCameraPreviewQt *render_camera_preview_qt;
		
		//Some colors needed by Exposure Sheet and Timeline (They are created here for efficiency)
		
	// 	QColor *es_default_color, *es_use_color, *es_selection_color, *es_select_and_use_color, 
	// 		   *es_text_color_when_selected, *es_default_text_color, *es_lock_color, *es_lock_and_select_color, 
	// 		   *es_has_drawing_color;
		QColor *tl_default_color, *tl_special_color, *tl_selection_color, *tl_offset_color, *tl_drag_offset_color, 
			*tl_border_color, *tl_use_border_color, *tl_motion_color, *tl_drawing_color;
			
	signals:
		void changedCursor(QCursor);
};

#endif
