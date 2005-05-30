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

#include <qtooltip.h>
#include <qtoolbutton.h>
#include <qaccel.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <ktapplication.h>
#include <memory>
#include <qwmatrix.h>
#include <qprocess.h>

#include "ktoon.h"
#include "images.h"
#include "symbol.h"
#include "folder.h"

#include "ktxmlreader.h"
#include "ktconfigdocument.h"

//--------------- CONSTRUCTOR --------------------

KToon::KToon() : QMainWindow( 0, "KToon", WDestructiveClose )
{
	qDebug("[Initializing KToon]");
	QString recent1, recent2, recent3, recent4, recent5;
	QFile settings( QDir::homeDirPath()+QString("/.ktoonrc"));
	
	KTXmlReader reader;
	QXmlInputSource xmlsource(&settings);
	
	if ( ! reader.parseXml(&xmlsource))
	{
		qDebug( "Could not open the settings file. Loaded empty recent file names." );
		recent1 = "<empty>";
		recent2 = "<empty>";
		recent3 = "<empty>";
		recent4 = "<empty>";
		recent5 = "<empty>";
	}
	else
	{
		XMLResults results = reader.getResults();
		recent1 = results["file1"];
		recent2 = results["file2"];
		recent3 = results["file3"];
		recent4 = results["file4"];
		recent5 = results["file5"];
	}
	
	settings.close();
	
	//ExposureSheet & Timeline Color initializations
	
	setupColors();
	setupIcons();
	setupCursors();
	setupToolBarActions();
	setupMenu();
	
	//Current Status Object
	current_status = new Status();
	current_status -> setCurrentCursor( Tools::NORMAL_SELECTION );
	
	//Image Initializations
	background_image = QPixmap( background_xpm );
	
	
	
	//--------------- Status Bar --------------------
	
	statusBar() -> message( tr( "Ready." ), 2000 );
	
	//--------------- Main Panel ----------------
	
	main_panel = new QWorkspace( this );
	main_panel -> setPaletteBackgroundPixmap( background_image );
	main_panel -> show();
	
	//--------------- Document Object ---------------
	
	document_ = new Document();
	document_ -> setNameDocument( tr( "Document" ) + QString( "1" ) );
	
	//-------------- Drawing Area ------------------
	
	// TODO: for non-accelerated We need use macros like USE_OPENGL
	//*** ---- Test for the non accelerated mode ---

#ifdef DEBUG_QT
	current_drawing_area_qt = new DrawingAreaQt( main_panel, this, tr( "Document" ) + QString( "1" ) );
	current_drawing_area_qt -> resize( 641, 481 );
	current_drawing_area_qt -> move( 146, 8 );
	current_drawing_area_qt -> show();
	
	render_camera_preview_qt = new RenderCameraPreviewQt( main_panel, this, current_drawing_area_qt -> canvas() );
	render_camera_preview_qt -> setMinimumSize( 321, 241 );
	render_camera_preview_qt -> setMaximumSize( 321, 241 );
	QWMatrix matrix = render_camera_preview_qt -> worldMatrix();
	matrix.scale( 0.47, 0.47 );
	render_camera_preview_qt -> setWorldMatrix( matrix );
	render_camera_preview_qt -> show();
#else
	current_drawing_area = new DrawingArea( main_panel, this, tr( "Document" ) + QString( "1" ) );
	current_drawing_area -> setCursor( QCursor( Qt::ForbiddenCursor ) );
	current_drawing_area -> show();
	current_drawing_area -> setActiveWindow();
#endif
	
	// ---- ***

	setupDialogs();
	
	//------------- Generic Initializations -----------
	
	//resize( 1023, 728 ); // FIXME: Revisar esto!!
	//showMaximized();
	
// 	setFont( QFont( "helvetica", 10 ) );
// 	QToolTip::setFont( QFont( "helvetica", 8 ) );
	setCentralWidget( main_panel );
	document_max_value = 1;
	file_name = "";
	recent_names << recent1 << recent2 << recent3 << recent4 << recent5;
// 	setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
	window -> setItemEnabled( id_window_illustration, false );
	current_status -> setCurrentScene( ( document_ -> getAnimation() -> getScenes() ).first() );
	current_status -> setCurrentLayer( ( current_status -> currentScene() -> getLayers() ).first() );
	
	//------------- Main Connections ----------------

	connect( exposure_sheet_dialog, SIGNAL( layerInserted() ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotInsertLayer() ) );
	
	connect( exposure_sheet_dialog, SIGNAL( layerInserted() ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotInsertFrameSequence() ) );
	connect( exposure_sheet_dialog, SIGNAL( layerRemoved() ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotRemoveLayer() ) );
	connect( exposure_sheet_dialog, SIGNAL( layerRemoved() ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotRemoveFrameSequence() ) );

	connect( exposure_sheet_dialog, SIGNAL( layerRemoved() ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotUpdateMaxUsedFrames() ) );
	connect( exposure_sheet_dialog, SIGNAL( layerSelected( int ) ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotSelectLayerFromES( int ) ) );
	connect( exposure_sheet_dialog, SIGNAL( layerRenamed( int, const QString & ) ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotRenameLayerFromES( int, const QString & ) ) );

	connect( exposure_sheet_dialog, SIGNAL( framesInsertedAtTheEnd( int ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotInsertKeyframesIntoTheCurrentFS( int ) ) );
	connect( exposure_sheet_dialog, SIGNAL( frameRemoved( int ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotRemoveKeyframeBlockFromTheCurrentFS( int ) ) );
	connect( exposure_sheet_dialog, SIGNAL( frameMovedUp( int ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotMoveKeyframeLeftInTheCurrentFS( int ) ) );
	connect( exposure_sheet_dialog, SIGNAL( frameMovedDown( int ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotMoveKeyframeRightInTheCurrentFS( int ) ) );
	
#ifndef USE_QT
	connect( exposure_sheet_dialog, SIGNAL( frameSelected() ), current_drawing_area, SLOT( slotSelectFrame() ) );
#endif
	connect( exposure_sheet_dialog, SIGNAL( frameSelected() ), SLOT( slotActivateCursor() ) );

	connect( timeline_dialog, SIGNAL( insertLayerClicked() ), exposure_sheet_dialog, SLOT( slotInsertLayer() ) );
	connect( timeline_dialog, SIGNAL( removeLayerClicked() ), exposure_sheet_dialog, SLOT( slotRemoveLayer() ) );
	connect( timeline_dialog -> layerManager() -> layerSequence(), SIGNAL( layerSelectedToES( int ) ), exposure_sheet_dialog, SLOT( slotSelectLayerFromTL( int ) ) );
	connect( timeline_dialog -> layerManager() -> layerSequence(), SIGNAL( layerRenamed( int, const QString & ) ), exposure_sheet_dialog, SLOT( slotRenameLayerFromTL( int, const QString & ) ) );

	connect( timeline_dialog -> frameSequenceManager() -> frameLayout(), SIGNAL( keyframeRemovedToES( int ) ), exposure_sheet_dialog, SLOT( slotRemoveFrameFromTL( int ) ) );
	connect( timeline_dialog, SIGNAL( layerMovedUp() ), exposure_sheet_dialog, SLOT( slotMoveLayerLeft() ) );
	connect( timeline_dialog, SIGNAL( layerMovedDown() ), exposure_sheet_dialog, SLOT( slotMoveLayerRight() ) );
	connect( timeline_dialog -> layerManager() -> layerSequence(), SIGNAL( layerReleasedAbove( int, int ) ), exposure_sheet_dialog, SLOT( slotSwapWithLeftLayer( int, int ) ) );
	connect( timeline_dialog -> layerManager() -> layerSequence(), SIGNAL( layerReleasedBelow( int, int ) ), exposure_sheet_dialog, SLOT( slotSwapWithRightLayer( int, int ) ) );
	connect( timeline_dialog -> frameSequenceManager() -> frameLayout(), SIGNAL( motionTweenCreatedToES( int ) ), exposure_sheet_dialog, SLOT( slotCreateMotionTween( int ) ) );
	connect( timeline_dialog -> frameSequenceManager() -> frameLayout(), SIGNAL( motionTweenRemovedToES( int ) ), exposure_sheet_dialog, SLOT( slotRemoveMotionTween( int ) ) );

	insert -> connectItem( id_insert_scene, scenes_dialog, SLOT( slotInsertScene() ) );
	insert -> connectItem( id_insert_remove_scene, scenes_dialog, SLOT( slotRemoveScene() ) );
#ifndef USE_QT
	file -> connectItem( id_file_close, current_drawing_area, SLOT( close() ) );
	connect( current_drawing_area, SIGNAL( closed() ), SLOT( slotCloseDrawingArea() ) );
	connect( color_palette_dialog, SIGNAL( outlineColorChanged() ), current_drawing_area, SLOT( slotChangeOutlineColor() ) );
	connect( color_palette_dialog, SIGNAL( fillColorChanged() ), current_drawing_area, SLOT( slotChangeFillColor() ) );
	connect( brushes_dialog, SIGNAL( minThicknessChanged() ), current_drawing_area, SLOT( slotChangeMinThicknessBrush() ) );
	connect( brushes_dialog, SIGNAL( maxThicknessChanged() ), current_drawing_area, SLOT( slotChangeMaxThicknessBrush() ) );
	connect( brushes_dialog, SIGNAL( smoothnessChanged() ), current_drawing_area, SLOT( slotChangeSmoothnessBrush() ) );
#endif

	connect( scenes_dialog, SIGNAL( sceneInserted() ), SLOT( slotInsertSync() ) );
	connect( scenes_dialog, SIGNAL( sceneMovedDown( int ) ), SLOT( slotMoveDownSync( int ) ) );
	connect( scenes_dialog, SIGNAL( sceneMovedUp( int ) ), SLOT( slotMoveUpSync( int ) ) );
	connect( scenes_dialog, SIGNAL( sceneRemoved( int ) ), SLOT( slotRemoveSync( int ) ) );
	connect( scenes_dialog, SIGNAL( sceneSelected( int ) ), SLOT( slotSelectSync( int ) ) );
	
#ifndef USE_QT
	connect( current_drawing_area, SIGNAL( colorGrabbed( Color::Color * ) ), color_palette_dialog, SLOT( slotSetColor( Color::Color * ) ) );
	connect( current_drawing_area, SIGNAL( updated() ), library_dialog -> getSymbolView(), SLOT( updateGL() ) );
	connect( current_drawing_area, SIGNAL( wasDrawn( bool ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotFrameHasDrawing( bool ) ) );
#endif
	
	scenes_dialog -> selectFirstScene();
	exposure_sheet_dialog -> touchFirstFrame();
	exposure_sheet_dialog -> updateIndicators( exposure_sheet_dialog -> currentLayerObj() );
	
	updateOpenRecentMenu();
}

void KToon::setupColors()
{
    es_default_color = new QColor( 239, 237, 223 );
    es_use_color = new QColor( 200, 200, 200 );
    es_selection_color = new QColor( 0, 0, 0 );
    es_select_and_use_color = new QColor( 255, 255, 255 );
    es_text_color_when_selected = new QColor( 255, 255, 255 );
    es_default_text_color = new QColor( 0, 0, 0 );
    es_lock_color = new QColor( 150, 150, 255 );
    es_lock_and_select_color = new QColor( 210, 210, 255 );
    es_has_drawing_color = new QColor( 170, 70, 10 );
    
    tl_default_color = new QColor( 255, 255, 255 );
    tl_selection_color = new QColor( 0, 0, 0 );
    tl_special_color = new QColor( 200, 200, 200 );
    tl_offset_color = new QColor( 255, 0, 0 );
    tl_drag_offset_color = new QColor( 0, 0, 0 );
    tl_border_color = new QColor( 180, 180, 180 );
    tl_use_border_color = new QColor( 0, 0, 0 );
    tl_motion_color = new QColor( 220, 220, 255 );
    tl_drawing_color = new QColor( 230, 230, 230 );
}


void KToon::setupIcons()
{
    icon_new = QPixmap( new_xpm );
    icon_open = QPixmap( open_xpm );
    icon_save = QPixmap( save_xpm );
    icon_import = QPixmap( import_xpm );
    icon_export = QPixmap( export_xpm );
    icon_undo = QPixmap( undo_xpm );
    icon_redo = QPixmap( redo_xpm );
    icon_zoom_in = QPixmap( zoomin_xpm );
    icon_zoom_out = QPixmap( zoomout_xpm );
    icon_fit = QPixmap( fit_xpm );
    icon_grid12 = QPixmap( grid12_xpm );
    icon_grid16 = QPixmap( grid16_xpm );
    icon_subgrid = QPixmap( subgrid_xpm );
    icon_no_grid = QPixmap( nogrid_xpm );
    icon_close = QPixmap( close_xpm );
    icon_cut = QPixmap( cut_xpm );
    icon_copy = QPixmap( copy_xpm );
    icon_paste = QPixmap( paste_xpm );
    icon_perspective = QPixmap( perspective_xpm );
    icon_ani_mode = QPixmap( ani_mode_xpm );
    icon_bring_to_front = QPixmap( bring_to_front_xpm );
    icon_brushes = QPixmap( brushes_xpm );
    icon_camera_preview = QPixmap( camera_preview_xpm );
    icon_color_palette = QPixmap( color_palette_xpm );
    icon_drawing_area = QPixmap( drawing_area_xpm );
    icon_exposure_sheet = QPixmap( exposure_sheet_xpm );
    icon_group = QPixmap( group_xpm );
    icon_illus_mode = QPixmap( illus_mode_xpm );
    icon_library = QPixmap( library_xpm );
    icon_light_table = QPixmap( light_table_xpm );
    icon_front_back_grid = QPixmap( front_back_grid_xpm );
    icon_ntsc = QPixmap( ntsc_xpm );
    icon_one_backward = QPixmap( one_backward_xpm );
    icon_one_forward = QPixmap( one_forward_xpm );
    icon_properties = QPixmap( properties_xpm );
    icon_scenes = QPixmap( scenes_xpm );
    icon_send_to_back = QPixmap( send_to_back_xpm );
    icon_side_camera = QPixmap( side_camera_xpm );
    icon_time_line = QPixmap( time_line_xpm );
    icon_top_camera = QPixmap( top_camera_xpm );
    icon_tools = QPixmap( tools_xpm );
    icon_ungroup = QPixmap( ungroup_xpm );
    icon_no_previous = QPixmap( no_previous_xpm );
    icon_previous = QPixmap( previous_xpm );
    icon_previous2 = QPixmap( previous2_xpm );
    icon_previous3 = QPixmap( previous3_xpm );
    icon_no_next = QPixmap( no_next_xpm );
    icon_next = QPixmap( next_xpm );
    icon_next2 = QPixmap( next2_xpm );
    icon_next3 = QPixmap( next3_xpm );
    icon_align_l = QPixmap( align_l_xpm );
    icon_align_r = QPixmap( align_r_xpm );
    icon_align_t = QPixmap( align_t_xpm );
    icon_align_b = QPixmap( align_b_xpm );
    icon_align_cv = QPixmap( align_cv_xpm );
    icon_align_ch = QPixmap( align_ch_xpm );

    icon_selection = QPixmap( selection_xpm );
    icon_nodes = QPixmap( nodes_xpm );
    icon_brush = QPixmap( brush_xpm );
    icon_pencil = QPixmap( pencil_xpm );
    icon_line = QPixmap( line_xpm );
    icon_pen = QPixmap( bezier_xpm );
    icon_rectangle = QPixmap( square_xpm );
    icon_ellipse = QPixmap( ellipse_xpm );
    icon_eraser = QPixmap( eraser_xpm );
    icon_slicer = QPixmap( slicer_xpm );
    icon_fill = QPixmap( fill_xpm );
    icon_contour_fill = QPixmap( contour_xpm );
    icon_remove_fill = QPixmap( removefill_xpm );
    icon_dropper = QPixmap( dropper_xpm );
    icon_magnifying_glass = QPixmap( magnifying_xpm );
    icon_hand = QPixmap( hand_xpm );
}

void KToon::setupCursors()
{
    //Cursor Initializations
    i_cursor_selection = QPixmap( cselect_xpm );
    i_cursor_nodes = QPixmap( cnodes_xpm );
    i_cursor_brush = QPixmap( cbrush_xpm );
    i_cursor_pencil = QPixmap( cpencil_xpm );
    i_cursor_pen = QPixmap( cpen_xpm );
    i_cursor_rectangle = QPixmap( csquare_xpm );
    i_cursor_ellipse = QPixmap( ccircle_xpm );
    i_cursor_eraser = QPixmap( ceraser_xpm );
    i_cursor_slicer = QPixmap( cslicer_xpm );
    i_cursor_fill = QPixmap( cpaint_xpm );
    i_cursor_remove_fill = QPixmap( cunpaint_xpm );
    i_cursor_magnifying_glass = QPixmap( cmagnifying_xpm );
    i_cursor_hand = QPixmap( chand_xpm );
    i_cursor_dropper = QPixmap( cdropper_xpm );
    i_cursor_contour_fill = QPixmap( ccontour_xpm );
    
    cursor_line = new QCursor( Qt::CrossCursor );
    cursor_selection = new QCursor( i_cursor_selection, 0, 0 );
    cursor_nodes = new QCursor( i_cursor_nodes, 1, 2 );
    cursor_brush = new QCursor( i_cursor_brush, 2, 13 );
    cursor_pencil = new QCursor( i_cursor_pencil, 0, 16 );
    cursor_pen = new QCursor( i_cursor_pen, 0, 14 );
    cursor_rectangle = new QCursor( i_cursor_rectangle );
    cursor_ellipse = new QCursor( i_cursor_ellipse );
    cursor_eraser = new QCursor( i_cursor_eraser, 1, 14 );
    cursor_slicer = new QCursor( i_cursor_slicer, 0, 13 );
    cursor_fill = new QCursor( i_cursor_fill, 0, 10 );
    cursor_remove_fill = new QCursor( i_cursor_remove_fill, 0, 10 );
    cursor_magnifying_glass = new QCursor( i_cursor_magnifying_glass, 5, 5 );
    cursor_hand = new QCursor( i_cursor_hand );
    cursor_dropper = new QCursor( i_cursor_dropper, 0, 14 );
    cursor_contour_fill = new QCursor( i_cursor_contour_fill, 0, 15 );
}

void KToon::setupToolBarActions()
{
    tool_bar = new QToolBar( this );
    tool_bar -> setLabel( tr( "File and Edit Operations" ) );
//     tool_bar -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );

    file_new = new QToolButton( icon_new, tr( "New Document" ), QString::null, this, SLOT( slotNewDocument() ), tool_bar );
    file_open = new QToolButton( icon_open, tr( "Open Document" ), QString::null, this, SLOT( slotChoose() ), tool_bar );
    file_save = new QToolButton( icon_save, tr( "Save Document" ), QString::null, this, SLOT( slotSave() ), tool_bar );
    tool_bar -> addSeparator();
    edit_undo = new QToolButton( icon_undo, tr( "Undo" ), QString::null, this, SLOT( slotUndo() ), tool_bar );
    edit_redo = new QToolButton( icon_redo, tr( "Redo" ), QString::null, this, SLOT( slotRedo() ), tool_bar );
    tool_bar -> addSeparator();
    edit_cut = new QToolButton( icon_cut, tr( "Cut" ), QString::null, this, SLOT( slotCut() ), tool_bar );
    edit_copy = new QToolButton( icon_copy, tr( "Copy" ), QString::null, this, SLOT( slotCopy() ), tool_bar );
    edit_paste = new QToolButton( icon_paste, tr( "Paste" ), QString::null, this, SLOT( slotPaste() ), tool_bar );
    tool_bar -> addSeparator();
    window_drawing_area = new QToolButton( icon_drawing_area, tr( "Drawing Area" ), QString::null, this, SLOT( slotWindowDrawingArea() ), tool_bar );
    window_drawing_area -> setDown( true );
    window_tools = new QToolButton( icon_tools, tr( "Tools" ), QString::null, this, SLOT( slotWindowTools() ), tool_bar );
    window_tools -> setDown( true );
    window_exposure_sheet = new QToolButton( icon_exposure_sheet, tr( "Exposure Sheet" ), QString::null, this, SLOT( slotWindowExposureSheet() ), tool_bar );
    window_exposure_sheet -> setDown( true );
    window_color_palette = new QToolButton( icon_color_palette, tr( "Color Palette" ), QString::null, this, SLOT( slotWindowColorPalette() ), tool_bar );
    window_color_palette -> setDown( true );
    window_brushes = new QToolButton( icon_brushes, tr( "Brushes" ), QString::null, this, SLOT( slotWindowBrushes() ), tool_bar );
    window_brushes -> setDown( true );
    window_scenes = new QToolButton( icon_scenes, tr( "Scenes" ), QString::null, this, SLOT( slotWindowScenes() ), tool_bar );
    window_scenes -> setDown( true );
    window_library = new QToolButton( icon_library, tr( "Library" ), QString::null, this, SLOT( slotWindowLibrary() ), tool_bar );
    window_timeline = new QToolButton( icon_time_line, tr( "Timeline" ), QString::null, this, SLOT( slotWindowTimeline() ), tool_bar );
    window_timeline -> hide();
    window_render_camera_preview = new QToolButton( icon_camera_preview, tr( "Render Camera Preview" ), QString::null, this, SLOT( slotWindowRenderCameraPreview() ), tool_bar );
    window_render_camera_preview -> hide();
    window_top_camera_view = new QToolButton( icon_top_camera, tr( "Top Camera View" ), QString::null, this, SLOT( slotWindowTopCameraView() ), tool_bar );
    window_top_camera_view -> hide();
    window_side_camera_view = new QToolButton( icon_side_camera, tr( "Side Camera View" ), QString::null, this, SLOT( slotWindowSideCameraView() ), tool_bar );
    window_side_camera_view -> hide();
    tool_bar -> addSeparator();
    window_illustration = new QToolButton( icon_illus_mode, tr( "Illustration Mode" ), QString::null, this, SLOT( slotSeeIllustration() ), tool_bar );
    window_illustration -> hide();
    window_animation = new QToolButton( icon_ani_mode, tr( "Animation Mode" ), QString::null, this, SLOT( slotSeeAnimation() ), tool_bar );

    tool_bar2 = new QToolBar( this );
    tool_bar2 -> setLabel( tr( "View Operations" ) );
//     tool_bar2 -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );

    view_no_grid = new QToolButton( icon_no_grid, tr( "No Grid" ), QString::null, this, SLOT( slotNoGrid() ), tool_bar2 );
    view_grid12 = new QToolButton( icon_grid12, tr( "Grid of 12 Fields" ), QString::null, this, SLOT( slotSeeGrid12() ), tool_bar2 );
    view_grid16 = new QToolButton( icon_grid16, tr( "Grid of 16 Fields" ), QString::null, this, SLOT( slotSeeGrid16() ), tool_bar2 );
    view_subgrid = new QToolButton( icon_subgrid, tr( "Grid with a 3 field Subgrid" ), QString::null, this, SLOT( slotSeeSubgrid() ), tool_bar2 );
    tool_bar2 -> addSeparator();
    view_front_back_grid = new QToolButton( icon_front_back_grid, tr( "Sends the grid to the front or to the back" ), QString::null, this, SLOT( slotFrontBackGrid() ), tool_bar2 );
    view_ntsc = new QToolButton( icon_ntsc, tr( "NTSC Zone" ), QString::null, this, SLOT( slotSeeNTSC() ), tool_bar2 );
    view_light_table = new QToolButton( icon_light_table, tr( "Light Table" ), QString::null, this, SLOT( slotLightTable() ), tool_bar2 );
    tool_bar2 -> addSeparator();
    view_no_previous = new QToolButton( icon_no_previous, tr( "No Previous Onion Skin" ), QString::null, this, SLOT( slotNoPreviousOnionSkin() ), tool_bar2 );
    view_previous = new QToolButton( icon_previous, tr( "Previous Onion Skin" ), QString::null, this, SLOT( slotPreviousOnionSkin() ), tool_bar2 );
    view_previous2 = new QToolButton( icon_previous2, tr( "Previous Two Onion Skin" ), QString::null, this, SLOT( slotPrevious2OnionSkin() ), tool_bar2 );
    view_previous3 = new QToolButton( icon_previous3, tr( "Previous Three Onion Skin" ), QString::null, this, SLOT( slotPrevious3OnionSkin() ), tool_bar2 );
    view_no_previous -> setDown( true );
    previous_toggled = view_no_previous;
    tool_bar2 -> addSeparator();
    view_no_next = new QToolButton( icon_no_next, tr( "No Next Onion Skin" ), QString::null, this, SLOT( slotNoNextOnionSkin() ), tool_bar2 );
    view_next = new QToolButton( icon_next, tr( "Next Onion Skin" ), QString::null, this, SLOT( slotNextOnionSkin() ), tool_bar2 );
    view_next2 = new QToolButton( icon_next2, tr( "Next Two Onion Skin" ), QString::null, this, SLOT( slotNext2OnionSkin() ), tool_bar2 );
    view_next3 = new QToolButton( icon_next3, tr( "Next Three Onion Skin" ), QString::null, this, SLOT( slotNext3OnionSkin() ), tool_bar2 );
    view_no_next -> setDown( true );
    next_toggled = view_no_next; 
}

void KToon::setupMenu()
{
//-------------------- Operations related with the Menu Bar ----------------------

//     menuBar() -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );

    //FILE MENU
    file = new QPopupMenu( this );
//     file -> setFont( QFont( "helvetica", 10 ) );
//     file -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_file = menuBar() -> insertItem( tr("&File"), file );

    id_file_new = file -> insertItem( icon_new, tr( "&New..." ), this, SLOT( slotNewDocument() ), CTRL+Key_N );
    id_file_open = file -> insertItem( icon_open, tr( "&Open..." ), this, SLOT( slotChoose() ), CTRL+Key_O );
        //Open Recent Submenu
    open_recent = new QPopupMenu( this );
//     open_recent -> setFont( QFont( "helvetica", 10 ) );
//     open_recent -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    connect( open_recent, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );
    connect( open_recent, SIGNAL( activated( int ) ), SLOT( slotOpenRecent( int ) ) );
    file -> insertItem( tr( "Open Recent" ), open_recent );
    id_file_save = file -> insertItem( icon_save, tr( "&Save" ), this, SLOT( slotSave() ), CTRL+Key_S );
    id_file_save_as = file -> insertItem( tr( "Save &As..." ), this, SLOT( slotSaveAs() ) );
    id_file_close = file -> insertItem( icon_close, tr( "Cl&ose" ) );
    file -> setAccel( CTRL+Key_W, id_file_close );
    file -> insertSeparator();
    id_file_import = file -> insertItem( icon_import, tr( "&Import..." ), this, SLOT( slotImport() ), CTRL+Key_I );
    id_file_export = file -> insertItem( icon_export, tr( "&Export..." ), this, SLOT( slotExport() ), CTRL+Key_E );
    file -> setItemVisible( id_file_export, false );
    file -> insertSeparator();
    id_file_properties = file -> insertItem( tr( "&Properties..." ), this, SLOT( slotProperties() ) );
    file -> insertSeparator();
    id_file_exit = file -> insertItem( tr( "E&xit" ), this, SLOT( close() ), CTRL+Key_Q );
    connect( file, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );

    //EDIT MENU
    edit = new QPopupMenu( this );
//     edit -> setFont( QFont( "helvetica", 10 ) );
//     edit -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_edit = menuBar() -> insertItem( tr( "&Edit" ), edit );

    id_edit_undo = edit -> insertItem( icon_undo, tr( "&Undo" ), this, SLOT( slotUndo() ), CTRL+Key_Z );
    id_edit_redo = edit -> insertItem( icon_redo, tr( "&Redo" ), this, SLOT( slotRedo() ), CTRL+SHIFT+Key_Z );
    edit -> insertSeparator();
    id_edit_cut = edit -> insertItem( icon_cut, tr( "&Cut" ), this, SLOT( slotCut() ), CTRL+Key_X );
    id_edit_copy = edit -> insertItem( icon_copy, tr( "C&opy" ), this, SLOT( slotCopy() ), CTRL+Key_C );
    id_edit_paste = edit -> insertItem( icon_paste, tr( "&Paste" ), this, SLOT( slotPaste() ), CTRL+Key_V );
    id_edit_paste_in_place = edit -> insertItem( tr( "Paste &In Place" ), this, SLOT( slotPasteInPlace() ), CTRL+SHIFT+Key_V );
    id_edit_delete = edit -> insertItem( tr( "&Delete" ), this, SLOT( slotDelete() ), Key_Delete );
    edit -> insertSeparator();
    id_edit_select_all = edit -> insertItem( tr( "&Select All" ), this, SLOT( slotSelectAll() ), CTRL+Key_A );
    edit -> insertSeparator();
    id_edit_preferences = edit -> insertItem( tr( "Pr&eferences..." ), this, SLOT( slotPreferences() ) );
    connect( edit, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );

    //VIEW MENU
    view = new QPopupMenu( this );
//     view -> setFont( QFont( "helvetica", 10 ) );
//     view -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_view = menuBar() -> insertItem( tr( "&View" ), view );

    id_view_zoom_in = view -> insertItem( icon_zoom_in, tr( "Zoom &In" ), this, SLOT( slotZoomIn() ), CTRL+Key_Plus );
    id_view_zoom_out = view -> insertItem( icon_zoom_out, tr( "Zoom &Out" ), this, SLOT( slotZoomOut() ), CTRL+Key_Minus );
    id_view_fit = view -> insertItem( icon_fit, tr( "&Fit" ), this, SLOT( slotFit() ) );
    view -> insertSeparator();
        //Grid Submenu
    grid = new QPopupMenu( this );
//     grid -> setFont( QFont( "helvetica", 10 ) );
//     grid -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_view_no_grid = grid -> insertItem( icon_no_grid, tr( "&No Grid" ), this, SLOT( slotNoGrid() ) );
    id_view_grid12 = grid -> insertItem( icon_grid12, tr( "&12 Field Grid" ), this, SLOT( slotSeeGrid12() ) );
    id_view_grid16 = grid -> insertItem( icon_grid16, tr( "&16 Field Grid" ), this, SLOT( slotSeeGrid16() ) );
    id_view_subgrid = grid -> insertItem( icon_subgrid, tr( "&Subgrid" ), this, SLOT( slotSeeSubgrid() ) );
    connect( grid, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );
    view -> insertItem( tr( "&Grid" ), grid );
    id_view_front_back_grid = view -> insertItem ( icon_front_back_grid, tr( "Grid to Front/Back" ), this, SLOT( slotFrontBackGrid() ) );
    id_view_NTSC_zone = view -> insertItem( icon_ntsc, tr( "&NTSC Zone" ), this, SLOT( slotSeeNTSC() ) );
    id_view_light_table = view -> insertItem( icon_light_table, tr( "&Light Table" ), this, SLOT( slotLightTable() ) );
    	//Onion Skin Submenu
    onion_skin = new QPopupMenu( this );
//     onion_skin -> setFont( QFont( "helvetica", 10 ) );
//     onion_skin -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_view_previous_none = onion_skin -> insertItem( icon_no_previous, tr( "No Previous" ), this, SLOT( slotNoPreviousOnionSkin() ), Key_0 );
    id_view_previous = onion_skin -> insertItem( icon_previous, tr( "Previous One" ), this, SLOT( slotPreviousOnionSkin() ), Key_1 );
    id_view_previous2 = onion_skin -> insertItem( icon_previous2, tr( "Previous Two" ), this, SLOT( slotPrevious2OnionSkin() ), Key_2 );
    id_view_previous3 = onion_skin -> insertItem( icon_previous3, tr( "Previous Three" ), this, SLOT( slotPrevious3OnionSkin() ), Key_3 );
    onion_skin -> setItemChecked( id_view_previous_none, true );
    previous_checked = id_view_previous_none;
    id_view_next_none = onion_skin -> insertItem( icon_no_next, tr( "No Next" ), this, SLOT( slotNoNextOnionSkin() ), CTRL + Key_0 );
    id_view_next = onion_skin -> insertItem( icon_next, tr( "Next One" ), this, SLOT( slotNextOnionSkin() ), CTRL + Key_1 );
    id_view_next2 = onion_skin -> insertItem( icon_next2, tr( "Next Two" ), this, SLOT( slotNext2OnionSkin() ), CTRL + Key_2 );
    id_view_next3 = onion_skin -> insertItem( icon_next3, tr( "Next Three" ), this, SLOT( slotNext3OnionSkin() ), CTRL + Key_3 );
    onion_skin -> setItemChecked( id_view_next_none, true );
    next_checked = id_view_next_none;
    connect( onion_skin, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );
    view -> insertItem( tr( "Onion &Skin" ), onion_skin );
    view -> insertSeparator();
    id_view_rotate_clockwise = view -> insertItem( tr( "Rotate &Clockwise" ), this, SLOT( slotRotateClockwise() ), Key_V );
    id_view_rotate_counterclockwise = view -> insertItem( tr( "Rotate Counterclock&wise" ), this, SLOT( slotRotateCounterClockwise() ), Key_C );
    id_view_restore_rotation = view -> insertItem( tr( "&Restore Rotation" ), this, SLOT( slotRestoreRotation() ), SHIFT + Key_C );
    connect( view, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );

    //INSERT MENU
    insert = new QPopupMenu( this );
//     insert -> setFont( QFont( "helvetica", 10 ) );
//     insert -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_insert = menuBar() -> insertItem( tr( "&Insert" ), insert );

    id_insert_layer = insert -> insertItem( tr( "&Layer" ), this, SLOT( slotInsertLayer() ) );
    insert -> insertSeparator();
    id_insert_frame = insert -> insertItem( tr( "&Frame" ), this, SLOT( slotInsertFrame() ), Key_F5 );
    insert -> setItemVisible( id_insert_frame, false );
    id_insert_remove_frame = insert -> insertItem( tr( "&Remove Frame" ), this, SLOT( slotRemoveFrame() ), SHIFT+Key_F5 );
    insert -> setItemVisible( id_insert_remove_frame, false );
    id_insert_keyframe = insert -> insertItem( tr("&KeyFrame"), this, SLOT( slotInsertKeyFrame() ), Key_F6 );
    insert -> insertSeparator();
    id_insert_scene = insert -> insertItem( tr("&Scene") );
    id_insert_remove_scene = insert -> insertItem( tr( "Re&move Scene" ) );
    connect( insert, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );

    //TOOLS MENU
    tools = new QPopupMenu( this );
//     tools -> setFont( QFont( "helvetica", 10 ) );
//     tools -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_tools = menuBar() -> insertItem( tr( "&Tools" ), tools );

       	//Selection Submenu
    QPopupMenu *selection = new QPopupMenu( this );
//     selection -> setFont( QFont( "helvetica", 10 ) );
//     selection -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_tools_normal_selection = selection -> insertItem( icon_selection, tr( "Normal &Selection" ), this, SLOT( slotNormalSelection() ), Key_S );
    id_tools_contour_selection = selection -> insertItem( icon_nodes, tr( "Con&tour Selection" ), this, SLOT( slotContourSelection() ), Key_T );
    connect( selection, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );
    tools -> insertItem( tr( "&Selection" ), selection );
       	//Draw Submenu
    QPopupMenu *draw = new QPopupMenu( this );
//     draw -> setFont( QFont( "helvetica", 10 ) );
//     draw -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_tools_brush = draw -> insertItem( icon_brush, tr( "&Brush" ), this, SLOT( slotBrush() ), Key_B );
    id_tools_pencil = draw -> insertItem( icon_pencil, tr( "&Pencil" ), this, SLOT( slotPencil() ), Key_P );
    id_tools_pen = draw -> insertItem( icon_pen, tr( "&Pen" ), this, SLOT( slotPen() ), Key_N );
    id_tools_line = draw -> insertItem( icon_line, tr( "&Line" ), this, SLOT( slotLine() ), Key_L );
    id_tools_rectangle = draw -> insertItem( icon_rectangle, tr( "&Rectangle" ), this, SLOT( slotRectangle() ), Key_R );
    id_tools_ellipse = draw -> insertItem( icon_ellipse, tr( "&Ellipse" ), this, SLOT( slotEllipse() ), Key_E );
    connect( draw, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );
    tools -> insertItem( tr( "D&raw" ), draw );
       	//Fill Submenu
    QPopupMenu *fill = new QPopupMenu( this );
//     fill -> setFont( QFont( "helvetica", 10 ) );
//     fill -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_tools_fill = fill -> insertItem( icon_fill, tr( "&Fill" ), this, SLOT( slotFill() ), Key_F );
    id_tools_remove_fill = fill -> insertItem( icon_remove_fill, tr( "&Remove Fill" ), this, SLOT( slotRemoveFill() ), SHIFT+Key_F );
    id_tools_contour_fill = fill -> insertItem( icon_contour_fill, tr( "&Contour Fill" ), this, SLOT( slotContourFill() ), CTRL+Key_F );
    id_tools_dropper = fill -> insertItem( icon_dropper, tr( "&Dropper" ), this, SLOT( slotDropper() ), Key_D );
    connect( fill, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );
    tools -> insertItem( tr( "&Fill" ), fill );
       	//Eraser Submenu
    QPopupMenu *eraser = new QPopupMenu( this );
//     eraser -> setFont( QFont( "helvetica", 10 ) );
//     eraser -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_tools_eraser = eraser -> insertItem( icon_eraser, tr( "&Eraser" ), this, SLOT( slotEraser() ), SHIFT+Key_Delete );
    id_tools_slicer = eraser -> insertItem( icon_slicer, tr( "&Slicer" ), this, SLOT( slotSlicer() ), CTRL+Key_Delete );
    connect( eraser, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );
    tools -> insertItem( tr( "&Eraser" ), eraser );
       	//View_ Submenu
    QPopupMenu *view_ = new QPopupMenu( this );
//     view_ -> setFont( QFont( "helvetica", 10 ) );
//     view_ -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_tools_magnifying_glass = view_ -> insertItem( icon_magnifying_glass, tr( "&Magnifying Glass" ), this, SLOT( slotMagnifyingGlass() ), Key_M );
    id_tools_hand = view_ -> insertItem( icon_hand, tr( "&Hand" ), this, SLOT( slotHand() ), Key_H );
    connect( view_, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );
    tools -> insertItem( tr("&View"), view_ );
    tools -> insertSeparator();
    id_tools_group = tools -> insertItem( icon_group, tr( "&Group" ), this, SLOT( slotGroup() ) );
    id_tools_ungroup = tools -> insertItem( icon_ungroup, tr( "&Ungroup" ), this, SLOT( slotUngroup() ) );
    tools -> insertSeparator();
    	//Order Submenu
    QPopupMenu *order = new QPopupMenu( this );
//     order -> setFont( QFont( "helvetica", 10 ) );
//     order -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_tools_bring_front = order -> insertItem( icon_bring_to_front, tr( "&Bring to Front" ), this, SLOT( slotBringToFront() ), CTRL+SHIFT+Key_Up );
    id_tools_send_back = order -> insertItem( icon_send_to_back, tr( "&Send to Back" ), this, SLOT( slotSendToBack() ), CTRL+SHIFT+Key_Down );
    id_tools_one_step_forward = order -> insertItem( icon_one_forward, tr( "One Step &Forward" ), this, SLOT( slotOneStepForward() ), CTRL+Key_Up );
    id_tools_one_step_backward = order -> insertItem( icon_one_backward, tr( "One Step B&ackward" ), this, SLOT( slotOneStepBackward() ), CTRL+Key_Down );
    connect( order, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );
    tools -> insertItem( tr( "&Order" ), order );
    	//Align Submenu
    QPopupMenu *align = new QPopupMenu( this );
//     align -> setFont( QFont( "helvetica", 10 ) );
//     align -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_tools_left = align -> insertItem( icon_align_l, tr( "&Left" ), this, SLOT( slotAlignLeft() ) );
    id_tools_center_vertically = align -> insertItem( icon_align_cv, tr( "&Center Vertically" ), this, SLOT( slotCenterVertically() ) );
    id_tools_right = align -> insertItem( icon_align_r, tr( "&Right" ), this, SLOT( slotAlignRight() ) );
    align -> insertSeparator();
    id_tools_top = align -> insertItem( icon_align_t, tr( "&Top" ), this, SLOT( slotAlignTop() ) );
    id_tools_center_horizontally = align -> insertItem( icon_align_ch, tr( "Center &Horizontally" ), this, SLOT( slotCenterHorizontally() ) );
    id_tools_bottom = align -> insertItem( icon_align_b, tr( "&Bottom" ), this, SLOT( slotAlignBottom() ) );
    connect( align, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );
    tools -> insertItem(tr( "A&lign" ), align );
    	//Transform Submenu
    QPopupMenu *transform = new QPopupMenu( this );
//     transform -> setFont( QFont( "helvetica", 10 ) );
//     transform -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_tools_flip_horizontally = transform -> insertItem( tr( "Flip &Horizontally" ), this, SLOT( slotFlipHorizontally() ) );
    id_tools_flip_vertically = transform -> insertItem( tr( "Flip &Vertically" ), this, SLOT( slotFlipVertically() ) );
    transform -> insertSeparator();
    id_tools_rotate_cw90 = transform -> insertItem( tr( "&Rotate 90 CW" ), this, SLOT( slotRotateCW90() ) );
    id_tools_rotate_ccw90 = transform -> insertItem( tr( "R&otate 90 CCW" ), this, SLOT( slotRotateCCW90() ) );
    id_tools_rotate180 = transform -> insertItem( tr( "Rotate &180" ), this, SLOT( slotRotate180() ) );
    transform -> insertSeparator();
    id_tools_perspective = transform -> insertItem( icon_perspective, tr( "&Perspective" ), this, SLOT( slotPerspectiveSelection() ) );
    connect( transform, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );
    tools -> insertItem( tr( "&Transform" ), transform );
    connect( tools, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );

    //CONTROL MENU
    control = new QPopupMenu( this );
//     control -> setFont( QFont( "helvetica", 10 ) );
//     control -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_control = menuBar() -> insertItem( tr( "&Control" ), control );

    id_control_play = control -> insertItem( tr( "&Play/Stop" ), this, SLOT( slotPlayStop() ), Key_Return );
    id_control_rewind = control -> insertItem( tr( "&Rewind" ), this, SLOT( slotRewind() ), CTRL+Key_Comma );
    id_control_go_to_end = control -> insertItem( tr( "Go to &End" ), this, SLOT( slotGoToEnd() ), CTRL+Key_Period );
    control -> insertSeparator();
    id_control_step_forward = control -> insertItem( tr( "Step &Forward" ), this, SLOT( slotStepForward() ), Key_Period );
    id_control_step_backward = control -> insertItem( tr( "Step &Backward" ), this, SLOT( slotStepBackward() ), Key_Comma );
    menuBar() -> setItemVisible( id_control, false );
    connect( control, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );

    //WINDOW MENU
    window = new QPopupMenu( this );
//     window -> setFont( QFont( "helvetica", 10 ) );
//     window -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_window = menuBar() -> insertItem( tr( "&Window" ), window );

    id_window_illustration = window -> insertItem( icon_illus_mode, tr( "&Illustration Mode" ), this, SLOT( slotSeeIllustration() ), Key_F9 );
    id_window_animation = window -> insertItem( icon_ani_mode, tr( "&Animation Mode" ), this, SLOT( slotSeeAnimation() ), Key_F10 );
    window -> insertSeparator();
    id_window_drawing_area = window -> insertItem( icon_drawing_area, tr( "&Drawing Area" ), this, SLOT( slotWindowDrawingArea() ), CTRL+Key_D );
    window -> setItemChecked( id_window_drawing_area, true );
    id_window_tools = window -> insertItem( icon_tools, tr( "&Tools" ), this, SLOT( slotWindowTools() ), CTRL+Key_T );
    window -> setItemChecked( id_window_tools, true );
    id_window_exposure_sheet = window -> insertItem( icon_exposure_sheet, tr( "&Exposure Sheet" ), this, SLOT( slotWindowExposureSheet() ), CTRL+Key_H );
    window -> setItemChecked( id_window_exposure_sheet, true );
    id_window_color_palette = window -> insertItem( icon_color_palette, tr( "&Color Palette" ), this, SLOT( slotWindowColorPalette() ), CTRL+Key_P );
    window -> setItemChecked( id_window_color_palette, true );
    id_window_brushes = window -> insertItem( icon_brushes, tr( "&Brushes" ), this, SLOT( slotWindowBrushes() ), CTRL+Key_B );
    window -> setItemChecked( id_window_brushes, true );
    id_window_scenes = window -> insertItem( icon_scenes, tr( "&Scenes" ), this, SLOT( slotWindowScenes() ), CTRL+Key_Y );
    window -> setItemChecked( id_window_scenes, true );
    id_window_library = window -> insertItem( icon_library, tr( "&Library" ), this, SLOT( slotWindowLibrary() ), CTRL+Key_L );
    id_window_timeline = window -> insertItem( icon_time_line, tr( "T&imeline" ), this, SLOT( slotWindowTimeline() ), CTRL+Key_K );
    window -> setItemVisible( id_window_timeline, false );
    id_window_render_camera_preview = window -> insertItem( icon_camera_preview, tr( "&Preview Render" ), this, SLOT( slotWindowRenderCameraPreview() ), CTRL+Key_R );
    window -> setItemVisible( id_window_render_camera_preview, false );
    id_window_top_camera_view = window -> insertItem( icon_top_camera, tr( "T&op Camera View" ), this, SLOT( slotWindowTopCameraView() ), CTRL+Key_Slash );
    window -> setItemVisible( id_window_top_camera_view, false );
    id_window_side_camera_view = window -> insertItem( icon_side_camera, tr( "Si&de Camera View" ), this, SLOT( slotWindowSideCameraView() ), CTRL+Key_Asterisk );
    window -> setItemVisible( id_window_side_camera_view, false );
    connect( window, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );

    //HELP MENU
    help = new QPopupMenu( this );
//     help -> setFont( QFont( "helvetica", 10 ) );
//     help -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    id_help = menuBar() -> insertItem( tr( "&Help" ), help );

    id_help_contents = help -> insertItem( tr( "&Contents" ), this, SLOT( slotContents() ), Key_F1 );
    help -> insertSeparator();
    id_help_about = help -> insertItem( tr( "&About K-Toon..." ), this, SLOT( slotAbout() ) );
    id_help_about_qt = help -> insertItem( tr( "About &Qt..." ), qApp, SLOT( aboutQt() ) );
    id_help_about_opengl = help -> insertItem( tr( "About &OpenGL..." ), this, SLOT( slotAboutOpenGL() ) );
    connect( help, SIGNAL( highlighted( int ) ), SLOT( slotStatusBarMessage( int ) ) );
}

void KToon::setupDialogs()
{
    //--------------- Dialog Boxes -----------------

    //For Illustration
    exposure_sheet_dialog = new ExposureSheet( this, Qt::WStyle_Tool, window, id_window_exposure_sheet, window_exposure_sheet );
    exposure_sheet_dialog -> show();
    list_of_es.append( exposure_sheet_dialog );

    tools_dialog = new Tools( this, Qt::WStyle_Tool, window, id_window_tools, window_tools );
    tools_dialog -> show();

    scenes_dialog = new Scenes( this, Qt::WStyle_Tool, window, id_window_scenes, window_scenes );
    scenes_dialog -> show();

    color_palette_dialog = new ColorPalette( this, Qt::WStyle_Tool, window, id_window_color_palette, window_color_palette );
    color_palette_dialog -> show();

    brushes_dialog = new Brushes( this, Qt::WStyle_Tool, window, id_window_brushes, window_brushes );
    brushes_dialog -> show();

#ifndef USE_QT
    library_dialog = new Library( this, Qt::WStyle_Tool, window, id_window_library, current_drawing_area, window_library );
#endif
    //For animation
    timeline_dialog = new Timeline( this, Qt::WStyle_Tool, window, id_window_timeline, window_timeline );
    list_of_tl.append( timeline_dialog );
#ifndef USE_QT
    render_camera_preview = new GLRenderCameraPreview( main_panel, this, window, id_window_render_camera_preview, window_render_camera_preview, current_drawing_area );
    render_camera_preview -> hide();

    top_camera_view = new GLTopCameraView( main_panel, this, window, id_window_top_camera_view, window_top_camera_view, current_drawing_area );
    top_camera_view -> hide();

    side_camera_view = new GLSideCameraView( main_panel, this, window, id_window_side_camera_view, window_side_camera_view, current_drawing_area );
    side_camera_view -> hide();
#endif
}

//-------------- DESTRUCTOR -----------------

KToon::~KToon()
{
    delete cursor_line;
    delete cursor_selection;
    delete cursor_nodes;
    delete cursor_brush;
    delete cursor_pencil;
    delete cursor_pen;
    delete cursor_rectangle;
    delete cursor_ellipse;
    delete cursor_eraser;
    delete cursor_slicer;
    delete cursor_fill;
    delete cursor_remove_fill;
    delete cursor_magnifying_glass;
    delete cursor_hand;
    delete cursor_dropper;
    delete cursor_contour_fill;
    delete file;
    delete open_recent;
    delete edit;
    delete view;
    delete grid;
    delete onion_skin;
    delete insert;
    delete tools;
    delete control;
    delete window;
    delete help;
    delete file_new;
    delete file_open;
    delete file_save;
    delete edit_undo;
    delete edit_redo;
    delete edit_cut;
    delete edit_copy;
    delete edit_paste;
    delete view_no_grid;
    delete view_grid12;
    delete view_grid16;
    delete view_subgrid;
    delete view_no_previous;
    delete view_previous;
    delete view_previous2;
    delete view_previous3;
    delete view_no_next;
    delete view_next;
    delete view_next2;
    delete view_next3;
    delete view_front_back_grid;
    delete view_ntsc;
    delete view_light_table;
    delete window_illustration;
    delete window_animation;
    delete window_tools;
    delete window_brushes;
    delete window_scenes;
    delete window_color_palette;
    delete window_exposure_sheet;
    delete window_library;
    delete window_drawing_area;
    delete window_timeline;
    delete window_render_camera_preview;
    delete window_top_camera_view;
    delete window_side_camera_view;
    delete main_panel;
    delete tool_bar;
    delete tool_bar2;
    delete current_status;
    delete es_default_color;
    delete es_use_color;
    delete es_selection_color;
    delete es_select_and_use_color;
    delete es_text_color_when_selected;
    delete es_default_text_color;
    delete es_lock_color;
    delete es_lock_and_select_color;
    delete es_has_drawing_color;
    delete tl_default_color;
    delete tl_selection_color;
    delete tl_special_color;
    delete tl_offset_color;
    delete tl_drag_offset_color;
    delete tl_border_color;
    delete tl_use_border_color;
    delete tl_motion_color;
    delete tl_drawing_color;
}

//------------------- PUBLIC MEMBERS ----------------

Status *KToon::currentStatus()
{
    Q_CHECK_PTR( current_status );
    return current_status;
}

Timeline *KToon::timeline()
{
    Q_CHECK_PTR( timeline_dialog );
    return timeline_dialog;
}

ExposureSheet *KToon::exposureSheet()
{
    Q_CHECK_PTR( exposure_sheet_dialog );
    return exposure_sheet_dialog;
}

Library *KToon::library()
{
    Q_CHECK_PTR( library_dialog );
    return library_dialog;
}

ColorPalette *KToon::colorPalette()
{
    Q_CHECK_PTR( color_palette_dialog );
    return color_palette_dialog;
}

Brushes *KToon::brushes()
{
    Q_CHECK_PTR( brushes_dialog );
    return brushes_dialog;
}

DrawingArea *KToon::drawingArea()
{
    Q_CHECK_PTR( current_drawing_area );
    return current_drawing_area;
}

GLRenderCameraPreview *KToon::renderCameraPreview()
{
    Q_CHECK_PTR( render_camera_preview );
    return render_camera_preview;
}

Scenes *KToon::scenes()
{
    Q_CHECK_PTR( scenes_dialog );
    return scenes_dialog;
}

Tools *KToon::toolsDialog()
{
    Q_CHECK_PTR( tools_dialog );
    return tools_dialog;
}

GLTopCameraView *KToon::topCameraView()
{
    Q_CHECK_PTR( top_camera_view );
    return top_camera_view;
}

GLSideCameraView *KToon::sideCameraView()
{
    Q_CHECK_PTR( side_camera_view );
    return side_camera_view;
}

Document *KToon::document()
{
    Q_CHECK_PTR( document_ );
    return document_;
}

void KToon::loadImage( const QString &file_name, bool from_load )
{
    if ( !from_load )
    {
       current_drawing_area -> loadImage( file_name );
       library_dialog -> loadImageSymbol( file_name );
    }
    statusBar() -> message( tr( "Image loaded successfully - " ) + file_name, 2000 );
}

void KToon::loadImageSequence( const QString &file_name, bool from_load )
{
    if ( !from_load )
       current_drawing_area -> loadImageSequence( file_name );
    statusBar() -> message( tr( "Image Sequence loaded successfully - " ) + file_name, 2000 );
}

void KToon::loadLibrary( const QString &file_name, bool from_load )
{
    QFile library_file( file_name );
    
    if ( !library_file.open( IO_ReadOnly ) )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Could not load library - The file might be moved or deleted - %1" ).arg( file_name ) );
	return;
    }
    QDomDocument library_doc( "_lbr_" + Document::getLocalName(file_name) );
    if ( !library_doc.setContent( &library_file ) )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Could not load library - Unrecognized .lbr file - %1" ).arg( file_name ) );
	library_file.close();
	return;
    }
    library_file.close();
    QDomElement library_tag = library_doc.documentElement();
    
    std::auto_ptr<LibraryData> ap_library(new LibraryData);
    LibraryData* library = ap_library.get();
    
    QDomElement items_tag = library_tag.firstChild().toElement();
    QDomNode n_item = items_tag.firstChild();
    QPtrList<Item> items;
    while ( !n_item.isNull() )
    {
	QDomElement item_tag = n_item.toElement();
	if ( item_tag.tagName() == "Symbol" )
	{
	    Symbol *symbol = new Symbol();
	    try { 
	    QString s_n = item_tag.attribute( "Name" );
	    symbol -> setName( s_n );
	    QDomElement graphic_tag = item_tag.firstChild().toElement();
	    symbol -> setGraphic( createGraphic( graphic_tag ) );
	    items.append( symbol );
	    }
	    catch(...)
	        {
		  delete symbol;
		  throw;
		  }
	}
	else if ( item_tag.tagName() == "Folder" )
	{
	    Folder *folder = createFolder( item_tag );
	    items.append( folder );
	}
	n_item = n_item.nextSibling();
    }
    library -> setItems( items );
    document_ -> setLibrary( ap_library.release() );
    
    if ( !from_load )
        library_dialog -> loadItems( items );

    statusBar() -> message( tr( "Library loaded successfully - " ) + file_name, 2000 );
}

void KToon::loadPalette( const QString &file_name, bool from_load )
{
    QFile palette_file( file_name );
    if ( !palette_file.open( IO_ReadOnly ) )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Could not load color palette - The file might be moved or deleted - %1" ).arg( file_name ) );
	return;
    }
    QDomDocument palette_doc( "_cpl_"+Document::getLocalName(file_name));
    if ( !palette_doc.setContent( &palette_file ) )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Could not load color palette - Unrecognized .cpl file - %1" ).arg( file_name ) );
	palette_file.close();
	return;
    }
    palette_file.close();
    QDomElement palette_tag = palette_doc.documentElement();
    
    std::auto_ptr<Palette> ap_palette(new Palette);
    Palette* palette = ap_palette.get();
    
    QDomNode n_color = palette_tag.firstChild();
    QPtrList<Color> colors;
    while ( !n_color.isNull() )
    {
	QDomElement color_tag = n_color.toElement();
	QString n = color_tag.attribute( "Name" );
	QString r = color_tag.attribute( "Red" );
	QString g = color_tag.attribute( "Green" );
	QString b = color_tag.attribute( "Blue" );
	QString a = color_tag.attribute( "Alpha" );
	Color *color = new Color( r.toFloat(), g.toFloat(), b.toFloat(), a.toFloat() );
	try {
	    color -> setNameColor( n );
	    colors.append( color );
	    }
	catch(...)
	    {
	    delete color;
	    throw;
	    }
	n_color = n_color.nextSibling();
    }
    palette -> setColors( colors );
    document_ -> setPalette( ap_palette.release() );
    if ( !from_load )
        color_palette_dialog -> loadCustomColors( colors );

    statusBar() -> message( tr( "Palette loaded successfully - " ) + file_name, 2000 );
}

void KToon::loadBrushes( const QString &file_name, bool from_load )
{
    QFile brushes_file( file_name );
    if ( !brushes_file.open( IO_ReadOnly ) )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Could not load the brushes - The file might be moved or deleted - %1" ).arg( file_name ) );
	return;
    }
    QDomDocument brushes_doc( "_bru_" + Document::getLocalName(file_name ) );
    if ( !brushes_doc.setContent( &brushes_file ) )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Could not load the brushes - Unrecognized .bru file - %1" ).arg( file_name ) );
	brushes_file.close();
	return;
    }
    brushes_file.close();
    QDomElement brushes_tag = brushes_doc.documentElement();
    QDomNode n_brush = brushes_tag.firstChild();
    QPtrList<Brush> brushes;
    while ( !n_brush.isNull() )
    {
	QDomElement brush_tag = n_brush.toElement();
	QString n = brush_tag.attribute( "Name" );
	QString mint = brush_tag.attribute( "MinThickness" );
	QString maxt = brush_tag.attribute( "MaxThickness" );
	QString s = brush_tag.attribute( "Smoothness" );
	QString i = brush_tag.attribute( "Id" );
	Brush *brush = new Brush( mint.toInt(), maxt.toInt(), s.toInt() );
	try {
	    brush -> setNameBrush( n );
	    brushes.append( brush );
	    }
	catch(...)
	    {
	    delete brush;
	    throw;
	    }
	n_brush = n_brush.nextSibling();
    }
    document_ -> setBrushes( brushes );
    if ( !from_load )
        brushes_dialog -> loadBrushes( brushes );

    statusBar() -> message( tr( "Brushes loaded successfully - " ) + file_name, 2000 );
}

void KToon::loadSound( const QString &file_name, bool from_load )
{
    if ( !from_load )
        timeline_dialog -> loadSound( file_name );
    statusBar() -> message( tr( "Sound loaded successfully - " ) + file_name, 2000 );
}

GLGraphicComponent *KToon::createGraphic( const QDomElement &graphic_tag )
{
    QString k = graphic_tag.attribute( "Kind" );
    QString id = graphic_tag.attribute( "Id" );
    QString fi = graphic_tag.attribute( "Fill" );
    QString p = graphic_tag.attribute( "Pattern" );
    QString fa = graphic_tag.attribute( "Factor" );
    QString z = graphic_tag.attribute( "Z" );
    QString ang = graphic_tag.attribute( "Angle" );
    QString xt = graphic_tag.attribute( "XTranslate" );
    QString yt = graphic_tag.attribute( "YTranslate" );

    //Origin Tag
    QDomElement origin_tag = graphic_tag.firstChild().toElement();
    QString ox = origin_tag.attribute( "X" );
    QString oy = origin_tag.attribute( "Y" );
    QPoint origin = QPoint( ox.toInt(), oy.toInt() );

    //Outline Color Tag
    QDomElement outline_color_tag = origin_tag.nextSibling().toElement();
    QString no = outline_color_tag.attribute( "Name" );
    QString ro = outline_color_tag.attribute( "Red" );
    QString go = outline_color_tag.attribute( "Green" );
    QString bo = outline_color_tag.attribute( "Blue" );
    QString ao = outline_color_tag.attribute( "Alpha" );
    
    Color outline_color = Color( ro.toFloat(), go.toFloat(), bo.toFloat(), ao.toFloat() );
    outline_color.setNameColor( no );

    //Width Tag
    QDomElement width_tag = outline_color_tag.nextSibling().toElement();
    QString nw = width_tag.attribute( "Name" );
    QString mintw = width_tag.attribute( "MinThickness" );
    QString maxtw = width_tag.attribute( "MaxThickness" );
    QString sw = width_tag.attribute( "Smoothness" );
    QString idw = width_tag.attribute( "Id" );
    
    Brush width = Brush( mintw.toInt(), maxtw.toInt(), sw.toInt() );
    width.setNameBrush( nw );

    int kg = k.toInt();
    switch ( kg )
    {
	case GLGraphicComponent::GC_BRUSH:
	{
	    //End Tag
	    QDomElement end_tag = width_tag.nextSibling().toElement();
    	    QString ex = end_tag.attribute( "X" );
    	    QString ey = end_tag.attribute( "Y" );
    	    QPoint end = QPoint( ex.toInt(), ey.toInt() );

	    QDomNode n_point = end_tag.nextSibling();
	    QPtrList<QPoint> points;
	    while ( !n_point.isNull() )
	    {
	        //Point Tag
		QDomElement point_tag = n_point.toElement();
		QString _x = point_tag.attribute( "X" );
		QString _y = point_tag.attribute( "Y" );
		QPoint *point = new QPoint( _x.toInt(), _y.toInt() );
		try {
		    points.append( point );
		    }
		catch(...)
		    {
		    delete point;
		    throw;
		    }
		n_point = n_point.nextSibling();
	    }

	    GLBrush *brush = new GLBrush( current_drawing_area, origin, outline_color, width, end );
	    try {
	        brush -> setFill( fi == "true" );
	        brush -> setStipplePattern( p.toUInt() );
	        brush -> setStippleFactor( fa.toUInt() );
	        brush -> setZ( z.toInt() );
	        brush -> setPoints( points );
	        brush -> calculateBottomRight();
	        brush -> calculateTopLeft();
	        brush -> setRotationAngle( ang.toFloat() );
	        brush -> translateGraphic( xt.toInt(), yt.toInt() );
	        }
	    catch(...)
	        {
	        delete brush;
	        throw;
	        }

	    return brush;
	    break;
	}
	case GLGraphicComponent::GC_PENCIL:
	{
	    //End Tag
	    QDomElement end_tag = width_tag.nextSibling().toElement();
    	    QString ex = end_tag.attribute( "X" );
    	    QString ey = end_tag.attribute( "Y" );
    	    QPoint end = QPoint( ex.toInt(), ey.toInt() );

	    QDomNode n_point = end_tag.nextSibling();
	    QPtrList<QPoint> points;
	    while ( !n_point.isNull() )
	    {
	        //Point Tag
		QDomElement point_tag = n_point.toElement();
		QString _x = point_tag.attribute( "X" );
		QString _y = point_tag.attribute( "Y" );
		QPoint *point = new QPoint( _x.toInt(), _y.toInt() );
		try {
		    points.append( point );
		    }
		catch(...)
		    {
		    delete point;
		    throw;
		    }
		n_point = n_point.nextSibling();
	    }

	    GLPencil *pencil = new GLPencil( current_drawing_area, origin, outline_color, width, end );
	    try {
	        pencil -> setFill( fi == "true" );
	        pencil -> setStipplePattern( p.toUInt() );
	        pencil -> setStippleFactor( fa.toUInt() );
	        pencil -> setZ( z.toInt() );
	        pencil -> setPoints( points );
	        pencil -> calculateBottomRight();
	        pencil -> calculateTopLeft();
	        pencil -> setRotationAngle( ang.toFloat() );
	        pencil -> translateGraphic( xt.toInt(), yt.toInt() );
	        }
	    catch(...)
	        {
	        delete pencil;
	        throw;
	        }
	    
	    return pencil;
	    break;
	}
	case GLGraphicComponent::GC_PEN:
	{
	    QString ord = graphic_tag.attribute( "Order" );
	    QString str = graphic_tag.attribute( "Stride" );
	    QString knt = graphic_tag.attribute( "Knots" );
	    QString ctr = graphic_tag.attribute( "Control" );

	    //End Tag
	    QDomElement end_tag = width_tag.nextSibling().toElement();
    	    QString ex = end_tag.attribute( "X" );
    	    QString ey = end_tag.attribute( "Y" );
    	    QPoint end = QPoint( ex.toInt(), ey.toInt() );

	    //Points Tag
	    QDomElement points_tag = end_tag.nextSibling().toElement();
	    QDomNode n_point = points_tag.firstChild();
	    QPtrList<QPoint> points;
	    while ( !n_point.isNull() )
	    {
	        //Point Tag
		QDomElement point_tag = n_point.toElement();
		QString _x = point_tag.attribute( "X" );
		QString _y = point_tag.attribute( "Y" );
		QPoint *point = new QPoint( _x.toInt(), _y.toInt() );
		try {
		    points.append( point );
		    }
		catch(...)
		    {
		    delete point;
		    throw;
		    }
		n_point = n_point.nextSibling();
	    }

	    //Beziers Tag
	    QDomElement beziers_tag = points_tag.nextSibling().toElement();
	    QDomNode n_bezier = beziers_tag.firstChild();
	    QPtrList<QPoint> beziers;
	    while ( !n_bezier.isNull() )
	    {
	        //Bezier Tag
		QDomElement bezier_tag = n_bezier.toElement();
		QString _x = bezier_tag.attribute( "X" );
		QString _y = bezier_tag.attribute( "Y" );
		QPoint *bezier = new QPoint( _x.toInt(), _y.toInt() );
		try {
		    beziers.append( bezier );
		    }
		catch(...)
		    {
		    delete bezier;
		    throw;
		    }
		n_bezier = n_bezier.nextSibling();
	    }

	    GLPen *pen = new GLPen( current_drawing_area, origin, outline_color, width, end );
	    try {
	        pen -> setFill( fi == "true" );
	        pen -> setStipplePattern( p.toUInt() );
	        pen -> setStippleFactor( fa.toUInt() );
	        pen -> setZ( z.toInt() );
	        pen -> setPoints( points );
	        pen -> setBezier( beziers );
	        pen -> setOrderPen( ord.toInt() );
	        pen -> setStridePen( str.toInt() );
	        pen -> setKnotsCount( knt.toInt() );
	        pen -> setControlPointsPen( ctr.toInt() );
	        pen -> calculateBottomRight();
	        pen -> calculateTopLeft();
	        pen -> setRotationAngle( ang.toFloat() );
	        pen -> translateGraphic( xt.toInt(), yt.toInt() );
	        }
	    catch(...)
	        {
	        delete pen;
	        throw;
	        }
	    
	    return pen;
	    break;
	}
	case GLGraphicComponent::GC_LINE:
	{
	    //End Tag
	    QDomElement end_tag = width_tag.nextSibling().toElement();
    	    QString ex = end_tag.attribute( "X" );
    	    QString ey = end_tag.attribute( "Y" );
    	    QPoint end = QPoint( ex.toInt(), ey.toInt() );

	    GLLine *line = new GLLine( current_drawing_area, origin, outline_color, width, end );
	    try {
	        line -> setFill( fi == "true" );
	        line -> setStipplePattern( p.toUInt() );
	        line -> setStippleFactor( fa.toUInt() );
	        line -> setZ( z.toInt() );
	        line -> calculateBottomRight();
	        line -> calculateTopLeft();
	        line -> setRotationAngle( ang.toFloat() );
	        line -> translateGraphic( xt.toInt(), yt.toInt() );
	        }
	    catch(...)
	        {
		  delete line;
		  throw;
		  }
	    
	    return line;
	    break;
	}
	case GLGraphicComponent::GC_RECTANGLE:
	{
	    QString pe = graphic_tag.attribute( "Perfect" );

	    //End Tag
	    QDomElement end_tag = width_tag.nextSibling().toElement();
    	    QString ex = end_tag.attribute( "X" );
    	    QString ey = end_tag.attribute( "Y" );
    	    QPoint end = QPoint( ex.toInt(), ey.toInt() );

	    //Fill Color Tag
    	    QDomElement fill_color_tag = end_tag.nextSibling().toElement();
    	    QString nf = fill_color_tag.attribute( "Name" );
    	    QString rf = fill_color_tag.attribute( "Red" );
    	    QString gf = fill_color_tag.attribute( "Green" );
    	    QString bf = fill_color_tag.attribute( "Blue" );
    	    QString af = fill_color_tag.attribute( "Alpha" );
    	    
	    Color fill_color = Color( rf.toFloat(), gf.toFloat(), bf.toFloat(), af.toFloat() );
    	    fill_color.setNameColor( nf );

	    GLRectangle *rectangle = new GLRectangle( current_drawing_area, origin, outline_color, width, end, fill_color );
	    try {
	        rectangle -> setFill( fi == "true" );
	        rectangle -> setStipplePattern( p.toUInt() );
	        rectangle -> setStippleFactor( fa.toUInt() );
	        rectangle -> setPerfectRectangle( pe == "true" );
	        rectangle -> setZ( z.toInt() );
	        rectangle -> calculateBottomRight();
	        rectangle -> calculateTopLeft();
	        rectangle -> setRotationAngle( ang.toFloat() );
	        rectangle -> translateGraphic( xt.toInt(), yt.toInt() );
		  }
	    catch(...)
	        {
		  delete rectangle;
		  throw;
		  }

	    return rectangle;
	    break;
	}
	case GLGraphicComponent::GC_ELLIPSE:
	{
	    QString pe = graphic_tag.attribute( "Perfect" );

	    //Radius Tag
	    QDomElement radius_tag = width_tag.nextSibling().toElement();
    	    QString rx = radius_tag.attribute( "X" );
    	    QString ry = radius_tag.attribute( "Y" );
    	    QPoint radius = QPoint( rx.toInt(), ry.toInt() );

            //Fill Color Tag
    	    QDomElement fill_color_tag = radius_tag.nextSibling().toElement();
    	    QString nf = fill_color_tag.attribute( "Name" );
    	    QString rf = fill_color_tag.attribute( "Red" );
    	    QString gf = fill_color_tag.attribute( "Green" );
    	    QString bf = fill_color_tag.attribute( "Blue" );
    	    QString af = fill_color_tag.attribute( "Alpha" );
    	    
	    Color fill_color = Color( rf.toFloat(), gf.toFloat(), bf.toFloat(), af.toFloat() );
    	    fill_color.setNameColor( nf );

	    GLEllipse *ellipse = new GLEllipse( current_drawing_area, origin, outline_color, width, radius, fill_color );
	    try {
	        ellipse -> setFill( fi == "true" );
	        ellipse -> setStipplePattern( p.toUInt() );
	        ellipse -> setStippleFactor( fa.toUInt() );
	        ellipse -> setPerfectEllipse( pe == "true" );
	        ellipse -> setZ( z.toInt() );
	        ellipse -> calculateBottomRight();
	        ellipse -> calculateTopLeft();
	        ellipse -> setRotationAngle( ang.toFloat() );
	        ellipse -> translateGraphic( xt.toInt(), yt.toInt() );
	        }
	    catch(...)
	        {
		  delete ellipse;
		  throw;
		  }
	
	    return ellipse;
	    break;
	}
	default: break;
    }
    return NULL;
}

Folder *KToon::createFolder( const QDomElement &tag )
{
    
    std::auto_ptr<Folder> ap_folder(new Folder);
    Folder* folder = ap_folder.get();
    
    folder -> setName( tag.attribute( "Name" ) );
    QDomElement items_tag = tag.firstChild().toElement();
    QPtrList<Item> items;
    QDomNode n_item = items_tag.firstChild();
    while ( !n_item.isNull() )
    {
	QDomElement item_tag = n_item.toElement();
	if ( item_tag.tagName() == "Symbol" )
	{
	    Symbol *symbol = new Symbol();
	    try {
	        QString s_n = item_tag.attribute( "Name" );
	        symbol -> setName( s_n );
	        QDomElement graphic_tag = item_tag.firstChild().toElement();
	        symbol -> setGraphic( createGraphic( graphic_tag ) );
	        items.append( symbol );
	        }
	    catch(...)
	        {
		  delete symbol;
		  throw;
		  } 
	}
	else if ( item_tag.tagName() == "Folder" )
	{
	    Folder *folder = createFolder( item_tag );
	    items.append( folder );
	}
	n_item = n_item.nextSibling();
    }
    folder -> setItems( items );
    Q_CHECK_PTR( folder );
    return ap_folder.release();
}

//---------- PUBLIC MEMBERS FOR EXPOSURE SHEET AND TIMELINE ----------------

QColor *KToon::esDefaultColor()
{
    Q_CHECK_PTR( es_default_color );
    return es_default_color;
}

QColor *KToon::esUseColor()
{
    Q_CHECK_PTR( es_use_color );
    return es_use_color;
}

QColor *KToon::esSelectionColor()
{
    Q_CHECK_PTR( es_selection_color );
    return es_selection_color;
}

QColor *KToon::esSelectAndUseColor()
{
    Q_CHECK_PTR( es_select_and_use_color );
    return es_select_and_use_color;
}

QColor *KToon::esTextColorWhenSelected()
{
    Q_CHECK_PTR( es_text_color_when_selected );
    return es_text_color_when_selected;
}

QColor *KToon::esDefaultTextColor()
{
    Q_CHECK_PTR( es_default_text_color );
    return es_default_text_color;
}

QColor *KToon::esLockColor()
{
    Q_CHECK_PTR( es_lock_color );
    return es_lock_color;
}

QColor *KToon::esLockAndSelectColor()
{
    Q_CHECK_PTR( es_lock_and_select_color );
    return es_lock_and_select_color;
}

QColor *KToon::esHasDrawingColor()
{
    Q_CHECK_PTR( es_has_drawing_color );
    return es_has_drawing_color;
}

QColor *KToon::tlDefaultColor()
{
    Q_CHECK_PTR( tl_default_color );
    return tl_default_color;
}

QColor *KToon::tlSelectionColor()
{
    Q_CHECK_PTR( tl_selection_color );
    return tl_selection_color;
}

QColor *KToon::tlSpecialColor()
{
    Q_CHECK_PTR( tl_special_color );
    return tl_special_color;
}

QColor *KToon::tlOffsetColor()
{
    Q_CHECK_PTR( tl_offset_color );
    return tl_offset_color;
}

QColor *KToon::tlDragOffsetColor()
{
    Q_CHECK_PTR( tl_drag_offset_color );
    return tl_drag_offset_color;
}

QColor *KToon::tlBorderColor()
{
    Q_CHECK_PTR( tl_border_color );
    return tl_border_color;
}

QColor *KToon::tlUseBorderColor()
{
    Q_CHECK_PTR( tl_use_border_color );
    return tl_use_border_color;
}

QColor *KToon::tlMotionColor()
{
    Q_CHECK_PTR( tl_motion_color );
    return tl_motion_color;
}

QColor *KToon::tlDrawingColor()
{
    Q_CHECK_PTR( tl_drawing_color );
    return tl_drawing_color;
}

//------------------- SLOTS AND FUNCTIONS FILE MENU ----------------------------

void KToon::slotNewDocument()
{
    QString document_number;
    document_max_value++;
    document_number.setNum( document_max_value );

    bool closed = true;
    if ( current_drawing_area != NULL )
        closed = current_drawing_area -> close( true );

    if ( closed )
    {
        file_name = "";
	  
	//VL: What happens with old document_? Is it deleted somewhere?	
	//murakumo: Volker, it is deleted into slotCloseDrawingArea() that is called if it was accepted
	//          the drawing area's close event (close == true)
    	document_ = new Document();
    	document_ -> setNameDocument( tr( "Document" ) + document_number );
	setCaption( tr( "Document" ) + document_number );

    	current_status -> setCurrentScene( ( document_ -> getAnimation() -> getScenes() ).first() );
    	current_status -> setCurrentLayer( ( current_status -> currentScene() -> getLayers() ).first() );
    	current_status -> setCurrentKeyFrame( NULL );
    	QPtrList<KeyFrame> empty;
    	current_status -> setRenderKeyframes( empty );

    	current_drawing_area = new DrawingArea( main_panel, this, tr( "Document" ) + document_number );
    	exposure_sheet_dialog = new ExposureSheet( this, Qt::WStyle_Tool, window, id_window_exposure_sheet, window_exposure_sheet );
    	scenes_dialog = new Scenes( this, Qt::WStyle_Tool, window, id_window_scenes, window_scenes );
    	timeline_dialog = new Timeline( this, Qt::WStyle_Tool, window, id_window_timeline, window_timeline );
    	brushes_dialog = new Brushes( this, Qt::WStyle_Tool, window, id_window_brushes, window_brushes );
    	library_dialog = new Library( this, Qt::WStyle_Tool, window, id_window_library, current_drawing_area, window_library );
    	render_camera_preview = new GLRenderCameraPreview( main_panel, this, window, id_window_render_camera_preview, window_render_camera_preview, current_drawing_area );
    	top_camera_view = new GLTopCameraView( main_panel, this, window, id_window_top_camera_view, window_top_camera_view, current_drawing_area );
    	side_camera_view = new GLSideCameraView( main_panel, this, window, id_window_side_camera_view, window_side_camera_view, current_drawing_area );

    	list_of_es.append( exposure_sheet_dialog );
    	list_of_tl.append( timeline_dialog );

    	if ( window -> isItemEnabled( id_window_animation ) )
    	{
        	current_drawing_area -> show();
        	exposure_sheet_dialog -> show();
	        scenes_dialog -> show();
        	tools_dialog -> show();
		brushes_dialog -> show();
    	}
	else if ( window -> isItemEnabled( id_window_illustration ) )
    	{
        	timeline_dialog -> show();
		render_camera_preview -> show();
		top_camera_view -> show();
		side_camera_view -> show();
    	}

    	current_drawing_area -> setCursor( QCursor( Qt::ForbiddenCursor ) );

	    //------------- Main Connections Again ----------------

    	connect( exposure_sheet_dialog, SIGNAL( layerInserted() ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotInsertLayer() ) );
    	connect( exposure_sheet_dialog, SIGNAL( layerInserted() ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotInsertFrameSequence() ) );
    	connect( exposure_sheet_dialog, SIGNAL( layerRemoved() ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotRemoveLayer() ) );
    	connect( exposure_sheet_dialog, SIGNAL( layerRemoved() ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotRemoveFrameSequence() ) );
    	connect( exposure_sheet_dialog, SIGNAL( layerRemoved() ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotUpdateMaxUsedFrames() ) );
    	connect( exposure_sheet_dialog, SIGNAL( layerSelected( int ) ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotSelectLayerFromES( int ) ) );
    	connect( exposure_sheet_dialog, SIGNAL( layerRenamed( int, const QString & ) ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotRenameLayerFromES( int, const QString & ) ) );
    	connect( exposure_sheet_dialog, SIGNAL( framesInsertedAtTheEnd( int ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotInsertKeyframesIntoTheCurrentFS( int ) ) );
    	connect( exposure_sheet_dialog, SIGNAL( frameRemoved( int ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotRemoveKeyframeBlockFromTheCurrentFS( int ) ) );
    	connect( exposure_sheet_dialog, SIGNAL( frameMovedUp( int ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotMoveKeyframeLeftInTheCurrentFS( int ) ) );
    	connect( exposure_sheet_dialog, SIGNAL( frameMovedDown( int ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotMoveKeyframeRightInTheCurrentFS( int ) ) );
    	connect( exposure_sheet_dialog, SIGNAL( frameSelected() ), current_drawing_area, SLOT( slotSelectFrame() ) );
    	connect( exposure_sheet_dialog, SIGNAL( frameSelected() ), SLOT( slotActivateCursor() ) );

    	connect( timeline_dialog, SIGNAL( insertLayerClicked() ), exposure_sheet_dialog, SLOT( slotInsertLayer() ) );
    	connect( timeline_dialog, SIGNAL( removeLayerClicked() ), exposure_sheet_dialog, SLOT( slotRemoveLayer() ) );
    	connect( timeline_dialog -> layerManager() -> layerSequence(), SIGNAL( layerSelectedToES( int ) ), exposure_sheet_dialog, SLOT( slotSelectLayerFromTL( int ) ) );
    	connect( timeline_dialog -> layerManager() -> layerSequence(), SIGNAL( layerRenamed( int, const QString & ) ), exposure_sheet_dialog, SLOT( slotRenameLayerFromTL( int, const QString & ) ) );
    	connect( timeline_dialog -> frameSequenceManager() -> frameLayout(), SIGNAL( keyframeRemovedToES( int ) ), exposure_sheet_dialog, SLOT( slotRemoveFrameFromTL( int ) ) );
    	connect( timeline_dialog, SIGNAL( layerMovedUp() ), exposure_sheet_dialog, SLOT( slotMoveLayerLeft() ) );
    	connect( timeline_dialog, SIGNAL( layerMovedDown() ), exposure_sheet_dialog, SLOT( slotMoveLayerRight() ) );
    	connect( timeline_dialog -> layerManager() -> layerSequence(), SIGNAL( layerReleasedAbove( int, int ) ), exposure_sheet_dialog, SLOT( slotSwapWithLeftLayer( int, int ) ) );
    	connect( timeline_dialog -> layerManager() -> layerSequence(), SIGNAL( layerReleasedBelow( int, int ) ), exposure_sheet_dialog, SLOT( slotSwapWithRightLayer( int, int ) ) );
    	connect( timeline_dialog -> frameSequenceManager() -> frameLayout(), SIGNAL( motionTweenCreatedToES( int ) ), exposure_sheet_dialog, SLOT( slotCreateMotionTween( int ) ) );
    	connect( timeline_dialog -> frameSequenceManager() -> frameLayout(), SIGNAL( motionTweenRemovedToES( int ) ), exposure_sheet_dialog, SLOT( slotRemoveMotionTween( int ) ) );

    	insert -> connectItem( id_insert_scene, scenes_dialog, SLOT( slotInsertScene() ) );
    	insert -> connectItem( id_insert_remove_scene, scenes_dialog, SLOT( slotRemoveScene() ) );
    	file -> connectItem( id_file_close, current_drawing_area, SLOT( close() ) );
    	connect( current_drawing_area, SIGNAL( closed() ), SLOT( slotCloseDrawingArea() ) );

    	connect( color_palette_dialog, SIGNAL( outlineColorChanged() ), current_drawing_area, SLOT( slotChangeOutlineColor() ) );
    	connect( color_palette_dialog, SIGNAL( fillColorChanged() ), current_drawing_area, SLOT( slotChangeFillColor() ) );
    	connect( brushes_dialog, SIGNAL( minThicknessChanged() ), current_drawing_area, SLOT( slotChangeMinThicknessBrush() ) );
    	connect( brushes_dialog, SIGNAL( maxThicknessChanged() ), current_drawing_area, SLOT( slotChangeMaxThicknessBrush() ) );
    	connect( brushes_dialog, SIGNAL( smoothnessChanged() ), current_drawing_area, SLOT( slotChangeSmoothnessBrush() ) );

    	connect( scenes_dialog, SIGNAL( sceneInserted() ), SLOT( slotInsertSync() ) );
    	connect( scenes_dialog, SIGNAL( sceneMovedDown( int ) ), SLOT( slotMoveDownSync( int ) ) );
    	connect( scenes_dialog, SIGNAL( sceneMovedUp( int ) ), SLOT( slotMoveUpSync( int ) ) );
    	connect( scenes_dialog, SIGNAL( sceneRemoved( int ) ), SLOT( slotRemoveSync( int ) ) );
    	connect( scenes_dialog, SIGNAL( sceneSelected( int ) ), SLOT( slotSelectSync( int ) ) );

	connect( current_drawing_area, SIGNAL( colorGrabbed( Color::Color * ) ), color_palette_dialog, SLOT( slotSetColor( Color::Color * ) ) );
    	connect( current_drawing_area, SIGNAL( updated() ), library_dialog -> getSymbolView(), SLOT( updateGL() ) );
    	connect( current_drawing_area, SIGNAL( wasDrawn( bool ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotFrameHasDrawing( bool ) ) );

    	scenes_dialog -> selectFirstScene();
    	exposure_sheet_dialog -> touchFirstFrame();

    	file -> setItemEnabled( id_file_save, true );
    	file -> setItemEnabled( id_file_save_as, true );
	file -> setItemEnabled( id_file_close, true );
	file -> setItemEnabled( id_file_import, true );
	file -> setItemEnabled( id_file_export, true );
	file -> setItemEnabled( id_file_properties, true );

    	menuBar() -> setItemVisible( id_insert, true );
    	if ( window -> isItemEnabled( id_window_animation ) )
    	{
    		menuBar() -> setItemVisible( id_edit, true );
    		menuBar() -> setItemVisible( id_view, true );
    		menuBar() -> setItemVisible( id_tools, true );
    		window -> setItemVisible( id_window_exposure_sheet, true );
    		window -> setItemVisible( id_window_scenes, true );
    		window -> setItemVisible( id_window_tools, true );
    		window -> setItemVisible( id_window_brushes, true );
    		window -> setItemVisible( id_window_library, true );
		window -> setItemVisible( id_window_drawing_area, true );

		window_drawing_area -> show();
		window_exposure_sheet -> show();
		window_tools -> show();
		window_brushes -> show();
		window_scenes -> show();
		window_library -> show();

    		edit_undo -> show();
    		edit_redo -> show();
    		edit_cut -> show();
    		edit_copy -> show();
    		edit_paste -> show();
		tool_bar2 -> show();
    	}
    	else
	{
        	menuBar() -> setItemVisible( id_control, true );
    		window -> setItemVisible( id_window_timeline, true );
    		window -> setItemVisible( id_window_render_camera_preview, true );
    		window -> setItemVisible( id_window_top_camera_view, true );
    		window -> setItemVisible( id_window_side_camera_view, true );

		window_timeline -> show();
		window_render_camera_preview -> show();
		window_top_camera_view -> show();
		window_side_camera_view -> show();
	}
    	file_save -> show();

    	color_palette_dialog -> enableCustomPalette( true );
    	color_palette_dialog -> clearCustomPalette();

	slotSeeIllustration();

    	statusBar() -> message( tr( "Ready." ), 2000 );
    }
}

void KToon::slotChoose()
{
	QString fn = QFileDialog::getOpenFileName( KTOON_REPOSITORY, "KToon Project ( *.ktn )", this );
	if ( !fn.isEmpty() )
	{
		bool is_recent = false;
		for ( QStringList::Iterator it = recent_names.begin(); it != recent_names.end(); ++it )
		{
			if ( *it == fn )
			{
				is_recent = true;
				it = recent_names.remove( it );
				--it;
			}
		}
		if ( !is_recent )
			recent_names.pop_back();
		recent_names.prepend( fn );
		updateOpenRecentMenu();
		slotLoadDocument( fn );
	}
}

void KToon::slotLoadDocument( const QString &in_file_name )
{
    QFile f( in_file_name );
    if ( !f.open( IO_ReadOnly ) )
	return;

    QDomDocument xml_doc( "KTProject"+ktapp->getVersion() );
    
    QString parseerror = "";
    int errorLine = 0;
    
    if ( !xml_doc.setContent( &f, &parseerror, &errorLine ) )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Unrecognized file format - %1" ).arg( in_file_name ) );
        statusBar() -> message( tr( "Unrecognized file format - %1" ).arg( in_file_name ), 2000 );
	f.close();

	std::cout << "The error was: " << parseerror << " in the line: " << errorLine << std::endl; 
	
	return;
    }
    f.close();

    bool closed = true;
    if ( current_drawing_area != NULL )
        closed = current_drawing_area -> close( true );

    //----------- Load all the file data into the storing classes created for that purpose ------------

    if ( closed ) {

    //1. Document Tag
    QDomElement root = xml_doc.documentElement();
    
    //VL: old document_ ?
    //murakumo: See the comment above (into slotNewDocument()).
    document_ = new Document();
    document_ -> setNameDocument( in_file_name );

    //Drawing Area Initialization
    current_drawing_area = new DrawingArea( main_panel, this, document_ -> nameDocument() );

    //1.1. Palette Tag
    QDomElement palette_path_tag = root.firstChild().toElement();
    QString palette_path = KTOON_REPOSITORY + "/"+palette_path_tag.attribute( "PATH" );
    loadPalette( /*Document::turnUnderscoresIntoSlashes(*/ palette_path /*)*/, true );

    //1.2. Brushes Tag
    QDomElement brushes_path_tag = palette_path_tag.nextSibling().toElement();
    QString brushes_path = KTOON_REPOSITORY + "/"+brushes_path_tag.attribute( "PATH" );
    loadBrushes( /*Document::turnUnderscoresIntoSlashes(*/ brushes_path, true );

    //1.2A. Library Tag
    QDomElement library_path_tag = brushes_path_tag.nextSibling().toElement();
    QString library_path = KTOON_REPOSITORY + "/"+ library_path_tag.attribute( "PATH" );
    loadLibrary( library_path, true );

    //1.3. Animation Tag
    QDomElement animation_tag = library_path_tag.nextSibling().toElement();
    std::auto_ptr<Animation> ap_animation(new Animation);
    Animation* animation = ap_animation.get();
    
    QString fr = animation_tag.attribute( "FrameRate" );
    QString cw = animation_tag.attribute( "CameraWidth" );
    QString cl = animation_tag.attribute( "CameraLength" );
    animation -> setFrameRate( fr.toInt() );
    animation -> setCameraWidth( cw.toInt() );
    animation -> setCameraLength( cl.toInt() );

    //1.3.1. Scenes Tag
    QDomElement scenes_tag = animation_tag.firstChild().toElement();
    QDomNode n_scene = scenes_tag.firstChild();
    QPtrList<Scene> scenes;
    while ( !n_scene.isNull() )
    {
        //1.3.1.(1..*). Scene Tag
	QDomElement scene_tag = n_scene.toElement();
	QString n = scene_tag.attribute( "Name" );
	
	std::auto_ptr<Scene> ap_scene(new Scene);
	Scene* scene = ap_scene.get();
	
	scene -> setNameScene( n );

	//1.3.1.(1..*).1. Layers Tag
	QDomElement layers_tag = scene_tag.firstChild().toElement();
	QDomNode n_layer = layers_tag.firstChild();
	QPtrList<Layer> layers;
	while ( !n_layer.isNull() )
	{
	    Layer* layer = 0;
	    KeyFrame* keyframe = 0;
	    Camera* camera = 0;
	    GLDrawing* drawing = 0;
	    
	    try {
	    //1.3.1.(1..*).1.(1..*). Layer Tag
	    QDomElement layer_tag = n_layer.toElement();
	    QString id = layer_tag.attribute( "Id" );
	    QString n = layer_tag.attribute( "Name" );
	    layer = new Layer();
	    layer -> setIndexLayer( id.toInt() );
	    layer -> setNameLayer( n );

	    //1.3.1.(1..*).1.(1..*).1. Keyframes Tag
	    QDomElement keyframes_tag = layer_tag.firstChild().toElement();
	    QDomNode n_keyframe = keyframes_tag.firstChild();
	    QPtrList<KeyFrame> keyframes;
	    while ( !n_keyframe.isNull() )
	    {
		//1.3.1.(1..*).1.(1..*).1.(1..*). Keyframe Tag
		QDomElement keyframe_tag = n_keyframe.toElement();
		QString n = keyframe_tag.attribute( "Name" );
		QString o = keyframe_tag.attribute( "Offset" );
		QString l = keyframe_tag.attribute( "Length" );
		QString m = keyframe_tag.attribute( "Motion" );
		
		keyframe = new KeyFrame();
		
		keyframe -> setNameKeyFrame( n );
		keyframe -> setOffsetKeyFrame( o.toInt() );
		keyframe -> setLengthKeyFrame( l.toInt() );
		keyframe -> setMotionKeyFrame( m == "true" );

		//1.3.1.(1..*).1.(1..*).1.(1..*).1. Camera Tag
		QDomElement camera_tag = keyframe_tag.firstChild().toElement();
		camera = new Camera();

		//1.3.1.(1..*).1.(1..*).1.(1..*).1.1. Position Tag
		QDomElement position_tag = camera_tag.firstChild().toElement();

		//1.3.1.(1..*).1.(1..*).1.(1..*).1.1.1. Position Point Tag
		QDomElement position_point_tag = position_tag.firstChild().toElement();
		QString px = position_point_tag.attribute( "X" );
		QString py = position_point_tag.attribute( "Y" );
		QString pz = position_point_tag.attribute( "Z" );
		Point3D position_point = Point3D( px.toInt(), py.toInt(), pz.toInt() );

		//1.3.1.(1..*).1.(1..*).1.(1..*).1.2. Center Tag
		QDomElement center_tag = position_tag.nextSibling().toElement();

		//1.3.1.(1..*).1.(1..*).1.(1..*).1.2.1. Center Point Tag
		QDomElement center_point_tag = center_tag.firstChild().toElement();
		QString cx = center_point_tag.attribute( "X" );
		QString cy = center_point_tag.attribute( "Y" );
		QString cz = center_point_tag.attribute( "Z" );
		Point3D center_point = Point3D( cx.toInt(), cy.toInt(), cz.toInt() );
		
		//1.3.1.(1..*).1.(1..*).1.(1..*).1.3. Up Tag
		QDomElement up_tag = center_tag.nextSibling().toElement();

		//1.3.1.(1..*).1.(1..*).1.(1..*).1.3.1. Up Point Tag
		QDomElement up_point_tag = up_tag.firstChild().toElement();
		QString ux = up_point_tag.attribute( "X" );
		QString uy = up_point_tag.attribute( "Y" );
		QString uz = up_point_tag.attribute( "Z" );
		Point3D up_point = Point3D( ux.toInt(), uy.toInt(), uz.toInt() );

		camera -> setPositionCamera( position_point );
		camera -> setCenterCamera( center_point );
		camera -> setUpCamera( up_point );

		//1.3.1.(1..*).1.(1..*).1.(1..*).2. Drawing Tag
		QDomElement drawing_tag = camera_tag.nextSibling().toElement();
		drawing = new GLDrawing();
		
		QDomNode n_graphic = drawing_tag.firstChild();
		QPtrList<GLGraphicComponent> graphics;
		while ( !n_graphic.isNull() )
		{
		    //1.3.1.(1..*).1.(1..*).1.(1..*).2.(1..*). Graphic Tag
		    QDomElement graphic_tag = n_graphic.toElement();
		    graphics.append( createGraphic( graphic_tag ) );
		    n_graphic = n_graphic.nextSibling();
		}
		drawing -> setGraphicComponents( graphics );

		keyframe -> setCamera( camera );
		camera = 0;
		
		keyframe -> setDrawing( drawing );
		drawing = 0;

		keyframes.append( keyframe );
		keyframe = 0;
		
		n_keyframe = n_keyframe.nextSibling();
	     }
	     layer -> setKeyFrames( keyframes );

	     layers.append( layer );
	    } // try
	    catch(...)
	        {
		  delete layer;
		  
		  // keyframe, camera and drawing are zeroed out after 
		  // ownership of them is took by another object, so it
		  // is safe to call delete on all of them here.
		  delete keyframe;
		  delete camera ;
		  delete drawing;
		  
		  throw;
		  }
	    
	    n_layer = n_layer.nextSibling();
	}
	
	
	scene -> setLayers( layers );

	scenes.append( scene );
	ap_scene.release();
	
	n_scene = n_scene.nextSibling();
    }
    animation -> setScenes( scenes );
    document_ -> setAnimation( ap_animation.release() );

    //----------- Create the GUI -----------------

    createGUI();

    file_name = QString( in_file_name );
    statusBar() -> message( tr( "File Opened Successfully - %1" ).arg( in_file_name ), 2000 );

    } //-- END: if ( closed )
}

void KToon::slotOpenRecent( int mi )
{
    QString fn = open_recent -> text( mi );
    if ( fn == tr( "No Documents" ) )
    {
        QMessageBox::information( this, tr( "Message" ), tr( "There is not Document History" ) );
	return;
    }

    if ( QFile::exists( fn ) )
        slotLoadDocument( fn );
    else
        QMessageBox::critical( this, tr( "Error" ), tr( "Could not load the file - It might be moved or deleted - %1" ).arg( fn ) );
}

void KToon::slotSave()
{
    if ( file_name.isEmpty() )
    {
	slotSaveAs();
	return;
    }

    QFile f( file_name );
    if ( !f.open( IO_WriteOnly ) )
    {
	statusBar() -> message( tr( "Couldn't Save - %1" ).arg( file_name ), 2000 );
	return;
    }

    setCaption( file_name );
    current_drawing_area -> setCaption( file_name );
    current_drawing_area -> modifyDocument( false );

    document_ -> setNameDocument( file_name );
    document_ -> save( &f );

    f.close();
    statusBar() -> message( tr( "File Saved Successfully - %1" ).arg( file_name ), 2000 );
}

void KToon::slotSaveAs()
{
    QString fn = QFileDialog::getSaveFileName( KTOON_REPOSITORY, "KToon Project ( *.ktn )", this );
    if ( !fn.isEmpty() )
    {
	file_name = fn + ".ktn";
	recent_names.prepend( file_name );
	recent_names.pop_back();
	updateOpenRecentMenu();
	slotSave();
    }
}

void KToon::slotImport()
{
    import_dialog = new Import( this );
    import_dialog -> show();
    statusBar() -> message( tr( "Import Dialog opened" ), 2000 );
}

void KToon::slotExport()
{
    export_dialog = new Export( this );
    export_dialog -> show();
    statusBar() -> message( tr( "Import Dialog opened" ), 2000 );
}

void KToon::slotProperties()
{
    properties_dialog = new Properties( this );
    properties_dialog -> show();
    properties_dialog -> setMinimumSize( properties_dialog -> width(), properties_dialog -> height() );
    properties_dialog -> setMaximumSize( properties_dialog -> width(), properties_dialog -> height() );
    statusBar() -> message( tr( "Properties Dialog opened" ), 2000 );
}

//------------------- SLOTS AND FUNCTIONS EDIT MENU ----------------------------

void KToon::slotUndo()
{
	current_drawing_area -> slotUndo();
	statusBar() -> message( tr( "Last Action Undone" ), 2000 );
	
	//QMessageBox::information( this, tr( "Info" ), tr( "Undo" ) + " - " + tr( "Coming Soon..." ) );
}

void KToon::slotRedo()
{
	current_drawing_area -> slotRedo();
	statusBar() -> message( tr( "Last Action Redone" ), 2000 );
	//QMessageBox::information( this, tr( "Info" ), tr( "Redo" ) + " - " + tr( "Coming Soon..." ) );
}

void KToon::slotCut()
{
	current_drawing_area -> slotCut();
	statusBar() -> message( tr( "Cut Action" ), 2000 );
}

void KToon::slotCopy()
{
	current_drawing_area -> slotCopy();
	statusBar() -> message( tr( "Copy Action" ), 2000 );
}

void KToon::slotPaste()
{
	current_drawing_area -> slotPaste();
	statusBar() -> message( tr( "Paste Action" ), 2000 );
}

void KToon::slotPasteInPlace()
{
	current_drawing_area -> slotPasteInPlace();
	statusBar() -> message( tr( "Paste In Place Action" ), 2000 );
}

void KToon::slotDelete()
{
    current_drawing_area -> slotDelete();
    statusBar() -> message( tr( "Delete Action" ), 2000 );
}

void KToon::slotSelectAll()
{
    current_drawing_area -> slotSelectAll();
    statusBar() -> message( tr( "Select All Action" ), 2000 );
}

void KToon::slotPreferences()
{
    preferences_dialog = new Preferences( this );
    preferences_dialog -> show();
    statusBar() -> message( tr( "Preferences Dialog Opened" ), 2000 );
}

//------------------- SLOTS AND FUNCTIONS VIEW MENU ----------------------------

void KToon::slotZoomIn()
{
    current_drawing_area -> slotZoomIn();
    statusBar() -> message( tr( "Zoom In Action" ), 2000 );
}

void KToon::slotZoomOut()
{
    current_drawing_area -> slotZoomOut();
    statusBar() -> message( tr( "Zoom Out Action" ), 2000 );
}

void KToon::slotFit()
{
    current_drawing_area -> slotFit();
    statusBar() -> message( tr( "Grid Fit Action" ), 2000 );
}

void KToon::slotSeeGrid12()
{
    current_drawing_area -> slotSeeGrid12();
    statusBar() -> message( tr( "12 Field Grid Action" ), 2000 );
}

void KToon::slotSeeGrid16()
{
    current_drawing_area -> slotSeeGrid16();
    statusBar() -> message( tr( "16 Field Grid Action" ), 2000 );
}

void KToon::slotSeeSubgrid()
{
    current_drawing_area -> slotSeeSubgrid();
    statusBar() -> message( tr( "Subgrid Action" ), 2000 );
}

void KToon::slotNoGrid()
{
    current_drawing_area -> slotNoGrid();
    statusBar() -> message( tr( "No Grid Action" ), 2000 );
}

void KToon::slotNoPreviousOnionSkin()
{
    current_drawing_area -> slotNoPreviousOnionSkin();
    current_status -> setCurrentPreviousOnionSkin( 0 );
    onion_skin -> setItemChecked( previous_checked, false );
    onion_skin -> setItemChecked( id_view_previous_none, true );
    previous_toggled -> setDown( false );
    view_no_previous -> setDown( true );
    previous_toggled = view_no_previous;
    previous_checked = id_view_previous_none;
    current_drawing_area -> updateGL();
    statusBar() -> message( tr( "No Previous Onion Skin Action" ), 2000 );
}

void KToon::slotPreviousOnionSkin()
{
    current_drawing_area -> slotPreviousOnionSkin();
    current_status -> setCurrentPreviousOnionSkin( 1 );
    onion_skin -> setItemChecked( previous_checked, false );
    onion_skin -> setItemChecked( id_view_previous, true );
    previous_toggled -> setDown( false );
    view_previous -> setDown( true );
    previous_toggled = view_previous;
    previous_checked = id_view_previous;
    current_drawing_area -> updateGL();
    statusBar() -> message( tr( "Previous Onion Skin Action" ), 2000 );
}

void KToon::slotPrevious2OnionSkin()
{
    current_drawing_area -> slotPrevious2OnionSkin();
    current_status -> setCurrentPreviousOnionSkin( 2 );
    onion_skin -> setItemChecked( previous_checked, false );
    onion_skin -> setItemChecked( id_view_previous2, true );
    previous_toggled -> setDown( false );
    view_previous2 -> setDown( true );
    previous_toggled = view_previous2;
    previous_checked = id_view_previous2;
    current_drawing_area -> updateGL();
    statusBar() -> message( tr( "Previous 2 Onion Skin Action" ), 2000 );
}

void KToon::slotPrevious3OnionSkin()
{
    current_drawing_area -> slotPrevious3OnionSkin();
    current_status -> setCurrentPreviousOnionSkin( 3 );
    onion_skin -> setItemChecked( previous_checked, false );
    onion_skin -> setItemChecked( id_view_previous3, true );
    previous_toggled -> setDown( false );
    view_previous3 -> setDown( true );
    previous_toggled = view_previous3;
    previous_checked = id_view_previous3;
    current_drawing_area -> updateGL();
    statusBar() -> message( tr( "Previous 3 Onion Skin Action" ), 2000 );
}

void KToon::slotNoNextOnionSkin()
{
    current_drawing_area -> slotNoNextOnionSkin();
    current_status -> setCurrentNextOnionSkin( 0 );
    onion_skin -> setItemChecked( next_checked, false );
    onion_skin -> setItemChecked( id_view_next_none, true );
    next_toggled -> setDown( false );
    view_no_next -> setDown( true );
    next_toggled = view_no_next;
    next_checked = id_view_next_none;
    current_drawing_area -> updateGL();
    statusBar() -> message( tr( "No Next Onion Skin Action" ), 2000 );
}

void KToon::slotNextOnionSkin()
{
    current_drawing_area -> slotNextOnionSkin();
    current_status -> setCurrentNextOnionSkin( 1 );
    onion_skin -> setItemChecked( next_checked, false );
    onion_skin -> setItemChecked( id_view_next, true );
    next_toggled -> setDown( false );
    view_next -> setDown( true );
    next_toggled = view_next;
    next_checked = id_view_next;
    current_drawing_area -> updateGL();
    statusBar() -> message( tr( "Next Onion Skin Action" ), 2000 );
}

void KToon::slotNext2OnionSkin()
{
    current_drawing_area -> slotNext2OnionSkin();
    current_status -> setCurrentNextOnionSkin( 2 );
    onion_skin -> setItemChecked( next_checked, false );
    onion_skin -> setItemChecked( id_view_next2, true );
    next_toggled -> setDown( false );
    view_next2 -> setDown( true );
    next_toggled = view_next2;
    next_checked = id_view_next2;
    current_drawing_area -> updateGL();
    statusBar() -> message( tr( "Next 2 Onion Skin Action" ), 2000 );
}

void KToon::slotNext3OnionSkin()
{
    current_drawing_area -> slotNext3OnionSkin();
    current_status -> setCurrentNextOnionSkin( 3 );
    onion_skin -> setItemChecked( next_checked, false );
    onion_skin -> setItemChecked( id_view_next3, true );
    next_toggled -> setDown( false );
    view_next3 -> setDown( true );
    next_toggled = view_next3;
    next_checked = id_view_next3;
    current_drawing_area -> updateGL();
    statusBar() -> message( tr( "Next 3 Onion Skin Action" ), 2000 );
}

void KToon::slotRotateClockwise()
{
    current_drawing_area -> slotRotateClockwise();
    statusBar() -> message( tr( "Scene Rotated Clockwise" ), 2000 );
}

void KToon::slotRotateCounterClockwise()
{
    current_drawing_area -> slotRotateCounterClockwise();
    statusBar() -> message( tr( "Scene Rotated Counter Clockwise" ), 2000 );
}

void KToon::slotRestoreRotation()
{
    current_drawing_area -> slotRestoreRotation();
    statusBar() -> message( tr( "Rotation Restored" ), 2000 );
}

void KToon::slotFrontBackGrid()
{
    current_drawing_area -> slotFrontBackGrid();
    statusBar() -> message( tr( "Grid to Front/Back Action" ), 2000 );
}

void KToon::slotSeeNTSC()
{
    current_drawing_area -> slotSeeNTSC();
    statusBar() -> message( tr( "NTSC Action" ), 2000 );
}

void KToon::slotLightTable()
{
    current_drawing_area -> slotLightTable();
    statusBar() -> message( tr( "Light Table Action" ), 2000 );
}

//------------------- SLOTS AND FUNCTIONS INSERT MENU ----------------------------

void KToon::slotInsertLayer()
{
    exposure_sheet_dialog -> slotInsertLayer();
}

void KToon::slotInsertFrame()
{
    if ( window -> isItemEnabled( id_window_illustration ) )
        timeline_dialog -> frameSequenceManager() -> frameLayout() -> currentFrameSequence() -> slotInsertFrame();
}

void KToon::slotRemoveFrame()
{
    if ( window -> isItemEnabled( id_window_illustration ) )
        timeline_dialog -> frameSequenceManager() -> frameLayout() -> currentFrameSequence() -> slotRemoveFrame();
}

void KToon::slotInsertKeyFrame()
{
    if ( window -> isItemEnabled( id_window_animation ) )
        exposure_sheet_dialog -> slotInsertFrame();
}

//------------------- SLOTS AND FUNCTIONS TOOLS MENU ----------------------------

void KToon::slotGroup()
{
    current_drawing_area -> slotGroup();
    statusBar() -> message( tr( "Group Action" ), 2000 );
    QMessageBox::information( this, tr( "Info" ), tr( "Group" ) + " - " + tr( "Coming Soon..." ) );
}

void KToon::slotUngroup()
{
    current_drawing_area -> slotUngroup();
    statusBar() -> message( tr( "Ungroup Action" ), 2000 );
    QMessageBox::information( this, tr( "Info" ), tr( "Ungroup" ) + " - " + tr( "Coming Soon..." ) );
}

void KToon::slotBringToFront()
{
    current_drawing_area -> slotBringToFront();
    statusBar() -> message( tr( "Bring To Front Action" ), 2000 );
}

void KToon::slotSendToBack()
{
    current_drawing_area -> slotSendToBack();
    statusBar() -> message( tr( "Send To Back Action" ), 2000 );
}

void KToon::slotOneStepForward()
{
    current_drawing_area -> slotOneStepForward();
    statusBar() -> message( tr( "One Step Forward Action" ), 2000 );
}

void KToon::slotOneStepBackward()
{
    current_drawing_area -> slotOneStepBackward();
    statusBar() -> message( tr( "One Step Backward Action" ), 2000 );
}

void KToon::slotAlignRight()
{
    current_drawing_area -> slotAlignRight();
    statusBar() -> message( tr( "Align Right Action" ), 2000 );
}

void KToon::slotCenterVertically()
{
    current_drawing_area -> slotCenterVertically();
    statusBar() -> message( tr( "Center Vertically Action" ), 2000 );
}

void KToon::slotAlignLeft()
{
    current_drawing_area -> slotAlignLeft();
    statusBar() -> message( tr( "Align Left Action" ), 2000 );
}

void KToon::slotAlignTop()
{
    current_drawing_area -> slotAlignTop();
    statusBar() -> message( tr( "Align Top Action" ), 2000 );
}

void KToon::slotCenterHorizontally()
{
    current_drawing_area -> slotCenterHorizontally();
    statusBar() -> message( tr( "Center Horizontally Action" ), 2000 );
}

void KToon::slotAlignBottom()
{
    current_drawing_area -> slotAlignBottom();
    statusBar() -> message( tr( "Align Bottom Action" ), 2000 );
}

void KToon::slotFlipHorizontally()
{
    current_drawing_area -> slotFlipHorizontally();
    statusBar() -> message( tr( "Flip Horizontally Action" ), 2000 );
}

void KToon::slotFlipVertically()
{
    current_drawing_area -> slotFlipVertically();
    statusBar() -> message( tr( "Flip Vertically Action" ), 2000 );
}

void KToon::slotRotateCW90()
{
    current_drawing_area -> slotRotateCW90();
    statusBar() -> message( tr( "Rotate Clock Wise 90 Degrees Action" ), 2000 );
}

void KToon::slotRotateCCW90()
{
    current_drawing_area -> slotRotateCCW90();
    statusBar() -> message( tr( "Rotate Counter Clock Wise 90 Degrees Action" ), 2000 );
}

void KToon::slotRotate180()
{
    current_drawing_area -> slotRotate180();
    statusBar() -> message( tr( "Rotate 180 Degrees Action" ), 2000 );
}

void KToon::slotPerspectiveSelection()
{
    current_drawing_area -> slotPerspectiveSelection();
    statusBar() -> message( tr( "Perspective Selection Action" ), 2000 );
    QMessageBox::information( this, tr( "Info" ), tr( "Perspective" ) + " - " + tr( "Coming Soon..." ) );
}

void KToon::slotNormalSelection()
{
    tools_dialog -> changeButtonImage( Tools::NORMAL_SELECTION );
    current_status -> setCurrentCursor( Tools::NORMAL_SELECTION );
    slotActivateCursor();
}

void KToon::slotContourSelection()
{
    tools_dialog -> changeButtonImage( Tools::CONTOUR_SELECTION );
    current_status -> setCurrentCursor( Tools::CONTOUR_SELECTION );
    slotActivateCursor();
}

void KToon::slotBrush()
{
    tools_dialog -> changeButtonImage( Tools::BRUSH );
    current_status -> setCurrentCursor( Tools::BRUSH );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotPencil()
{
    tools_dialog -> changeButtonImage( Tools::PENCIL );
    current_status -> setCurrentCursor( Tools::PENCIL );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotPen()
{
    tools_dialog -> changeButtonImage( Tools::PEN );
    current_status -> setCurrentCursor( Tools::PEN );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotLine()
{
    tools_dialog -> changeButtonImage( Tools::LINE );
    current_status -> setCurrentCursor( Tools::LINE );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotRectangle()
{
    tools_dialog -> changeButtonImage( Tools::RECTANGLE );
    current_status -> setCurrentCursor( Tools::RECTANGLE );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotEllipse()
{
    tools_dialog -> changeButtonImage( Tools::ELLIPSE );
    current_status -> setCurrentCursor( Tools::ELLIPSE );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotContourFill()
{
    tools_dialog -> changeButtonImage( Tools::CONTOUR_FILL );
    current_status -> setCurrentCursor( Tools::CONTOUR_FILL );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotFill()
{
    tools_dialog -> changeButtonImage( Tools::FILL );
    current_status -> setCurrentCursor( Tools::FILL );
    color_palette_dialog -> slotActivateFillColor();
    slotActivateCursor();
}

void KToon::slotRemoveFill()
{
    tools_dialog -> changeButtonImage( Tools::REMOVE_FILL );
    current_status -> setCurrentCursor( Tools::REMOVE_FILL );
    slotActivateCursor();
}

void KToon::slotDropper()
{
    tools_dialog -> changeButtonImage( Tools::DROPPER );
    current_status -> setCurrentCursor( Tools::DROPPER );
    slotActivateCursor();
}

void KToon::slotEraser()
{
    tools_dialog -> changeButtonImage( Tools::ERASER );
    current_status -> setCurrentCursor( Tools::ERASER );
    slotActivateCursor();
}

void KToon::slotSlicer()
{
    tools_dialog -> changeButtonImage( Tools::SLICER );
    current_status -> setCurrentCursor( Tools::SLICER );
    slotActivateCursor();
}

void KToon::slotMagnifyingGlass()
{
    tools_dialog -> changeButtonImage( Tools::MAGNIFYING_GLASS );
    current_status -> setCurrentCursor( Tools::MAGNIFYING_GLASS );
    slotActivateCursor();
}

void KToon::slotHand()
{
    tools_dialog -> changeButtonImage( Tools::HAND );
    current_status -> setCurrentCursor( Tools::HAND );
    slotActivateCursor();
}

//------------------- SLOTS AND FUNCTIONS CONTROL MENU ----------------------------

void KToon::slotPlayStop()
{
    if ( timeline_dialog -> frameSequenceManager() -> getRuler() -> isAnimationPlaying() )
    {
        timeline_dialog -> frameSequenceManager() -> getRuler() -> slotStop();
        statusBar() -> message( tr( "Animation Stopped" ), 2000 );
    }
    else
    {
	if ( timeline_dialog -> frameSequenceManager() -> getRuler() -> getOffset() == timeline_dialog -> frameSequenceManager() -> getRuler() -> getMaxOffset() )
	    timeline_dialog -> frameSequenceManager() -> getRuler() -> slotSetOffset( 1 );
        timeline_dialog -> frameSequenceManager() -> getRuler() -> slotPlay();
        statusBar() -> message( tr( "Playing the Animation..." ), 2000 );
    }
}

void KToon::slotRewind()
{
    timeline_dialog -> frameSequenceManager() -> getRuler() -> slotSetOffset( 1 );
}

void KToon::slotGoToEnd()
{
    timeline_dialog -> frameSequenceManager() -> getRuler() -> slotSetOffset( timeline_dialog -> frameSequenceManager() -> getRuler() -> getMaxOffset() );
}

void KToon::slotStepForward()
{
    timeline_dialog -> frameSequenceManager() -> getRuler() -> slotStepForward();
}

void KToon::slotStepBackward()
{
    timeline_dialog -> frameSequenceManager() -> getRuler() -> slotStepBackward();
}

//------------------- SLOTS AND FUNCTIONS WINDOW MENU ----------------------------

void KToon::slotSeeIllustration()
{
    //-------------- Show all illustration dialog boxes and update the window menu items -----------

    if ( current_drawing_area != NULL )
    {
        current_drawing_area -> show();
        tools_dialog -> show();
        window -> setItemChecked( id_window_tools, true );
        window -> setItemVisible( id_window_tools, true );
	window_tools -> show();
	window_tools -> setDown( true );
        window -> setItemChecked( id_window_drawing_area, true );
        window -> setItemVisible( id_window_drawing_area, true );
	window_drawing_area -> show();
	window_drawing_area -> setDown( true );
    }

    if ( brushes_dialog != NULL )
    {
        brushes_dialog -> show();
        window -> setItemChecked( id_window_brushes, true );
        window -> setItemVisible( id_window_brushes, true );
	window_brushes -> show();
	window_brushes -> setDown( true );
    }

    if ( scenes_dialog != NULL )
    {
        scenes_dialog -> show();
        window -> setItemChecked( id_window_scenes, true );
        window -> setItemVisible( id_window_scenes, true );
	window_scenes -> show();
	window_scenes -> setDown( true );
    }

    if ( exposure_sheet_dialog != NULL )
    {
    	exposure_sheet_dialog -> show();
        window -> setItemChecked( id_window_exposure_sheet, true );
        window -> setItemVisible( id_window_exposure_sheet, true );
	window_exposure_sheet -> show();
	window_exposure_sheet -> setDown( true );
    }

    if ( library_dialog != NULL )
    {
        library_dialog -> hide();
        window -> setItemChecked( id_window_library, false );
        window -> setItemVisible( id_window_library, true );
	window_library -> show();
	window_library -> setDown( false );
    }

    color_palette_dialog -> show();
    window -> setItemChecked( id_window_color_palette, true );
    window -> setItemVisible( id_window_color_palette, true );
    window_color_palette -> show();
    window_color_palette -> setDown( true );

    if ( current_drawing_area != NULL )
    {
        menuBar() -> setItemVisible( id_edit, true );
        menuBar() -> setItemVisible( id_view, true );
        menuBar() -> setItemVisible( id_tools, true );

        insert -> setItemVisible( id_insert_frame, false );
        insert -> setItemVisible( id_insert_remove_frame, false );
        insert -> setItemVisible( id_insert_keyframe, true );
        insert -> setItemVisible( id_insert_scene, true );
        insert -> setItemVisible( id_insert_remove_scene, true );

	edit_undo -> show();
    	edit_redo -> show();
    	edit_cut -> show();
    	edit_copy -> show();
    	edit_paste -> show();
	tool_bar2 -> show();
    }
    menuBar() -> setItemVisible( id_control, false );
    file -> setItemVisible( id_file_export, false );

    //-------------- Hide all animation dialog boxes and update the window menu items -------------

    if ( render_camera_preview != NULL )
    {
        render_camera_preview -> hide();
        window -> setItemChecked( id_window_render_camera_preview, false );
        window -> setItemVisible( id_window_render_camera_preview, false );
	window_render_camera_preview -> hide();
	window_render_camera_preview -> setDown( false );
    }

    if ( side_camera_view != NULL )
    {
        side_camera_view -> hide();
        window -> setItemChecked( id_window_side_camera_view, false );
        window -> setItemVisible( id_window_side_camera_view, false );
	window_side_camera_view -> hide();
	window_side_camera_view -> setDown( false );
    }

    if ( top_camera_view != NULL )
    {
        top_camera_view -> hide();
        window -> setItemChecked( id_window_top_camera_view, false );
        window -> setItemVisible( id_window_top_camera_view, false );
	window_top_camera_view -> hide();
	window_top_camera_view -> setDown( false );
    }

    if ( timeline_dialog != NULL )
    {
        timeline_dialog -> hide();
        window -> setItemChecked( id_window_timeline, false );
        window -> setItemVisible( id_window_timeline, false );
	window_timeline -> hide();
	window_timeline -> setDown( false );
    }

    //Enable the animation menu item and disable the illustration menu item
    window -> setItemEnabled( id_window_illustration, false );
    window_illustration -> hide();
    window -> setItemEnabled( id_window_animation, true );
    window_animation -> show();
}

void KToon::slotSeeAnimation()
{
    //---------- Hide all illustration dialog boxes and update the window menu items ---------

    tools_dialog -> hide();
    window -> setItemChecked( id_window_tools, false );
    window -> setItemVisible( id_window_tools, false );
    window_tools -> hide();

    if ( brushes_dialog != NULL )
    {
        brushes_dialog -> hide();
        window -> setItemChecked( id_window_brushes, false );
        window -> setItemVisible( id_window_brushes, false );
	window_brushes -> hide();
	window_brushes -> setDown( false );
    }

    if ( scenes_dialog != NULL )
    {
    	scenes_dialog -> hide();
        window -> setItemChecked( id_window_scenes, false );
        window -> setItemVisible( id_window_scenes, false );
	window_scenes -> hide();
	window_scenes -> setDown( false );
    }

    if ( exposure_sheet_dialog != NULL )
    {
        exposure_sheet_dialog -> hide();
        window -> setItemChecked( id_window_exposure_sheet, false );
        window -> setItemVisible( id_window_exposure_sheet, false );
	window_exposure_sheet -> hide();
	window_exposure_sheet -> setDown( false );
    }

    if ( library_dialog != NULL )
    {
        library_dialog -> hide();
        window -> setItemChecked( id_window_library, false );
        window -> setItemVisible( id_window_library, false );
	window_library -> hide();
	window_library -> setDown( false );
    }

    color_palette_dialog -> hide();
    window -> setItemChecked( id_window_color_palette, false );
    window -> setItemVisible( id_window_color_palette, false );
    window_color_palette -> hide();
    window_color_palette -> setDown( false );

    if ( current_drawing_area != NULL )
    {
        current_drawing_area -> hide();
        window -> setItemChecked( id_window_drawing_area, false );
        window -> setItemVisible( id_window_drawing_area, false );
	window_drawing_area -> hide();
	window_drawing_area -> setDown( false );
    }

    insert -> setItemVisible( id_insert_frame, true );
    insert -> setItemVisible( id_insert_remove_frame, true );
    insert -> setItemVisible( id_insert_keyframe, false );
    insert -> setItemVisible( id_insert_scene, false );
    insert -> setItemVisible( id_insert_remove_scene, false );
    file -> setItemVisible( id_file_export, true );

    menuBar() -> setItemVisible( id_edit, false );
    menuBar() -> setItemVisible( id_view, false );
    menuBar() -> setItemVisible( id_tools, false );

    edit_undo -> hide();
    edit_redo -> hide();
    edit_cut -> hide();
    edit_copy -> hide();
    edit_paste -> hide();
    tool_bar2 -> hide();

    //-------------- Show all animation dialog boxes and update the window menu items -----------

    if ( render_camera_preview != NULL )
    {
        render_camera_preview -> show();
        window -> setItemChecked( id_window_render_camera_preview, true );
        window -> setItemVisible( id_window_render_camera_preview, true );
	window_render_camera_preview -> show();
	window_render_camera_preview -> setDown( true );
    }

    if ( side_camera_view != NULL )
    {
        side_camera_view -> show();
        window -> setItemChecked( id_window_side_camera_view, true );
        window -> setItemVisible( id_window_side_camera_view, true );
	window_side_camera_view -> show();
	window_side_camera_view -> setDown( true );
    }

    if ( top_camera_view != NULL )
    {
        top_camera_view -> show();
        window -> setItemChecked( id_window_top_camera_view, true );
        window -> setItemVisible( id_window_top_camera_view, true );
	window_top_camera_view -> show();
	window_top_camera_view -> setDown( true );
    }

    if ( timeline_dialog != NULL )
    {
    	timeline_dialog -> show();
        window -> setItemChecked( id_window_timeline, true );
        window -> setItemVisible( id_window_timeline, true );
	window_timeline -> show();
	window_timeline -> setDown( true );

	menuBar() -> setItemVisible( id_control, true );
    }

    //Enable the illustration menu item and disable the animation menu item
    window -> setItemEnabled( id_window_illustration, true );
    window_illustration -> show();
    window -> setItemEnabled( id_window_animation, false );
    window_animation -> hide();

    if ( timeline_dialog != NULL )
        timeline_dialog -> frameSequenceManager() -> getRuler() -> slotSetOffset( timeline_dialog -> frameSequenceManager() -> getRuler() -> getOffset() );
}

void KToon::slotWindowTools()
{
    if ( tools_dialog -> isVisible() )
    {
    	tools_dialog -> hide();
	window -> setItemChecked( id_window_tools, false );
	window_tools -> setDown( false );
    }
    else
    {
    	tools_dialog -> show();
	window -> setItemChecked( id_window_tools, true );
	window_tools -> setDown( true );
    }
}

void KToon::slotWindowBrushes()
{
    if ( brushes_dialog -> isVisible() )
    {
    	brushes_dialog -> hide();
	window -> setItemChecked( id_window_brushes, false );
	window_brushes -> setDown( false );
    }
    else
    {
    	brushes_dialog -> show();
	window -> setItemChecked( id_window_brushes, true );
	window_brushes -> setDown( true );
    }
}

void KToon::slotWindowScenes()
{
    if ( scenes_dialog -> isVisible() )
    {
    	scenes_dialog -> hide();
	window -> setItemChecked( id_window_scenes, false );
	window_scenes -> setDown( false );
    }
    else
    {
    	scenes_dialog -> show();
	window -> setItemChecked( id_window_scenes, true );
	window_scenes -> setDown( true );
    }
}

void KToon::slotWindowColorPalette()
{
    if ( color_palette_dialog -> isVisible() )
    {
    	color_palette_dialog -> hide();
	window -> setItemChecked( id_window_color_palette, false );
	window_color_palette -> setDown( false );
    }
    else
    {
    	color_palette_dialog -> show();
	window -> setItemChecked( id_window_color_palette, true );
	window_color_palette -> setDown( true );
    }
}

void KToon::slotWindowExposureSheet()
{
    if ( exposure_sheet_dialog -> isVisible() )
    {
    	exposure_sheet_dialog -> hide();
	window -> setItemChecked( id_window_exposure_sheet, false );
	window_exposure_sheet -> setDown( false );
    }
    else
    {
    	exposure_sheet_dialog -> show();
	window -> setItemChecked( id_window_exposure_sheet, true );
	window_exposure_sheet -> setDown( true );
    }
}

void KToon::slotWindowLibrary()
{
    if ( library_dialog -> isVisible() )
    {
    	library_dialog -> hide();
	window -> setItemChecked( id_window_library, false );
	window_library -> setDown( false );
    }
    else
    {
    	library_dialog -> show();
	window -> setItemChecked( id_window_library, true );
	window_library -> setDown( true );
    }
}

void KToon::slotWindowDrawingArea()
{
    if ( current_drawing_area -> isVisible() )
    {
    	current_drawing_area -> hide();
	window -> setItemChecked( id_window_drawing_area, false );
	window_drawing_area -> setDown( false );
    }
    else
    {
    	current_drawing_area -> show();
	window -> setItemChecked( id_window_drawing_area, true );
	window_drawing_area -> setDown( true );
    }
}

void KToon::slotWindowTimeline()
{
    if ( timeline_dialog -> isVisible() )
    {
    	timeline_dialog -> hide();
	window -> setItemChecked( id_window_timeline, false );
	window_timeline -> setDown( false );
    }
    else
    {
    	timeline_dialog -> show();
	window -> setItemChecked( id_window_timeline, true );
	window_timeline -> setDown( true );
    }
}

void KToon::slotWindowRenderCameraPreview()
{
    if ( render_camera_preview -> isVisible() )
    {
    	render_camera_preview -> hide();
	window -> setItemChecked( id_window_render_camera_preview, false );
	window_render_camera_preview -> setDown( false );
    }
    else
    {
    	render_camera_preview -> show();
	window -> setItemChecked( id_window_render_camera_preview, true );
	window_render_camera_preview -> setDown( true );
    }
}

void KToon::slotWindowTopCameraView()
{
    if ( top_camera_view -> isVisible() )
    {
    	top_camera_view -> hide();
	window -> setItemChecked( id_window_top_camera_view, false );
	window_top_camera_view -> setDown( false );
    }
    else
    {
    	top_camera_view -> show();
	window -> setItemChecked( id_window_top_camera_view, true );
	window_top_camera_view -> setDown( true );
    }
}

void KToon::slotWindowSideCameraView()
{
    if ( side_camera_view -> isVisible() )
    {
    	side_camera_view -> hide();
	window -> setItemChecked( id_window_side_camera_view, false );
	window_side_camera_view -> setDown( false );
    }
    else
    {
    	side_camera_view -> show();
	window -> setItemChecked( id_window_side_camera_view, true );
	window_side_camera_view -> setDown( true );
    }
}

//------------------- SLOTS AND FUNCTIONS HELP MENU ----------------------------

void KToon::slotContents()
{
    QProcess proc( this );
    
    proc.addArgument( "mozilla" ); // TODO: Necesitamos proveer configurabilidad!!!
    proc.addArgument( "http://ktoon.toonka.com/documentation/index.php?title=Manual_de_Referencia_de_KToon" );
    
    if ( !proc.start() )
        QMessageBox::critical( this, tr( "Error" ), tr( "Could not load documentation" ) );
}

void KToon::slotAbout()
{
    about_dialog = new About( this );
    about_dialog -> move( 100, 100 );
    about_dialog -> show();
}

void KToon::slotAboutOpenGL()
{
    QDialog *opengl_dialog = new QDialog( this );
    opengl_dialog -> resize( 400, 340 );
    opengl_dialog -> setMaximumSize( 400, 340 );
    opengl_dialog -> setMinimumSize( 400, 340 );
    opengl_dialog -> setCaption( tr( "About OpenGL" ) );

    QLabel *logo = new QLabel( opengl_dialog );
    logo -> move( 144, 10 );
    logo -> resize( 112, 70 );
    QPixmap i_logo = QPixmap( KTOON_HOME + "images/ogllogo.png", "PNG" );
    logo -> setPixmap( i_logo );

    QLabel *text = new QLabel( QString( "OpenGL is the premier environment for developing portable,\n" ) +
    			                "interactive 2D and 3D graphics applications. Since its introduction\n" +
					"in 1992, OpenGL has become the industry's most widely used and\n" +
					"supported 2D and 3D graphics application programming interface\n" +
					"bringing thousands of applications to a wide variety of computer\n" +
					"platforms. OpenGL fosters innovation and speeds application\n" +
					"development by incorporating a broad set of rendering, texture\n" +
					"mapping, special effects, and other powerful visualization functions.\n" +
					"Developers can leverage the power of OpenGL across all popular\n" +
					"desktop and workstation platforms, ensuring wide application\n" +
					"deployment.", opengl_dialog );
//     text -> setFont( QFont( "helvetica", 9 ) );
    text -> resize( 380, 200 );
    text -> setAlignment( Qt::AlignJustify );
    text -> move( 10, logo -> height() + logo -> y() + 10 );

    QPushButton *ok = new QPushButton( "OK", opengl_dialog );
    ok -> move( 144, text -> height() + text -> y() + 10 );
    connect( ok, SIGNAL( clicked() ), opengl_dialog, SLOT( close() ) );

    opengl_dialog -> show();
}

//********************* General Slots ********************

void KToon::slotCloseDrawingArea()
{
    list_of_es.setAutoDelete( true );
    list_of_tl.setAutoDelete( true );
    list_of_es.clear();
    list_of_tl.clear();
    list_of_es.setAutoDelete( false );
    list_of_tl.setAutoDelete( false );

    delete current_drawing_area;
    delete scenes_dialog;
    delete brushes_dialog;
    delete library_dialog;
    delete render_camera_preview;
    delete top_camera_view;
    delete side_camera_view;

    current_drawing_area = NULL;
    exposure_sheet_dialog = NULL;
    timeline_dialog = NULL;
    scenes_dialog = NULL;
    tools_dialog -> hide();
    library_dialog = NULL;
    brushes_dialog = NULL;
    render_camera_preview = NULL;
    top_camera_view = NULL;
    side_camera_view = NULL;

    file -> setItemEnabled( id_file_save, false );
    file -> setItemEnabled( id_file_save_as, false );
    file -> setItemEnabled( id_file_close, false );
    file -> setItemEnabled( id_file_import, false );
    file -> setItemEnabled( id_file_export, false );
    file -> setItemEnabled( id_file_properties, false );

    window -> setItemVisible( id_window_exposure_sheet, false );
    window -> setItemVisible( id_window_library, false );
    window -> setItemChecked( id_window_library, false );
    window -> setItemVisible( id_window_scenes, false );
    window -> setItemVisible( id_window_drawing_area, false );
    window -> setItemVisible( id_window_timeline, false );
    window -> setItemVisible( id_window_tools, false );
    window -> setItemVisible( id_window_brushes, false );
    window -> setItemVisible( id_window_render_camera_preview, false );
    window -> setItemVisible( id_window_top_camera_view, false );
    window -> setItemVisible( id_window_side_camera_view, false );

    window_exposure_sheet -> hide();
    window_library -> hide();
    window_library -> setDown( false );
    window_scenes -> hide();
    window_drawing_area -> hide();
    window_timeline -> hide();
    window_tools -> hide();
    window_brushes -> hide();
    window_render_camera_preview -> hide();
    window_top_camera_view -> hide();
    window_side_camera_view -> hide();

    //murakumo: Here it is, Volker!
    delete document_;
    document_ = NULL;

    current_status -> setCurrentScene( NULL );
    current_status -> setCurrentLayer( NULL );
    current_status -> setCurrentKeyFrame( NULL );
    QPtrList<KeyFrame> empty;
    current_status -> setRenderKeyframes( empty );

    menuBar() -> setItemVisible( id_insert, false );
    menuBar() -> setItemVisible( id_control, false );
    if ( window -> isItemEnabled( id_window_animation ) )
    {
    	menuBar() -> setItemVisible( id_edit, false );
    	menuBar() -> setItemVisible( id_view, false );
    	menuBar() -> setItemVisible( id_tools, false );

    	edit_undo -> hide();
    	edit_redo -> hide();
    	edit_cut -> hide();
    	edit_copy -> hide();
    	edit_paste -> hide();
	tool_bar2 -> hide();
    }
    file_save -> hide();

    color_palette_dialog -> enableCustomPalette( false );
    color_palette_dialog -> clearCustomPalette();
}

void KToon::slotStatusBarMessage( int mi )
{
    if ( id_file_new == mi ) statusBar() -> message( tr( "Opens a new document" ), 2000 );
    else if( id_file_open == mi ) statusBar() -> message( tr( "Loads an existent document" ), 2000 );
    else if( id_file_save == mi ) statusBar() -> message( tr( "Saves the current document in the current location" ), 2000 );
    else if( id_file_save_as == mi ) statusBar() -> message( tr( "Opens a dialog box to save the current document in any location" ), 2000 );
    else if( id_file_close == mi ) statusBar() -> message( tr( "Closes the active document" ), 2000 );
    else if( id_file_properties == mi ) statusBar() -> message( tr( "Opens the properties dialog box" ), 2000 );
    else if( id_file_import == mi ) statusBar() -> message( tr( "Imports a file in the supported format" ), 2000 );
    else if( id_file_export == mi ) statusBar() -> message( tr( "Exports this document as a file in the available formats" ), 2000 );
    else if( id_file_exit == mi ) statusBar() -> message( tr( "Closes the application" ), 2000 );
    else if( id_edit_undo == mi ) statusBar() -> message( tr( "Undoes the last draw action" ), 2000 );
    else if( id_edit_redo == mi ) statusBar() -> message( tr( "Redoes a previous undone action" ), 2000 );
    else if( id_edit_cut == mi ) statusBar() -> message( tr( "Cuts the selection and puts it onto the clipboard" ), 2000 );
    else if( id_edit_copy == mi ) statusBar() -> message( tr( "Copies the selection and puts it onto the clipboard" ), 2000 );
    else if( id_edit_paste == mi ) statusBar() -> message( tr( "Pastes the clipboard into the current document" ), 2000 );
    else if( id_edit_paste_in_place == mi ) statusBar() -> message( tr( "Pastes the clipboard into the same place as the copy was did" ), 2000 );
    else if( id_edit_delete == mi ) statusBar() -> message( tr( "Deletes the selected object" ), 2000 );
    else if( id_edit_select_all == mi ) statusBar() -> message( tr( "Selects all objects in the document" ), 2000 );
    else if( id_edit_preferences == mi ) statusBar() -> message( tr( "Opens the preferences dialog box" ), 2000 );
    else if( id_view_zoom_in == mi ) statusBar() -> message( tr( "Performs a Zoom In of the drawing area" ), 2000 );
    else if( id_view_zoom_out == mi ) statusBar() -> message( tr( "Performs a Zoom Out of the drawing area" ), 2000 );
    else if( id_view_fit == mi ) statusBar() -> message( tr( "Fits the grid into the drawing area" ), 2000 );
    else if( id_view_grid12 == mi ) statusBar() -> message( tr( "Shows a 12 field grid" ), 2000 );
    else if( id_view_grid16 == mi ) statusBar() -> message( tr( "Shows a 16 field grid" ), 2000 );
    else if( id_view_subgrid == mi ) statusBar() -> message( tr( "Shows or hides a 3 field subgrid in the current grid" ), 2000 );
    else if( id_view_no_grid == mi ) statusBar() -> message( tr( "Hides the grid" ), 2000 );
    else if( id_view_front_back_grid == mi ) statusBar() -> message( tr( "Sends the grid to the front or to the back of the drawing area" ), 2000 );
    else if( id_view_NTSC_zone == mi ) statusBar() -> message( tr( "Shows or hides the NTSC Zone" ), 2000 );
    else if( id_view_light_table == mi ) statusBar() -> message( tr( "Activates or deactivates the light table" ), 2000 );
    else if( id_view_previous_none == mi ) statusBar() -> message( tr( "Disables previous onion skin visualization" ), 2000 );
    else if( id_view_previous == mi ) statusBar() -> message( tr( "Shows the previous onion skin" ), 2000 );
    else if( id_view_previous2 == mi ) statusBar() -> message( tr( "Shows the previous 2 onion skins" ), 2000 );
    else if( id_view_previous3 == mi ) statusBar() -> message( tr( "Shows the previous 3 onion skins" ), 2000 );
    else if( id_view_next_none == mi ) statusBar() -> message( tr( "Disables next onion skin visualization" ), 2000 );
    else if( id_view_next == mi ) statusBar() -> message( tr( "Shows the next onion skin" ), 2000 );
    else if( id_view_next2 == mi ) statusBar() -> message( tr( "Shows the next 2 onion skins" ), 2000 );
    else if( id_view_next3 == mi ) statusBar() -> message( tr( "Shows the next 3 onion skins" ), 2000 );
    else if( id_view_rotate_clockwise == mi ) statusBar() -> message( tr( "Rotates the scene 30 degrees clockwise" ), 2000 );
    else if( id_view_rotate_counterclockwise == mi ) statusBar() -> message( tr( "Rotates the scene 30 degrees counterclockwise" ), 2000 );
    else if( id_view_restore_rotation == mi ) statusBar() -> message( tr( "Restores any rotation performed on the scene" ), 2000 );
    else if( id_insert_layer == mi ) statusBar() -> message( tr( "Inserts a new layer into the Exposure Sheet and Timeline" ), 2000 );
    else if( id_insert_frame == mi ) statusBar() -> message( tr( "Inserts a frame into the current layer of the Timeline" ), 2000 );
    else if( id_insert_remove_frame == mi ) statusBar() -> message( tr( "Removes a frame from the current layer of the Timeline" ), 2000 );
    else if( id_insert_keyframe == mi ) statusBar() -> message( tr( "Inserts a keyframe into the ExposureSheet and Timeline" ), 2000 );
    else if( id_insert_scene == mi ) statusBar() -> message( tr( "Inserts a new scene" ), 2000 );
    else if( id_insert_remove_scene == mi ) statusBar() -> message( tr( "Removes the selected scene" ), 2000 );
    else if( id_tools_normal_selection == mi ) statusBar() -> message( tr( "Activates the Selection tool" ), 2000 );
    else if( id_tools_contour_selection == mi ) statusBar() -> message( tr( "Activates the Node Selection tool" ), 2000 );
    else if( id_tools_brush == mi ) statusBar() -> message( tr( "Activates the Brush tool" ), 2000 );
    else if( id_tools_pencil == mi ) statusBar() -> message( tr( "Activates the Pencil tool" ), 2000 );
    else if( id_tools_pen == mi ) statusBar() -> message( tr( "Activates the Selection tool" ), 2000 );
    else if( id_tools_line == mi ) statusBar() -> message( tr( "Activates the Line tool" ), 2000 );
    else if( id_tools_rectangle == mi ) statusBar() -> message( tr( "Activates the Rectangle tool" ), 2000 );
    else if( id_tools_ellipse == mi ) statusBar() -> message( tr( "Activates the Ellipse tool" ), 2000 );
    else if( id_tools_fill == mi ) statusBar() -> message( tr( "Activates the Fill tool" ), 2000 );
    else if( id_tools_remove_fill == mi ) statusBar() -> message( tr( "Activates the Remove Fill tool" ), 2000 );
    else if( id_tools_contour_fill == mi ) statusBar() -> message( tr( "Activates the Contour Fill tool" ), 2000 );
    else if( id_tools_dropper == mi ) statusBar() -> message( tr( "Activates the Dropper tool" ), 2000 );
    else if( id_tools_eraser == mi ) statusBar() -> message( tr( "Activates the Eraser tool" ), 2000 );
    else if( id_tools_slicer == mi ) statusBar() -> message( tr( "Activates the Slicer tool" ), 2000 );
    else if( id_tools_magnifying_glass == mi ) statusBar() -> message( tr( "Activates the Magnifying Glass tool" ), 2000 );
    else if( id_tools_hand == mi ) statusBar() -> message( tr( "Activates the Hand tool" ), 2000 );
    else if( id_tools_group == mi ) statusBar() -> message( tr( "Group the selected objects into a single one" ), 2000 );
    else if( id_tools_ungroup == mi ) statusBar() -> message( tr( "Ungroups the selected object" ), 2000 );
    else if( id_tools_bring_front == mi ) statusBar() -> message( tr( "Brings the selected object to the front" ), 2000 );
    else if( id_tools_send_back == mi ) statusBar() -> message( tr( "Sends the selected objects to the back" ), 2000 );
    else if( id_tools_one_step_forward == mi ) statusBar() -> message( tr( "Moves the selected object one step forward" ), 2000 );
    else if( id_tools_one_step_backward == mi ) statusBar() -> message( tr( "Moves the selected object one step backward" ), 2000 );
    else if( id_tools_left == mi ) statusBar() -> message( tr( "Aligns the selected object to the left" ), 2000 );
    else if( id_tools_center_vertically == mi ) statusBar() -> message( tr( "Centers vertically the selected object" ), 2000 );
    else if( id_tools_right == mi ) statusBar() -> message( tr( "Aligns the selected object to the right" ), 2000 );
    else if( id_tools_top == mi ) statusBar() -> message( tr( "Aligns the selected object to the top" ), 2000 );
    else if( id_tools_center_horizontally == mi ) statusBar() -> message( tr( "Centers horizontally the selected object" ), 2000 );
    else if( id_tools_bottom == mi ) statusBar() -> message( tr( "Aligns the selected object to the bottom" ), 2000 );
    else if( id_tools_flip_horizontally == mi ) statusBar() -> message( tr( "Flips the selected object horizontally" ), 2000 );
    else if( id_tools_flip_vertically == mi ) statusBar() -> message( tr( "Flips the selected object vertically" ), 2000 );
    else if( id_tools_rotate_cw90 == mi ) statusBar() -> message( tr( "Rotates the selected object 90 degrees clockwise" ), 2000 );
    else if( id_tools_rotate_ccw90 == mi ) statusBar() -> message( tr( "Rotates the selected object 90 degrees counterclockwise" ), 2000 );
    else if( id_tools_rotate180 == mi ) statusBar() -> message( tr( "Rotates the selected object 180 degrees" ), 2000 );
    else if( id_tools_perspective == mi ) statusBar() -> message( tr( "Activates the perspective tool" ), 2000 );
    else if( id_control_play == mi ) statusBar() -> message( tr( "Plays the Animation" ), 2000 );
    else if( id_control_step_forward == mi ) statusBar() -> message( tr( "Steps one frame forward in the animation" ), 2000 );
    else if( id_control_step_backward == mi ) statusBar() -> message( tr( "Steps one frame backward in the animation" ), 2000 );
    else if( id_control_rewind == mi ) statusBar() -> message( tr( "Rewinds the Animation" ), 2000 );
    else if( id_control_go_to_end == mi ) statusBar() -> message( tr( "Goes to the last frame of the Animation" ), 2000 );
    else if( id_window_illustration == mi ) statusBar() -> message( tr( "Turns to the illustration mode" ), 2000 );
    else if( id_window_animation == mi ) statusBar() -> message( tr( "Turns to the animation mode" ), 2000 );
    else if( id_window_tools == mi ) statusBar() -> message( tr( "Shows or hides the tools dialog box" ), 2000 );
    else if( id_window_brushes == mi ) statusBar() -> message( tr( "Shows or hides the brushes dialog box" ), 2000 );
    else if( id_window_scenes == mi ) statusBar() -> message( tr( "Shows or hides the scenes dialog box" ), 2000 );
    else if( id_window_color_palette == mi ) statusBar() -> message( tr( "Shows or hides the color palette dialog box" ), 2000 );
    else if( id_window_exposure_sheet == mi ) statusBar() -> message( tr( "Shows or hides the exposure sheet dialog box" ), 2000 );
    else if( id_window_library == mi ) statusBar() -> message( tr( "Shows or hides the library dialog box" ), 2000 );
    else if( id_window_drawing_area == mi ) statusBar() -> message( tr( "Shows or hides the drawing area widget" ), 2000 );
    else if( id_window_timeline == mi ) statusBar() -> message( tr( "Shows or hides the timeline dialog box" ), 2000 );
    else if( id_window_render_camera_preview == mi ) statusBar() -> message( tr( "Shows or hides the render preview widget" ), 2000 );
    else if( id_window_top_camera_view == mi ) statusBar() -> message( tr( "Shows or hides the top camera view widget" ), 2000 );
    else if( id_window_side_camera_view == mi ) statusBar() -> message( tr( "Shows or hides the side camera view widget" ), 2000 );
    else if( id_help_contents == mi )  statusBar() -> message( tr( "Shows the help of the application" ), 2000 );
    else if( id_help_about == mi ) statusBar() -> message( tr( "Shows information about the authors and another stuff" ), 2000 );
    else if( id_help_about_qt == mi ) statusBar() -> message( tr( "Shows information about Qt" ), 2000 );
    else if( id_help_about_opengl == mi ) statusBar() -> message( tr( "Shows information about OpenGL" ), 2000 );
}

void KToon::slotActivateCursor()
{
    ILayer *il = exposure_sheet_dialog -> currentLayerObj();
    ESFrame *sf = il -> selectedFrame();
    bool locked = false;
    if ( sf != NULL )
	locked = sf -> isLocked();
    if ( current_status -> currentKeyFrame() != NULL && !locked )
    {
        switch ( current_status -> currentCursor() )
	{
 	    case Tools::NORMAL_SELECTION: current_drawing_area -> setCursor( *cursor_selection );
			break;
	    case Tools::CONTOUR_SELECTION: current_drawing_area -> setCursor( *cursor_nodes );
			break;
	    case Tools::BRUSH: current_drawing_area -> setCursor( *cursor_brush );
		        break;
	    case Tools::PENCIL: current_drawing_area -> setCursor( *cursor_pencil );
		        break;
	    case Tools::PEN: current_drawing_area -> setCursor( *cursor_pen );
		        break;
	    case Tools::LINE: current_drawing_area -> setCursor( *cursor_line );
	    		break;
	    case Tools::RECTANGLE: current_drawing_area -> setCursor( *cursor_rectangle );
		        break;
	    case Tools::ELLIPSE: current_drawing_area -> setCursor( *cursor_ellipse );
		        break;
	    case Tools::CONTOUR_FILL: current_drawing_area -> setCursor( *cursor_contour_fill );
		        break;
	    case Tools::FILL: current_drawing_area -> setCursor( *cursor_fill );
		        break;
	    case Tools::REMOVE_FILL: current_drawing_area -> setCursor( *cursor_remove_fill );
			break;
	    case Tools::DROPPER: current_drawing_area -> setCursor( *cursor_dropper );
		        break;
	    case Tools::ERASER: current_drawing_area -> setCursor( *cursor_eraser );
		        break;
	    case Tools::SLICER: current_drawing_area -> setCursor( *cursor_slicer );
		        break;
	    case Tools::MAGNIFYING_GLASS: current_drawing_area -> setCursor( *cursor_magnifying_glass );
		        break;
	    case Tools::HAND: current_drawing_area -> setCursor( *cursor_hand );
		        break;
	    default:    break;
        }
    }
    else
        current_drawing_area -> setCursor( QCursor( Qt::ForbiddenCursor ) );
}

//**************

void KToon::slotSelectSync( int sp )
{
    exposure_sheet_dialog -> hide();
    exposure_sheet_dialog = list_of_es.at( sp );
    if ( window -> isItemChecked( id_window_exposure_sheet ) && window -> isItemEnabled( id_window_animation ) )
        exposure_sheet_dialog -> show();

    timeline_dialog = list_of_tl.at( sp );

    slotActivateCursor();
}

void KToon::slotInsertSync()
{
    ExposureSheet *new_exposure_sheet = new ExposureSheet( this, Qt::WStyle_Tool, window, id_window_exposure_sheet, window_exposure_sheet );
    list_of_es.append( new_exposure_sheet );
    Timeline *new_timeline = new Timeline( this, Qt::WStyle_Tool, window, id_window_timeline, window_timeline );
    list_of_tl.append( new_timeline );

    //--------- Main Connections Again --------------

    connect( new_exposure_sheet, SIGNAL( layerInserted() ), new_timeline -> layerManager() -> layerSequence(), SLOT( slotInsertLayer() ) );
    connect( new_exposure_sheet, SIGNAL( layerInserted() ), new_timeline -> frameSequenceManager() -> frameLayout(), SLOT( slotInsertFrameSequence() ) );
    connect( new_exposure_sheet, SIGNAL( layerRemoved() ), new_timeline -> layerManager() -> layerSequence(), SLOT( slotRemoveLayer() ) );
    connect( new_exposure_sheet, SIGNAL( layerRemoved() ), new_timeline -> frameSequenceManager() -> frameLayout(), SLOT( slotRemoveFrameSequence() ) );
    connect( new_exposure_sheet, SIGNAL( layerRemoved() ), new_timeline -> frameSequenceManager() -> frameLayout(), SLOT( slotUpdateMaxUsedFrames() ) );
    connect( new_exposure_sheet, SIGNAL( layerSelected( int ) ), new_timeline -> layerManager() -> layerSequence(), SLOT( slotSelectLayerFromES( int ) ) );
    connect( new_exposure_sheet, SIGNAL( layerRenamed( int, const QString & ) ), new_timeline -> layerManager() -> layerSequence(), SLOT( slotRenameLayerFromES( int, const QString & ) ) );
    connect( new_exposure_sheet, SIGNAL( framesInsertedAtTheEnd( int ) ), new_timeline -> frameSequenceManager() -> frameLayout(), SLOT( slotInsertKeyframesIntoTheCurrentFS( int ) ) );
    connect( new_exposure_sheet, SIGNAL( frameRemoved( int ) ), new_timeline -> frameSequenceManager() -> frameLayout(), SLOT( slotRemoveKeyframeBlockFromTheCurrentFS( int ) ) );
    connect( new_exposure_sheet, SIGNAL( frameMovedUp( int ) ), new_timeline -> frameSequenceManager() -> frameLayout(), SLOT( slotMoveKeyframeLeftInTheCurrentFS( int ) ) );
    connect( new_exposure_sheet, SIGNAL( frameMovedDown( int ) ), new_timeline -> frameSequenceManager() -> frameLayout(), SLOT( slotMoveKeyframeRightInTheCurrentFS( int ) ) );
    connect( new_exposure_sheet, SIGNAL( frameSelected() ), current_drawing_area, SLOT( slotSelectFrame() ) );
    connect( new_exposure_sheet, SIGNAL( frameSelected() ), SLOT( slotActivateCursor() ) );

    connect( new_timeline, SIGNAL( insertLayerClicked() ), new_exposure_sheet, SLOT( slotInsertLayer() ) );
    connect( new_timeline, SIGNAL( removeLayerClicked() ), new_exposure_sheet, SLOT( slotRemoveLayer() ) );
    connect( new_timeline -> layerManager() -> layerSequence(), SIGNAL( layerSelectedToES( int ) ), new_exposure_sheet, SLOT( slotSelectLayerFromTL( int ) ) );
    connect( new_timeline -> layerManager() -> layerSequence(), SIGNAL( layerRenamed( int, const QString & ) ), new_exposure_sheet, SLOT( slotRenameLayerFromTL( int, const QString & ) ) );
    connect( new_timeline -> frameSequenceManager() -> frameLayout(), SIGNAL( keyframeRemovedToES( int ) ), new_exposure_sheet, SLOT( slotRemoveFrameFromTL( int ) ) );
    connect( new_timeline, SIGNAL( layerMovedUp() ), new_exposure_sheet, SLOT( slotMoveLayerLeft() ) );
    connect( new_timeline, SIGNAL( layerMovedDown() ), new_exposure_sheet, SLOT( slotMoveLayerRight() ) );
    connect( new_timeline -> layerManager() -> layerSequence(), SIGNAL( layerReleasedAbove( int, int ) ), new_exposure_sheet, SLOT( slotSwapWithLeftLayer( int, int ) ) );
    connect( new_timeline -> layerManager() -> layerSequence(), SIGNAL( layerReleasedBelow( int, int ) ), new_exposure_sheet, SLOT( slotSwapWithRightLayer( int, int ) ) );

    connect( current_drawing_area, SIGNAL( wasDrawn( bool ) ), new_timeline -> frameSequenceManager() -> frameLayout(), SLOT( slotFrameHasDrawing( bool ) ) );
}

void KToon::slotRemoveSync( int sp )
{
    ExposureSheet *es_to_remove = list_of_es.at( sp );
    list_of_es.remove( es_to_remove );
    delete es_to_remove;
    if ( sp == ( int )list_of_es.count() )
    {
        exposure_sheet_dialog = list_of_es.getLast();
	if ( window -> isItemChecked( id_window_exposure_sheet ) && window -> isItemEnabled( id_window_animation ) )
	    exposure_sheet_dialog -> show();
    }
    else
    {
        exposure_sheet_dialog = list_of_es.at( sp );
	if ( window -> isItemChecked( id_window_exposure_sheet ) && window -> isItemEnabled( id_window_animation ) )
	    exposure_sheet_dialog -> show();
    }

    Timeline *tl_to_remove = list_of_tl.at( sp );
    list_of_tl.remove( tl_to_remove );
    delete tl_to_remove;
    if ( sp == ( int )list_of_tl.count() )
        timeline_dialog = list_of_tl.getLast();
    else
        timeline_dialog = list_of_tl.at( sp );
}

void KToon::slotMoveUpSync( int sp )
{
    ExposureSheet *es_to_move = list_of_es.take( sp );
    Timeline *tl_to_move = list_of_tl.take( sp );
    list_of_es.insert( sp - 1, es_to_move );
    list_of_tl.insert( sp - 1, tl_to_move );
    exposure_sheet_dialog = es_to_move;
    timeline_dialog = tl_to_move;
}

void KToon::slotMoveDownSync( int sp )
{
    ExposureSheet *es_to_move = list_of_es.take( sp );
    Timeline *tl_to_move = list_of_tl.take( sp );
    list_of_es.insert( sp + 1, es_to_move );
    list_of_tl.insert( sp + 1, tl_to_move );
    exposure_sheet_dialog = es_to_move;
    timeline_dialog = tl_to_move;
}

//--------------------- EVENTS AND OTHER FUNCTIONS --------------------------------

void KToon::closeEvent( QCloseEvent *close_event )
{
    Q_CHECK_PTR( close_event );
    if ( current_drawing_area )
    {
        if ( !current_drawing_area -> close() )
	{
	    close_event -> ignore();
	    return;
	}
	else
	{
		KTConfigDocument config( QDir::homeDirPath()+QString("/.ktoonrc") );
		
		QFile settings( config.path() );
			
		if ( config.setContent(&settings) )
		{
			settings.close();
			config.addRecentFiles(recent_names);
			
			if ( settings.open( IO_WriteOnly ) )
			{
				QTextStream ts(&settings);
				ts << config.toString() << endl;
			}
			else
			{
				qDebug("Could not open the settings file. Impossible to save the recent file names.");
			}
		}
			
		settings.close();
	}
    }
    
    close_event -> accept();
}

void KToon::createGUI()
{
    setCaption( document_ -> nameDocument() );

    QPtrList<Color> custom_colors = document_ -> getPalette() -> getColors();
    color_palette_dialog -> enableCustomPalette( true );
    color_palette_dialog -> loadCustomColors( custom_colors );

    QPtrList<Brush> brushes = document_ -> getBrushes();
    brushes_dialog = new Brushes( this, Qt::WStyle_Tool, window, id_window_brushes, window_brushes );
    brushes_dialog -> loadBrushes( brushes );

    QPtrList<Scene> scenes = document_ -> getAnimation() -> getScenes();
    scenes_dialog = new Scenes( this, Qt::WStyle_Tool, window, id_window_scenes, window_scenes );
    scenes_dialog -> loadScenes( scenes );

    library_dialog = new Library( this, Qt::WStyle_Tool, window, id_window_library, current_drawing_area, window_library );
    library_dialog -> loadItems( document_ -> getLibrary() -> getItems() );

    render_camera_preview = new GLRenderCameraPreview( main_panel, this, window, id_window_render_camera_preview, window_render_camera_preview, current_drawing_area );
    top_camera_view = new GLTopCameraView( main_panel, this, window, id_window_top_camera_view, window_top_camera_view, current_drawing_area );
    side_camera_view = new GLSideCameraView( main_panel, this, window, id_window_side_camera_view, window_side_camera_view, current_drawing_area );

    Scene *s_it;
    for ( s_it = scenes.first(); s_it; s_it = scenes.next() )
    {
        slotInsertSync();
    	QPtrList<Layer> layers = s_it -> getLayers();
	ExposureSheet *es = list_of_es.at( scenes.find( s_it ) );
	es -> loadLayersAndKeyframes( layers );
	Timeline *tl = list_of_tl.at( scenes.find( s_it ) );
	tl -> loadLayersAndKeyframes( layers );
	QPtrList<ILayer> layer_list = es -> getILayers();
    }

    exposure_sheet_dialog = list_of_es.at( 0 );
    timeline_dialog = list_of_tl.at( 0 );
    if ( window -> isItemEnabled( id_window_animation ) )
    {
        window -> setItemVisible( id_window_exposure_sheet, true );
	window -> setItemChecked( id_window_exposure_sheet, true );
	window_exposure_sheet -> show();
	window_exposure_sheet -> setDown( true );
        exposure_sheet_dialog -> show();
        window -> setItemVisible( id_window_drawing_area, true );
	window -> setItemChecked( id_window_drawing_area, true );
	window_drawing_area -> show();
	window_drawing_area -> setDown( true );
        current_drawing_area -> show();
        window -> setItemVisible( id_window_scenes, true );
	window -> setItemChecked( id_window_scenes, true );
	window_scenes -> show();
	window_scenes -> setDown( true );
        scenes_dialog -> show();
        window -> setItemVisible( id_window_tools, true );
	window -> setItemChecked( id_window_tools, true );
	window_tools -> show();
	window_tools -> setDown( true );
        tools_dialog -> show();
        window -> setItemVisible( id_window_library, true );
	window -> setItemChecked( id_window_library, false );
	window_library -> show();
	window_library -> setDown( false );
        window -> setItemVisible( id_window_brushes, true );
	window -> setItemChecked( id_window_brushes, true );
	window_brushes -> show();
	window_brushes -> setDown( true );
        brushes_dialog -> show();
    }
    else if ( window -> isItemEnabled( id_window_illustration ) )
    {
        window -> setItemVisible( id_window_timeline, true );
	window -> setItemChecked( id_window_timeline, true );
	window_timeline -> show();
	window_timeline -> setDown( true );
        timeline_dialog -> show();
        window -> setItemVisible( id_window_render_camera_preview, true );
	window -> setItemChecked( id_window_render_camera_preview, true );
	window_render_camera_preview -> show();
	window_render_camera_preview -> setDown( true );
        render_camera_preview -> show();
        window -> setItemVisible( id_window_side_camera_view, true );
	window -> setItemChecked( id_window_side_camera_view, true );
	window_side_camera_view -> show();
	window_side_camera_view -> setDown( true );
        side_camera_view-> show();
        window -> setItemVisible( id_window_top_camera_view, true );
	window -> setItemChecked( id_window_top_camera_view, true );
	window_top_camera_view -> show();
	window_top_camera_view -> setDown( true );
        top_camera_view -> show();
    }

    current_drawing_area -> setCursor( QCursor( Qt::ForbiddenCursor ) );

    connect( current_drawing_area, SIGNAL( closed() ), SLOT( slotCloseDrawingArea() ) );
    connect( color_palette_dialog, SIGNAL( outlineColorChanged() ), current_drawing_area, SLOT( slotChangeOutlineColor() ) );
    connect( color_palette_dialog, SIGNAL( fillColorChanged() ), current_drawing_area, SLOT( slotChangeFillColor() ) );
    connect( brushes_dialog, SIGNAL( minThicknessChanged() ), current_drawing_area, SLOT( slotChangeMinThicknessBrush() ) );
    connect( brushes_dialog, SIGNAL( maxThicknessChanged() ), current_drawing_area, SLOT( slotChangeMaxThicknessBrush() ) );
    connect( brushes_dialog, SIGNAL( smoothnessChanged() ), current_drawing_area, SLOT( slotChangeSmoothnessBrush() ) );
    connect( current_drawing_area, SIGNAL( colorGrabbed( Color::Color * ) ), color_palette_dialog, SLOT( slotSetColor( Color::Color * ) ) );
    insert -> connectItem( id_insert_scene, scenes_dialog, SLOT( slotInsertScene() ) );
    insert -> connectItem( id_insert_remove_scene, scenes_dialog, SLOT( slotRemoveScene() ) );
    file -> connectItem( id_file_close, current_drawing_area, SLOT( close() ) );
    connect( scenes_dialog, SIGNAL( sceneInserted() ), SLOT( slotInsertSync() ) );
    connect( scenes_dialog, SIGNAL( sceneMovedDown( int ) ), SLOT( slotMoveDownSync( int ) ) );
    connect( scenes_dialog, SIGNAL( sceneMovedUp( int ) ), SLOT( slotMoveUpSync( int ) ) );
    connect( scenes_dialog, SIGNAL( sceneRemoved( int ) ), SLOT( slotRemoveSync( int ) ) );
    connect( scenes_dialog, SIGNAL( sceneSelected( int ) ), SLOT( slotSelectSync( int ) ) );

    current_status -> setCurrentScene( ( document_ -> getAnimation() -> getScenes() ).first() );
    current_status -> setCurrentLayer( ( current_status -> currentScene() -> getLayers() ).first() );
    current_status -> setCurrentKeyFrame( ( ( current_status -> currentScene() -> getLayers() ).first() -> keyFrames() ).first() );

    exposure_sheet_dialog -> touchFirstFrame();
    scenes_dialog -> selectFirstScene();

    timeline_dialog -> frameSequenceManager() -> frameLayout() -> slotUpdateMaxUsedFrames();
    exposure_sheet_dialog -> updateIndicators( exposure_sheet_dialog -> currentLayerObj() );

    file -> setItemEnabled( id_file_save, true );
    file -> setItemEnabled( id_file_save_as, true );
    file -> setItemEnabled( id_file_close, true );
    file -> setItemEnabled( id_file_import, true );
    file -> setItemEnabled( id_file_export, true );
    file -> setItemEnabled( id_file_properties, true );

    menuBar() -> setItemVisible( id_insert, true );
    if ( window -> isItemEnabled( id_window_animation ) )
    {
    	menuBar() -> setItemVisible( id_edit, true );
    	menuBar() -> setItemVisible( id_view, true );
    	menuBar() -> setItemVisible( id_tools, true );

    	edit_undo -> show();
    	edit_redo -> show();
    	edit_cut -> show();
    	edit_copy -> show();
    	edit_paste -> show();
	tool_bar2 -> show();
    }
    else
        menuBar() -> setItemVisible( id_control, true );
    file_save -> show();
}

void KToon::updateOpenRecentMenu()
{
    open_recent -> clear();
    if ( recent_names[0] != "<empty>" && ! recent_names[0].isEmpty() )
        open_recent -> insertItem( recent_names[0] );
    if ( recent_names[1] != "<empty>" && ! recent_names[1].isEmpty())
        open_recent -> insertItem( recent_names[1] );
    if ( recent_names[2] != "<empty>" && ! recent_names[2].isEmpty())
        open_recent -> insertItem( recent_names[2] );
    if ( recent_names[3] != "<empty>" && ! recent_names[3].isEmpty())
        open_recent -> insertItem( recent_names[3] );
    if ( recent_names[4] != "<empty>" && ! recent_names[4].isEmpty())
        open_recent -> insertItem( recent_names[4] );
    if ( open_recent -> count() == 0 )
        open_recent -> insertItem( tr( "No Documents" ) );
}
