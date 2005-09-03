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
#include <qstylesheet.h>

#include "ktoon.h"
#include "images.h"
#include "symbol.h"
#include "folder.h"

#include "ktxmlreader.h"
#include "ktconfigdocument.h"
#include "ktfiledialog.h"
#include "ktdebug.h"

// Borrar
#include "kttimeline.h"

//--------------- CONSTRUCTOR --------------------

KToon::KToon() : QMainWindow( 0, "KToon", WDestructiveClose ), document_max_value(1), file_name("")
{
	qDebug("[Initializing KToon]");

	QFile settings( KTCONFIG->configDocument()->path() );
	
	KTXmlReader reader;
	QXmlInputSource xmlsource(&settings);
	
	if ( ! reader.parseXml(&xmlsource))
	{
		qDebug( "Could not open the settings file. Loaded empty recent file names." );
		for(uint i = 0; i < 6; i++)
		{
			recent_names << "<" + QStyleSheet::escape(tr("empty")) + ">";
		}
	}
	else
	{
		XMLTotalResults results = reader.getResults();
		recent_names = results["Recent"];
	}
	
	settings.close();

	setupColors();
	setupIcons();
	setupCursors();
	createActions();
	setupToolBar();
	setupMenu();
	
	KTStatus -> setCurrentCursor( Tools::NORMAL_SELECTION );
	
	statusBar()->message( tr( "Ready." ), 2000 );
	
	main_panel = new QWorkspace( this );
	main_panel->setScrollBarsEnabled ( true );
	main_panel -> setPaletteBackgroundPixmap( QPixmap( background_xpm ) );
	main_panel -> show();
	
	//--------------- Document Object ---------------
	
	KTStatus->currentDocument()-> setNameDocument( tr( "Document" ) + QString( "1" ) );

	KTStatus->setupDrawingArea(main_panel);
	connect(KTStatus->currentDrawingArea(), SIGNAL(useTool(int)), this, SLOT(slotSelectTool(int )));

	setupDialogs();
	
	//------------- Generic Initializations -----------
	
	//resize( 1023, 728 ); // FIXME: Revisar esto!!
	//showMaximized();

	setCentralWidget( main_panel );
//FIXME:kuadrosx 
	//window -> setItemEnabled( id_window_illustration, false );
	
	//------------- Main Connections ----------------

	connect( exposure_sheet_dialog, SIGNAL( layerInserted() ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotInsertLayer() ) );
	
	connect( exposure_sheet_dialog, SIGNAL( layerInserted() ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotInsertFrameSequence() ) );
	
	connect( exposure_sheet_dialog, SIGNAL( layerRemoved() ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotRemoveLayer() ) );
	
	connect( exposure_sheet_dialog, SIGNAL( layerRemoved() ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotRemoveFrameSequence() ) );

	connect( exposure_sheet_dialog, SIGNAL( layerRemoved() ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotUpdateMaxUsedFrames() ) );
	
	connect( exposure_sheet_dialog, SIGNAL( layerSelected( int ) ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotSelectLayerFromES( int ) ) );
	
// 	connect( m_ExposureSheetDialog, SIGNAL( layerSelected( int ) ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotSelectLayerFromES( int ) ) );
	
	
	connect( exposure_sheet_dialog, SIGNAL( layerRenamed( int, const QString & ) ), timeline_dialog -> layerManager() -> layerSequence(), SLOT( slotRenameLayerFromES( int, const QString & ) ) );

	connect( exposure_sheet_dialog, SIGNAL( framesInsertedAtTheEnd( int ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotInsertKeyframesIntoTheCurrentFS( int ) ) );
	
	connect( exposure_sheet_dialog, SIGNAL( frameRemoved( int ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotRemoveKeyframeBlockFromTheCurrentFS( int ) ) );
	
	connect( exposure_sheet_dialog, SIGNAL( frameMovedUp( int ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotMoveKeyframeLeftInTheCurrentFS( int ) ) );
	
	connect( exposure_sheet_dialog, SIGNAL( frameMovedDown( int ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotMoveKeyframeRightInTheCurrentFS( int ) ) );
	
	connect( exposure_sheet_dialog, SIGNAL( frameSelected() ), SLOT( slotActivateCursor() ) );
	
	connect(m_ExposureSheetDialog, SIGNAL(frameSelected()), this, SLOT(slotActivateCursor()));
	
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

	connect(insert_scene, SIGNAL(activated()), scenes_dialog, SLOT(slotInsertScene()));
	
	connect(insert_remove_scene, SIGNAL(activated()), scenes_dialog, SLOT(slotRemoveScene()));
	
	connect(file_close, SIGNAL(activated()), KTStatus->currentDrawingArea(), SLOT(close()));
	
	connect( KTStatus->currentDrawingArea(), SIGNAL( closed() ), SLOT( slotCloseDrawingArea() ) );
	
	connect( color_palette_dialog, SIGNAL( outlineColorChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeOutlineColor() ) );
	
	connect( color_palette_dialog, SIGNAL( fillColorChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeFillColor() ) );
	
	connect( brushes_dialog, SIGNAL( minThicknessChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeMinThicknessBrush() ) );
	
	connect( brushes_dialog, SIGNAL( maxThicknessChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeMaxThicknessBrush() ) );
	
	connect( brushes_dialog, SIGNAL( smoothnessChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeSmoothnessBrush() ) );


	connect( scenes_dialog, SIGNAL( sceneInserted() ), SLOT( slotInsertSync() ) );
	
	connect( scenes_dialog, SIGNAL( sceneMovedDown( int ) ), SLOT( slotMoveDownSync( int ) ) );
	
	connect( scenes_dialog, SIGNAL( sceneMovedUp( int ) ), SLOT( slotMoveUpSync( int ) ) );
	
	connect( scenes_dialog, SIGNAL( sceneRemoved( int ) ), SLOT( slotRemoveSync( int ) ) );
	
	connect( scenes_dialog, SIGNAL( sceneSelected( int ) ), SLOT( slotSelectSync( int ) ) );
	
	connect( KTStatus->currentDrawingArea(), SIGNAL( colorGrabbed( KTColor * ) ), color_palette_dialog, SLOT( slotSetColor( KTColor * ) ) );
	
	connect( KTStatus->currentDrawingArea(), SIGNAL( updated() ), library_dialog -> getSymbolView(), SLOT( updateGL() ) );
	
	connect( KTStatus->currentDrawingArea(), SIGNAL( wasDrawn( bool ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotFrameHasDrawing( bool ) ) );
	
	connect(timeline_dialog, SIGNAL(saveImage(const QString& )), m_cameraPreview, SLOT(grabImage(const QString& )));
	
	scenes_dialog -> selectFirstScene();
	exposure_sheet_dialog -> touchFirstFrame();
	exposure_sheet_dialog -> updateIndicators( exposure_sheet_dialog -> currentLayerObj() );
	
	updateOpenRecentMenu();
	
#if 0
	QFile file( QDir::homeDirPath()+"/.ktmainwindow" );
	if ( file.open( IO_ReadOnly ) ) 
	{
		QTextStream stream( &file );
		stream >> *this;
		file.close();
	}
#endif
}

void KToon::setupColors()
{
//     es_default_color = new QColor( 239, 237, 223 );
//     es_use_color = new QColor( 200, 200, 200 ); //
//     es_selection_color = new QColor( 0, 0, 0 ); // 
//     es_select_and_use_color = new QColor( 255, 255, 255 ); //
//     es_text_color_when_selected = new QColor( 255, 255, 255 ); // 
//     es_default_text_color = new QColor( 0, 0, 0 ); // 
//     es_lock_color = new QColor( 150, 150, 255 );
//     es_lock_and_select_color = new QColor( 210, 210, 255 );
//     es_has_drawing_color = new QColor( 170, 70, 10 );
    
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

void KToon::createActions()
{
	file_new = new QAction( icon_new, tr( "New Document" ), tr("Ctrl+N"), this);
	
	connect(file_new, SIGNAL(activated()), this, SLOT(slotNewDocument()));
	file_new->setStatusTip(tr( "Opens a new document"));
	
	file_open = new QAction( icon_open, tr( "Open Document" ), tr("Ctrl+O"), this);
	connect(file_open, SIGNAL(activated()), this, SLOT(slotChoose()));
	file_open->setStatusTip(tr("Loads an existent document"));	

	file_save = new QAction( icon_save, tr( "Save Document" ),tr("Ctrl+O") , this);
	connect(file_save, SIGNAL(activated()), this, SLOT(slotSave()));
	file_save->setStatusTip(tr("Saves the current document in the current location"));
	
	//
	edit_undo = new QAction( icon_undo, tr( "Undo" ),tr("Ctrl+Z") , this);
	connect(edit_undo, SIGNAL(activated()), this, SLOT(slotUndo()));
	edit_undo->setStatusTip(tr("Undoes the last draw action"));
	
	edit_redo = new QAction( icon_redo, tr( "Redo" ), tr("CTRL+SHIFT+Z"), this);
	connect(edit_redo, SIGNAL(activated()), this, SLOT(slotRedo()));
	edit_redo->setStatusTip(tr("Redoes a previous undone action"));
	
	edit_cut = new QAction( icon_cut, tr( "&Cut" ), tr("Ctrl+X"), this);
	connect(edit_cut, SIGNAL(activated()), this, SLOT(slotCut()));
	edit_cut->setStatusTip(tr("Cuts the selection and puts it onto the clipboard"));
	
	edit_copy = new QAction( icon_copy, tr( "C&opy" ), tr("Ctrl+C"), this);
	connect(edit_copy, SIGNAL(activated()), this, SLOT(slotCopy()));
	edit_copy->setStatusTip(tr("Copies the selection and puts it onto the clipboard"));
	
	edit_paste = new QAction( icon_paste, tr( "&Paste" ), tr("Ctrl+V"), this);
	connect(edit_paste, SIGNAL(activated()), this, SLOT(slotPaste()));
	edit_paste->setStatusTip(tr("Pastes the clipboard into the current document"));
	
	window_drawing_area = new QAction ( icon_drawing_area, tr( "&Drawing Area" ), tr("Ctrl+D"), this);
	connect(window_drawing_area, SIGNAL(activated()), this, SLOT(slotWindowDrawingArea()));
	window_drawing_area->setToggleAction ( true );
	window_drawing_area->setStatusTip(tr("Shows or hides the drawing area widget"));
	window_drawing_area->setOn( true );
	
	window_tools = new QAction( icon_tools, tr( "&Tools" ), tr("Ctrl+T"), this);
	connect(window_tools, SIGNAL(activated()), this, SLOT(slotWindowTools()));
	window_tools->setStatusTip(tr("Shows or hides the tools dialog box"));
	window_tools->setToggleAction ( true ); 
	window_tools->setOn ( true );
	
	window_exposure_sheet = new QAction( icon_exposure_sheet, tr( "&Exposure Sheet" ), tr("Ctrl+H"), this);
	connect(window_exposure_sheet, SIGNAL(activated()), this, SLOT(slotWindowExposureSheet()));
	window_exposure_sheet->setStatusTip(tr("Shows or hides the exposure sheet dialog box"));
	window_exposure_sheet->setToggleAction ( true );
	window_exposure_sheet->setOn ( true );
	
	window_color_palette = new QAction( icon_color_palette, tr( "&Color Palette" ), CTRL+Key_P, this);
	window_color_palette->setStatusTip(tr("Shows or hides the color palette dialog box"));
	connect(window_color_palette, SIGNAL(activated()), this, SLOT(slotWindowColorPalette()));
	window_color_palette->setToggleAction ( true );
	window_color_palette->setOn ( true );

	window_brushes = new QAction( icon_brushes, tr( "Brushes" ),tr("Ctrl+B") , this);
	connect(window_brushes, SIGNAL(activated()), this, SLOT(slotWindowBrushes()));
	window_brushes->setStatusTip(tr("Shows or hides the brushes dialog box"));
	window_brushes->setToggleAction ( true ); 
	window_brushes->setOn ( true );
	window_scenes = new QAction( icon_scenes, tr( "Scenes" ), tr("Ctrl+Y"), this);
	connect(window_scenes, SIGNAL(activated()), this, SLOT(slotWindowScenes()));
	window_scenes->setStatusTip(tr("Shows or hides the scenes dialog box"));
	window_scenes->setToggleAction ( true ); 
	window_scenes->setOn ( true );
	
	window_library = new QAction( icon_library, tr( "Library" ), tr("Ctrl+L"), this);
	connect(window_library, SIGNAL(activated()), this, SLOT(slotWindowLibrary()));
	window_library->setStatusTip(tr("Shows or hides the library dialog box"));
	window_library->setToggleAction(true);
		
	window_render_camera_preview = new QAction( icon_camera_preview, tr( "&Preview Render" ), tr("Ctrl+R"), this);
	window_render_camera_preview->setVisible(false);
	connect(window_render_camera_preview, SIGNAL(activated()), this, SLOT(slotWindowRenderCameraPreview()));
	window_render_camera_preview->setStatusTip(tr("Shows or hides the render preview widget"));
	window_render_camera_preview->setToggleAction(true);
	window_render_camera_preview->setOn(true);
	
	window_top_camera_view = new QAction( icon_top_camera, tr( "&Top Camera View" ), CTRL+Key_Slash, this);
	window_top_camera_view->setVisible(false);
	connect(window_top_camera_view, SIGNAL(activated()), this, SLOT(slotWindowTopCameraView()));
	window_top_camera_view->setStatusTip(tr("Shows or hides the top camera view widget"));
	window_top_camera_view->setToggleAction(true);
	window_top_camera_view->setOn(true);
	
	window_side_camera_view = new QAction( icon_side_camera, tr( "Si&de Camera View" ), CTRL+Key_Asterisk, this);
	window_side_camera_view->setVisible(false);
	connect(window_side_camera_view, SIGNAL(activated()), this, SLOT(slotWindowSideCameraView()));
	window_side_camera_view->setStatusTip(tr("Shows or hides the side camera view widget"));
	window_side_camera_view->setToggleAction(true);
	window_side_camera_view->setOn(true);
	
	
	window_illustration = new QAction( icon_illus_mode, tr( "&Illustration Mode" ), tr("F9") , this);
	connect(window_illustration, SIGNAL(activated()), this, SLOT(slotSeeIllustration()));
	window_illustration->setStatusTip(tr("Turns to the illustration mode" ));
	//FIXME:
 	window_illustration->setVisible(false); 
	
	window_animation = new QAction( icon_ani_mode, tr( "&Animation Mode" ), tr("F10"), this);
	connect(window_animation, SIGNAL(activated()), this, SLOT(slotSeeAnimation()));
	window_animation->setStatusTip(tr("Turns to the animation mode"));
	
	//porque no poner teclas aceleradoras para estas acciones
	QActionGroup *gridGroup = new QActionGroup( this );
	gridGroup->setExclusive( true ); 
	
	view_no_grid = new QAction( icon_no_grid, tr( "&No Grid" ), QString::null, gridGroup);
	connect(view_no_grid, SIGNAL(activated()), this, SLOT(slotNoGrid()));
	view_no_grid->setToggleAction ( true );
	view_no_grid->setStatusTip(tr("Hides the grid" ));
	
	view_grid12 = new QAction( icon_grid12, tr( "&12 Field Grid" ), QString::null, gridGroup);
	view_grid12->setToggleAction ( true );
	connect(view_grid12, SIGNAL(activated()), this, SLOT(slotSeeGrid12()));
	view_grid12->setStatusTip(tr("Shows a 12 field grid" ));
	
	view_grid16 = new QAction( icon_grid16, tr( "&16 Field Grid" ), QString::null, gridGroup);
	view_grid16->setToggleAction ( true );
	connect(view_grid16, SIGNAL(activated()), this, SLOT(slotSeeGrid16()));
	view_grid16->setStatusTip(tr("Shows a 16 field grid" ));
	view_grid16->setOn(true);
	
	view_subgrid = new QAction( icon_subgrid, tr( "&Subgrid" ), QString::null, this),
	view_subgrid->setToggleAction ( true );
	connect(view_subgrid, SIGNAL(activated()), this, SLOT(slotSeeSubgrid()));
	view_subgrid->setStatusTip(tr("Shows or hides a 3 field subgrid in the current grid" ));
	
	
	//FIXME: porque no colocarle teclas aceleradoras a estas acciones
	view_front_back_grid = new QAction( icon_front_back_grid, tr( "Grid to Front/Back" ), QString::null, this);
	view_front_back_grid->setToggleAction ( true );
	view_front_back_grid->setOn(true);
	connect(view_front_back_grid, SIGNAL(activated()), this, SLOT(slotFrontBackGrid()));
	view_front_back_grid->setStatusTip(tr("Sends the grid to the front or to the back of the drawing area" ));
	
	view_ntsc = new QAction( icon_ntsc, tr( "&NTSC Zone" ), QString::null, this);
	view_ntsc->setToggleAction ( true );
	connect(view_ntsc, SIGNAL(activated()), this, SLOT(slotSeeNTSC()));
	view_ntsc->setStatusTip(tr("Shows or hides the NTSC Zone" ));
	
	view_light_table = new QAction( icon_light_table, tr( "&Light Table" ), QString::null, this);
	connect(view_light_table, SIGNAL(activated()), this, SLOT(slotLightTable()));
	view_light_table->setStatusTip(tr("Activates or deactivates the light table" ));
	
	QActionGroup *viewPreviousGroup = new QActionGroup( this );
	viewPreviousGroup->setExclusive( true );
	
	view_no_previous = new QAction( icon_no_previous, tr( "No Previous" ), Key_1, viewPreviousGroup);
	connect(view_no_previous, SIGNAL(activated()), this, SLOT(slotNoPreviousOnionSkin()));
	view_no_previous->setToggleAction ( true );
	view_no_previous->setStatusTip(tr("Disables previous onion skin visualization"));
	
	view_previous = new QAction( icon_previous, tr( "Previous One" ), Key_2, viewPreviousGroup);
	connect(view_previous, SIGNAL(activated()), this, SLOT(slotPreviousOnionSkin()));
	view_previous->setStatusTip(tr("Shows the previous onion skin" ));
	view_previous->setToggleAction ( true );
	
	view_previous2 = new QAction( icon_previous2, tr( "Previous Two" ), Key_3, viewPreviousGroup);
	connect(view_previous2, SIGNAL(activated()), this, SLOT(slotPrevious2OnionSkin()));
	view_previous2->setStatusTip(tr("Shows the previous 2 onion skins" ));
	view_previous2->setToggleAction ( true );
	
	view_previous3 = new QAction( icon_previous3, tr( "Previous Three" ), Key_4, viewPreviousGroup);
	view_previous3->setToggleAction ( true );
	connect(view_previous3, SIGNAL(activated()), this, SLOT(slotPrevious3OnionSkin()));
	view_previous3->setStatusTip(tr("Shows the previous 3 onion skins" ));
	
	view_no_previous->setOn( true );
	
	QActionGroup *viewNextGroup = new QActionGroup( this );
	viewNextGroup->setExclusive( true );
	
	
	view_no_next = new QAction( icon_no_next, tr( "No Next" ), CTRL+Key_1, viewNextGroup);
	view_no_next->setToggleAction ( true );
	connect(view_no_next, SIGNAL(activated()), this, SLOT(slotNoNextOnionSkin()));
	view_no_next->setStatusTip(tr("Disables next onion skin visualization" ));
	
	view_next = new QAction( icon_next, tr( "Next One" ), CTRL+Key_2, viewNextGroup);
	view_next->setToggleAction ( true );
	connect(view_next, SIGNAL(activated()), this, SLOT(slotNextOnionSkin()));
	view_next->setStatusTip(tr("Shows the next onion skin"));
	
	view_next2 = new QAction( icon_next2, tr( "Next Two" ), CTRL+Key_3, viewNextGroup);
	view_next2->setToggleAction(true );
	connect(view_next2, SIGNAL(activated()), this, SLOT(slotNext2OnionSkin()));
	view_next2->setStatusTip(tr("Shows the next 2 onion skins"));
	
	view_next3 = new QAction( icon_next3, tr( "Next Three" ), CTRL+Key_4, viewNextGroup);
	view_next3->setToggleAction(true );
	connect(view_next3, SIGNAL(activated()), this, SLOT(slotNext3OnionSkin()));
	view_next3->setStatusTip(tr("Shows the next 3 onion skins"));
	view_no_next->setOn( true );
	
	
	//create action to menu
	file_save_as = new QAction( tr( "Save &As..." ), QString::null, this);
	connect(file_save_as, SIGNAL(activated()), this, SLOT(slotSaveAs()));
	file_save_as->setStatusTip(tr("Opens a dialog box to save the current document in any location"));
	
	file_close = new QAction(icon_close, tr( "Cl&ose" ), tr("Ctrl+W"), this);
	file_close->setStatusTip(tr("Closes the active document"));
	
	file_import = new QAction(icon_import, tr( "&Import..." ),  tr("Ctrl+I"), this);
	connect(file_import, SIGNAL(activated()), this, SLOT(slotImport()));
	file_import->setStatusTip(tr("Imports a file in the supported format"));
	
	file_export = new QAction(icon_export, tr( "&Export..." ),  tr("Ctrl+E"), this);
	connect(file_export, SIGNAL(activated()), this, SLOT(slotExport()));
	file_export->setStatusTip(tr("Exports this document as a file in the available formats"));

	file_export->setVisible(false);
	file_properties = new QAction( tr( "&Properties..." ),  QString::null, this);
	connect(file_properties, SIGNAL(activated()), this, SLOT(slotProperties()));
	file_properties->setStatusTip(tr("Opens the properties dialog box"));
	
	file_exit = new QAction(icon_export, tr( "E&xit" ),  tr("Ctrl+Q"), this);
	connect(file_exit, SIGNAL(activated()), this, SLOT(close()));
	file_exit->setStatusTip(tr("Closes the application"));
	
	edit_paste_in_place = new QAction( tr(  "Paste &In Place" ),  tr("Ctrl+Shift+V"), this);
	connect(edit_paste_in_place, SIGNAL(activated()), this, SLOT(slotPasteInPlace()));
	edit_paste_in_place->setStatusTip(tr("Pastes the clipboard into the same place as the copy was did"));
	
	edit_delete = new QAction( tr(  "&Delete" ), Key_Delete , this);
	connect(edit_delete, SIGNAL(activated()), this, SLOT(slotDelete()));
	edit_delete->setStatusTip(tr("Deletes the selected object"));
	
	edit_select_all = new QAction( tr(  "&Select All" ),  tr("Ctrl+A"), this);
	connect(edit_select_all, SIGNAL(activated()), this, SLOT(slotSelectAll()));
	edit_select_all->setStatusTip(tr("Selects all objects in the document"));
	
	edit_preferences = new QAction( tr( "Pr&eferences..." ), QString::null, this);
	connect(edit_preferences, SIGNAL(activated()), this, SLOT( slotPreferences()));
	edit_preferences->setStatusTip(tr("Opens the preferences dialog box"));
	
	view_zoom_in = new QAction(icon_zoom_in, tr( "Zoom &In" ),tr("Ctrl++") , this);
	connect(view_zoom_in, SIGNAL(activated()), this, SLOT( slotZoomIn()));
	view_zoom_in->setStatusTip(tr("Performs a Zoom In of the drawing area"));
	
	view_zoom_out = new QAction(icon_zoom_out, tr( "Zoom &Out" ),tr("Ctrl+-") , this);
	connect(view_zoom_out, SIGNAL(activated()), this, SLOT( slotZoomOut()));
	view_zoom_out->setStatusTip(tr("Performs a Zoom Out of the drawing area"));
	
	view_fit = new QAction(icon_fit, tr( "&Fit" ), QString::null, this);
	connect(view_fit, SIGNAL(activated()), this, SLOT( slotFit()));
	view_fit->setStatusTip(tr("Fits the grid into the drawing area"));
	
	view_rotate_clockwise = new QAction( tr( "Rotate &Clockwise" ), tr("V"), this);
	connect(view_rotate_clockwise, SIGNAL(activated()), this, SLOT( slotRotateClockwise()));
	view_rotate_clockwise->setStatusTip(tr("Rotates the scene 30 degrees clockwise"));
	
	view_rotate_counterclockwise = new QAction( tr( "Rotate Counterclock&wise" ), tr("C"), this);
	connect(view_rotate_counterclockwise, SIGNAL(activated()), this, SLOT( slotRotateCounterClockwise()));
	view_rotate_counterclockwise->setStatusTip(tr("Rotates the scene 30 degrees counterclockwise"));
	
	view_restore_rotation = new QAction( tr( "&Restore Rotation" ), tr("Shift+C"), this);
	connect(view_restore_rotation, SIGNAL(activated()), this, SLOT( slotRestoreRotation()));
	view_restore_rotation->setStatusTip(tr("Restores any rotation performed on the scene"));
	
	insert_layer = new QAction( tr( "&Layer" ), QString::null, this);
	connect(insert_layer, SIGNAL(activated()), this, SLOT( slotInsertLayer()));
	insert_layer->setStatusTip(tr("Inserts a new layer into the Exposure Sheet and Timeline"));
	
	insert_frame = new QAction( tr( "&Frame" ), tr("F5"), this);
	connect(insert_frame, SIGNAL(activated()), this, SLOT( slotInsertFrame()));
	insert_frame->setStatusTip(tr("Inserts a frame into the current layer of the Timeline"));
	insert_frame->setVisible(false);
	
	insert_remove_frame = new QAction( tr( "&Remove Frame" ), tr("Shift+F5"), this);
	connect(insert_frame, SIGNAL(activated()), this, SLOT( slotRemoveFrame()));
	insert_remove_frame->setStatusTip(tr("Removes a frame from the current layer of the Timeline"));
	insert_remove_frame->setVisible(false);
	
	insert_keyframe = new QAction( tr( "&KeyFrame" ), tr("F6"), this);
	connect(insert_frame, SIGNAL(activated()), this, SLOT( slotInsertKeyFrame()));
	insert_keyframe->setStatusTip(tr("Inserts a keyframe into the ExposureSheet and Timeline"));
	
	insert_scene = new QAction( tr( "&Scene" ), QString::null, this);
	insert_scene->setStatusTip(tr("Inserts a new scene"));
	
	insert_remove_scene = new QAction( tr( "Re&move  Scene" ), QString::null, this);
	insert_remove_scene->setStatusTip(tr("Removes the selected scene"));

	tools_selection = new QActionGroup( this );
	tools_selection->setText(tr( "Selection" ));
	tools_selection->setMenuText( tr( "&Selection" ) );
	tools_selection->setExclusive( FALSE );
	tools_selection->setUsesDropDown( TRUE );
	
	tools_normal_selection = new QAction(icon_selection, tr( "Normal &Selection" ), tr("S"), tools_selection);
	connect(tools_normal_selection, SIGNAL(activated()), this, SLOT( slotNormalSelection()));
	tools_normal_selection->setStatusTip(tr("Activates the Selection tool"));
	
	tools_contour_selection = new QAction(icon_nodes, tr( "Con&tour Selection" ), tr("T"), tools_selection);
	connect(tools_contour_selection, SIGNAL(activated()), this, SLOT( slotContourSelection()));
	tools_contour_selection->setStatusTip(tr("Activates the Node Selection tool"));
	
	tools_draw = new QActionGroup( this );
	tools_draw->setText(tr( "Draw" ));
	tools_draw->setMenuText( tr( "D&raw" ) );
	tools_draw->setExclusive( FALSE );
	tools_draw->setUsesDropDown( TRUE );
	
	tools_brush = new QAction(icon_brush, tr( "&Brush" ), tr("B"), tools_draw);
	connect(tools_brush, SIGNAL(activated()), this, SLOT( slotBrush()));
	tools_brush->setStatusTip(tr("Activates the Brush tool"));
	
	tools_pencil = new QAction(icon_pencil, tr( "&Pencil" ), tr("P"), tools_draw);
	connect(tools_pencil, SIGNAL(activated()), this, SLOT( slotPencil()));
	tools_pencil->setStatusTip(tr("Activates the Pencil tool"));
	
	tools_pen = new QAction(icon_pen, tr( "&Pen" ), tr("N"), tools_draw);
	connect(tools_pen, SIGNAL(activated()), this, SLOT( slotPen()));
	tools_pen->setStatusTip(tr("Activates the Pen tool"));
	
	tools_line = new QAction(icon_line, tr( "&line" ), tr("L"), tools_draw);
	connect(tools_line, SIGNAL(activated()), this, SLOT( slotLine()));
	tools_line->setStatusTip(tr("Activates the Line tool"));
	
	tools_rectangle = new QAction(icon_rectangle, tr( "&Rectangle" ), tr("R"), tools_draw);
	connect(tools_rectangle, SIGNAL(activated()), this, SLOT( slotRectangle()));
	tools_rectangle->setStatusTip(tr("Activates the Rectangle tool"));
	
	tools_ellipse = new QAction(icon_ellipse, tr( "&Ellipse" ), tr("E"), tools_draw);
	connect(tools_ellipse, SIGNAL(activated()), this, SLOT( slotEllipse()));
	tools_ellipse->setStatusTip(tr("Activates the Ellipse tool"));
	
	tools_fills = new QActionGroup( this );
	tools_fills->setText(tr( "Fill" ));
	tools_fills->setMenuText( tr( "&Fill" ) );
	tools_fills->setExclusive( FALSE );
	tools_fills->setUsesDropDown( TRUE );
	
	tools_fill = new QAction(icon_fill, tr( "&Fill" ), tr("F"), tools_fills);
	connect(tools_fill, SIGNAL(activated()), this, SLOT( slotFill()));
	tools_fill->setStatusTip(tr("Activates the Fill tool"));
	
	tools_remove_fill = new QAction(icon_remove_fill, tr( "&Remove Fill" ), tr("Shift+F"), tools_fills);
	connect(tools_remove_fill, SIGNAL(activated()), this, SLOT( slotRemoveFill()));
	tools_remove_fill->setStatusTip(tr("Activates the Remove Fill tool"));
	
	tools_contour_fill = new QAction(icon_contour_fill, tr( "&Contour Fill" ), tr("Ctrl+F"), tools_fills);
	connect(tools_contour_fill, SIGNAL(activated()), this, SLOT( slotContourFill()));
	tools_contour_fill->setStatusTip(tr("Activates the Contour Fill tool"));
	
	tools_dropper = new QAction(icon_dropper, tr( "&Dropper" ), tr("D"), tools_fills);
	connect(tools_dropper, SIGNAL(activated()), this, SLOT( slotDropper()));
	tools_dropper->setStatusTip(tr("Activates the Dropper tool"));
	
	tools_erasers = new QActionGroup( this );
	tools_erasers->setText(tr( "Eraser" ));
	tools_erasers->setMenuText( tr( "&Eraser" ) );
	tools_erasers->setExclusive( FALSE );
	tools_erasers->setUsesDropDown( TRUE );
	
	tools_eraser = new QAction(icon_eraser, tr( "&Eraser" ) , CTRL+Key_Delete, tools_erasers);
	connect(tools_eraser, SIGNAL(activated()), this, SLOT( slotEraser()));
	tools_eraser->setStatusTip(tr("Activates the Eraser tool"));
	
	tools_slicer = new QAction(icon_dropper, tr( "&Slicer" ), CTRL+Key_Delete, tools_erasers);
	connect(tools_slicer, SIGNAL(activated()), this, SLOT( slotSlicer()));
	tools_slicer->setStatusTip(tr("Activates the Slicer tool"));
	
	
	tools_view = new QActionGroup( this );
	tools_view->setText(tr( "View" ));
	tools_view->setMenuText( tr( "&View" ) );
	tools_view->setExclusive( FALSE );
	tools_view->setUsesDropDown( TRUE );
 	
	tools_magnifying_glass = new QAction(icon_magnifying_glass, tr( "&Magnifying Glass" ), tr("M"), tools_view);
	
	connect(tools_magnifying_glass, SIGNAL(activated()), this, SLOT( slotMagnifyingGlass()));
	tools_magnifying_glass->setStatusTip(tr("Activates the Magnifying Glass tool"));
	
	tools_hand = new QAction(icon_hand, tr( "&Hand" ), tr("H"), tools_view);
	connect(tools_hand, SIGNAL(activated()), this, SLOT( slotHand()));
	tools_hand->setStatusTip(tr("Activates the Hand tool"));
	
	tools_group = new QAction(icon_group, tr( "&Group" ), QString::null, this);
	connect(tools_group, SIGNAL(activated()), this, SLOT( slotGroup()));
	tools_group->setStatusTip(tr("Group the selected objects into a single one"));
	
	tools_ungroup = new QAction(icon_ungroup, tr( "&Ungroup" ), QString::null, this);
	connect(tools_ungroup, SIGNAL(activated()), this, SLOT( slotUngroup()));
	tools_ungroup->setStatusTip(tr("Ungroups the selected object"));
	
	tools_order = new QActionGroup( this );
	tools_order->setText(tr( "Order" ));
	tools_order->setMenuText( tr( "&Order" ) );
	tools_order->setExclusive( FALSE );
	tools_order->setUsesDropDown( TRUE );
	
	tools_bring_front = new QAction(icon_bring_to_front, tr( "&Bring to Front" ), CTRL+SHIFT+Key_Up, tools_order);
	connect(tools_bring_front, SIGNAL(activated()), this, SLOT( slotBringToFront()));
	tools_bring_front->setStatusTip(tr("Brings the selected object to the front"));

	tools_send_back = new QAction(icon_send_to_back, tr( "&Send to Back" ), CTRL+SHIFT+Key_Down, tools_order);
	connect(tools_send_back, SIGNAL(activated()), this, SLOT( slotSendToBack()));
	
	tools_send_back->setStatusTip(tr("Sends the selected objects to the back"));
	tools_one_step_forward = new QAction(icon_one_forward, tr( "One Step &Forward" ), CTRL+Key_Up, tools_order);
	connect(tools_one_step_forward, SIGNAL(activated()), this, SLOT( slotOneStepForward()));
	tools_one_step_forward->setStatusTip(tr("Moves the selected object one step forward"));
	
	tools_one_step_backward = new QAction(icon_one_backward, tr( "One Step B&ackward" ), CTRL+Key_Down, tools_order);
	connect(tools_one_step_backward, SIGNAL(activated()), this, SLOT( slotOneStepBackward()));
	tools_one_step_backward->setStatusTip(tr("Moves the selected object one step backward"));
	
	
	tools_align = new QActionGroup( this );
	tools_align->setText(tr( "Align" ));
	tools_align->setMenuText( tr( "A&lign" ) );
	tools_align->setExclusive( FALSE );
	tools_align->setUsesDropDown( TRUE );
	
	tools_left = new QAction(icon_align_l, tr( "&Left" ),  QString::null, tools_align);
	connect(tools_left, SIGNAL(activated()), this, SLOT( slotAlignLeft()));
	tools_left->setStatusTip(tr("Aligns the selected object to the left"));
	
	tools_center_vertically = new QAction(icon_align_cv, tr( "&Center Vertically" ),  QString::null, tools_align);
	connect(tools_center_vertically, SIGNAL(activated()), this, SLOT( slotCenterVertically()));
	tools_center_vertically->setStatusTip(tr("Centers vertically the selected object"));
	
	tools_right = new QAction(icon_align_r, tr( "&Right" ),  QString::null, tools_align);
	connect(tools_right, SIGNAL(activated()), this, SLOT( slotAlignRight()));
	tools_right->setStatusTip(tr("Aligns the selected object to the right"));
	
	tools_align->addSeparator();
	
	tools_top = new QAction(icon_align_t, tr( "&Top" ),  QString::null, tools_align);
	connect(tools_top, SIGNAL(activated()), this, SLOT( slotAlignTop()));
	tools_top->setStatusTip(tr("Aligns the selected object to the top"));
	
	tools_center_horizontally = new QAction(icon_align_ch, tr( "Center &Horizontally" ),  QString::null, tools_align);
	connect(tools_center_horizontally, SIGNAL(activated()), this, SLOT( slotCenterHorizontally()));
	tools_center_horizontally->setStatusTip(tr("Centers horizontally the selected object"));
	
	tools_bottom = new QAction(icon_align_b, tr( "&Bottom" ),  QString::null, tools_align);
	connect(tools_bottom, SIGNAL(activated()), this, SLOT( slotAlignBottom()));
	tools_bottom->setStatusTip(tr("Aligns the selected object to the bottom"));
	
	tools_transform = new QActionGroup( this );
	tools_transform->setText(tr( "Transform " ));
	tools_transform->setMenuText( tr( "&Transform" ) );
	tools_transform->setExclusive( FALSE );
	tools_transform->setUsesDropDown( TRUE );
	
	tools_flip_horizontally = new QAction(tr( "Flip &Horizontally" ),  QString::null, tools_transform);
	connect(tools_flip_horizontally, SIGNAL(activated()), this, SLOT( slotFlipHorizontally()));
	tools_flip_horizontally->setStatusTip(tr("Flips the selected object horizontally"));
	
	tools_flip_vertically = new QAction(tr( "Flip &Vertically" ),  QString::null, tools_transform );
	connect(tools_flip_vertically, SIGNAL(activated()), this, SLOT( slotFlipVertically()));
	tools_flip_vertically->setStatusTip(tr("Flips the selected object vertically"));
	
	tools_transform->addSeparator();
	
	tools_rotate_cw90 = new QAction(tr( "&Rotate 90 CW" ),  QString::null, tools_transform);
	connect(tools_rotate_cw90, SIGNAL(activated()), this, SLOT( slotRotateCW90()));
	tools_rotate_cw90->setStatusTip(tr("Rotates the selected object 90 degrees clockwise"));
	
	tools_rotate_ccw90 = new QAction(tr( "&Rotate 90 CCW" ),  QString::null, tools_transform);
	connect(tools_rotate_ccw90, SIGNAL(activated()), this, SLOT( slotRotateCCW90()));
	tools_rotate_ccw90->setStatusTip(tr("Rotates the selected object 90 degrees counterclockwise"));
	
	tools_transform->addSeparator();
	
	tools_rotate180 = new QAction(tr( "&Rotate &180" ),  QString::null, tools_transform);
	connect(tools_rotate180, SIGNAL(activated()), this, SLOT( slotRotate180()));
	tools_rotate180->setStatusTip(tr("Rotates the selected object 180 degrees"));
	
	tools_perspective = new QAction(icon_perspective, tr( "&Perspective" ),  QString::null, tools_transform);
	connect(tools_perspective, SIGNAL(activated()), this, SLOT( slotPerspectiveSelection()));
	tools_perspective->setStatusTip(tr("Activates the perspective tool"));
	
	control_play = new QAction(tr( "&Play/Stop" ),  Key_Return, this);
	connect(control_play, SIGNAL(activated()), this, SLOT( slotPlayStop()));
	control_play->setStatusTip(tr("Plays the Animation"));
	
	control_rewind = new QAction(tr( "&Rewind" ),  CTRL+Key_Comma, this);
	connect(control_rewind, SIGNAL(activated()), this, SLOT( slotRewind()));
	control_rewind->setStatusTip(tr("Rewinds the Animation"));
	
	control_go_to_end = new QAction(tr( "Go to &End" ),  CTRL+Key_Period, this);
	connect(control_go_to_end, SIGNAL(activated()), this, SLOT( slotGoToEnd()));
	control_go_to_end->setStatusTip(tr("Goes to the last frame of the Animation"));
	
	control_step_forward = new QAction(tr( "Step &Forward" ),  Key_Period, this);
	connect(control_step_forward, SIGNAL(activated()), this, SLOT( slotStepForward()));
	control_step_forward->setStatusTip(tr("Steps one frame forward in the animation"));
	
	control_step_backward = new QAction(tr( "Step &Backward" ), Key_Comma, this);
	connect(control_step_backward, SIGNAL(activated()), this, SLOT( slotStepBackward()));
	control_step_backward->setStatusTip(tr("Goes to the last frame of the Animation"));
	help_contents = new QAction(tr( "&Contents" ),  tr("F1"), this);
	connect(help_contents, SIGNAL(activated()), this, SLOT( slotContents()));
	help_contents->setStatusTip(tr("Shows the help of the application"));
	
	help_about = new QAction(tr( "&About K-Toon..." ), QString::null, this);
	connect(help_about, SIGNAL(activated()), this, SLOT( slotAbout()));
	help_about->setStatusTip(tr("Shows information about the authors and another stuff"));
	
	help_about_qt = new QAction(tr( "About &Qt..." ), QString::null, this);
	connect(help_about_qt, SIGNAL(activated()), qApp, SLOT( aboutQt()));
	help_about_qt->setStatusTip(tr("Shows information about Qt"));
	
	help_about_opengl = new QAction(tr( "About &OpenGL..." ), QString::null, this);
	connect(help_about_opengl, SIGNAL(activated()), this, SLOT( slotAboutOpenGL()));
	help_about_opengl->setStatusTip(tr("Shows information about OpenGL"));
}

void KToon::setupToolBar()
{
	tool_bar = new QToolBar( this );
	tool_bar -> setLabel( tr( "File and Edit Operations" ) );
	file_new->addTo(tool_bar);
	file_open->addTo(tool_bar);
	file_save->addTo(tool_bar);
	tool_bar -> addSeparator();
	edit_undo->addTo(tool_bar);
	edit_redo->addTo(tool_bar);
	tool_bar -> addSeparator();
	edit_cut->addTo(tool_bar);
	edit_copy->addTo(tool_bar);
	edit_paste->addTo(tool_bar);
	window_drawing_area->addTo(tool_bar);
	tool_bar -> addSeparator();
	window_tools->addTo(tool_bar);
	window_exposure_sheet->addTo(tool_bar);
	window_color_palette->addTo(tool_bar);
	window_brushes->addTo(tool_bar);
	window_scenes->addTo(tool_bar);
	window_library->addTo(tool_bar);
	
	window_timeline = new QToolButton( icon_time_line, tr( "Timeline" ), QString::null, this, SLOT( slotWindowTimeline() ), tool_bar );
	window_timeline -> hide();
	window_render_camera_preview->addTo(tool_bar);
	window_top_camera_view->addTo(tool_bar);
	window_side_camera_view->addTo(tool_bar);
	tool_bar -> addSeparator();
	window_illustration->addTo(tool_bar);
	window_animation->addTo(tool_bar);

	tool_bar2 = new QToolBar( this );
	tool_bar2 -> setLabel( tr( "View Operations" ) );

	view_no_grid->addTo(tool_bar2);
	view_grid12->addTo(tool_bar2);
	view_grid16->addTo(tool_bar2);
	view_subgrid->addTo(tool_bar2);
	view_front_back_grid->addTo(tool_bar2);
	tool_bar2 -> addSeparator();
	view_ntsc->addTo(tool_bar2);
	view_light_table->addTo(tool_bar2);
	tool_bar2 -> addSeparator();
	view_no_previous->addTo(tool_bar2);
	view_previous->addTo(tool_bar2);
	view_previous2->addTo(tool_bar2);
	view_previous3->addTo(tool_bar2);
	tool_bar2->addSeparator();
	view_no_next->addTo(tool_bar2);
	view_next->addTo(tool_bar2);
	view_next2->addTo(tool_bar2);
	view_next3->addTo(tool_bar2);
	
// 	tool_bar3 = new QToolBar( this );
// 	tool_bar3-> setLabel( tr( "Tools Operations" ) );
// 	tools_selection->addTo(tool_bar3);
// 	tools_draw->addTo(tool_bar3);
// 	tools_fills->addTo(tool_bar3);
// 	tools_erasers->addTo(tool_bar3);
// 	tools_view->addTo(tool_bar3);
}

void KToon::setupMenu()
{
	//FILE MENU
	file = new QPopupMenu( this );
	id_file = menuBar() -> insertItem( tr("&File"), file );
	file_new->addTo(file);
	file_open->addTo(file);
	open_recent = new QPopupMenu( this );
	connect( open_recent, SIGNAL( activated( int ) ), SLOT( slotOpenRecent( int ) ) );
	file->insertItem( tr( "Open Recent" ), open_recent );
	file_save->addTo(file);
	file_save_as->addTo(file);
	file_close->addTo(file);
	file->insertSeparator();
	file_import->addTo(file);
	file_export->addTo(file);
	file->insertSeparator();
	file_properties->addTo(file);
	file->insertSeparator();
	file_exit->addTo(file);
	file -> insertSeparator();
	//EDIT MENU
	edit = new QPopupMenu( this );
	id_edit = menuBar() -> insertItem( tr( "&Edit" ), edit );
	edit -> insertSeparator();
	edit_cut->addTo(edit);
	edit_copy->addTo(edit);
	edit_paste->addTo(edit);
	edit_paste_in_place->addTo(edit);
	edit_delete->addTo(edit);
	edit->insertSeparator();
	edit_select_all->addTo(edit);
	edit -> insertSeparator();
	edit_preferences->addTo(edit);
	//VIEW MENU
	view = new QPopupMenu( this );
	id_view = menuBar() -> insertItem( tr( "&View" ), view );
	view_zoom_in->addTo(view);
	view_zoom_out->addTo(view);
	view_fit->addTo(view);
	view->insertSeparator();
	//Grid Submenu
	grid = new QPopupMenu( this );
	view_no_grid->addTo(grid);
	view_grid12->addTo(grid);
	view_grid16->addTo(grid);
	view_subgrid->addTo(grid);
	
	view -> insertItem( tr( "&Grid" ), grid );
	view_front_back_grid->addTo(view);
	view_ntsc->addTo(view);
	view_light_table->addTo(view);
	onion_skin = new QPopupMenu( this );

	view_no_previous->addTo(onion_skin);
	view_previous->addTo(onion_skin);
	view_previous2->addTo(onion_skin);
	view_previous3->addTo(onion_skin);
	view_no_next->addTo(onion_skin);
	view_next->addTo(onion_skin);
	view_next2->addTo(onion_skin);
	view_next3->addTo(onion_skin);
	
	view->insertItem( tr( "Onion &Skin" ), onion_skin );
	view->insertSeparator();
	view_rotate_clockwise->addTo(view);
	view_rotate_counterclockwise->addTo(view);
	view_restore_rotation->addTo(view);
    //INSERT MENU
	insert = new QPopupMenu( this );
	id_insert = menuBar() -> insertItem( tr( "&Insert" ), insert );
	insert_layer->addTo(insert);
	insert -> insertSeparator();
	insert_frame->addTo(insert);
	insert_remove_frame->addTo(insert);
	insert_keyframe->addTo(insert);
	insert->insertSeparator();
	insert_scene->addTo(insert);
	insert_remove_scene->addTo(insert);
    //TOOLS MENU
	tools = new QPopupMenu( this );
	tools_selection->addTo(tools);
	tools_draw->addTo(tools);
	tools_fills->addTo(tools);
	tools_erasers->addTo(tools);
	tools_view->addTo(tools);
	tools->insertSeparator();
	tools_group->addTo(tools);
	tools_ungroup->addTo(tools);
	tools->insertSeparator();
	
	id_tools = menuBar() -> insertItem( tr( "&Tools" ), tools );
	QPopupMenu *order = new QPopupMenu( this );
	
	tools_order->addTo(tools);
	tools_align->addTo(tools);
	tools_transform->addTo(tools);
    //CONTROL MENU
	control = new QPopupMenu( this );
	id_control = menuBar() -> insertItem( tr( "&Control" ), control );
	control_play->addTo(control);
	control_rewind->addTo(control);
	control_go_to_end->addTo(control);
	control -> insertSeparator();
	control_step_forward->addTo(control);
	control_step_backward->addTo(control);
	menuBar() -> setItemVisible( id_control, false );
    //WINDOW MENU
	window = new QPopupMenu( this );
	id_window = menuBar() -> insertItem( tr( "&Window" ), window );
	window_illustration->addTo(window);
	window_animation->addTo(window);
	window -> insertSeparator();
	window_drawing_area->addTo(window);
	window_tools->addTo(window);
	window_exposure_sheet->addTo(window);
	window_color_palette->addTo(window);
	window_brushes->addTo(window);	
	window_scenes->addTo(window);
	window_library->addTo(window);
	id_window_timeline = window -> insertItem( icon_time_line, tr( "T&imeline" ), this, SLOT( slotWindowTimeline() ), CTRL+Key_K );
	window -> setItemVisible( id_window_timeline, false );
	
	window_render_camera_preview->addTo(window);
	window_top_camera_view->addTo(window);
	window_side_camera_view->addTo(window);
    //HELP MENU
	help = new QPopupMenu( this );
	id_help = menuBar() -> insertItem( tr( "&Help" ), help );
	help_contents->addTo(help);	
	help -> insertSeparator();
	help_about->addTo(help);
	help_about_qt->addTo(help);
	help_about_opengl->addTo(help);

}

void KToon::setupDialogs()
{
	    //--------------- Dialog Boxes -----------------
	
	KTDialogMenu *m_menu = new KTDialogMenu(this);
	
	setDockEnabled(Qt::DockLeft, true);
	setDockEnabled(Qt::DockRight, true);
	setDockEnabled(Qt::DockBottom, true);
	setDockEnabled(Qt::DockTop, true);
	
	leftDock ()->setMinimumSize(main_panel->height(), 30 );
// 	leftDock ()->setBackgroundColor( Qt::green);
	
    //For Illustration
	exposure_sheet_dialog = new ExposureSheet( this);//, Qt::WStyle_Tool, window, id_window_exposure_sheet, window_exposure_sheet );
	exposure_sheet_dialog->show();
	QObject::connect(exposure_sheet_dialog, SIGNAL(activate(bool)),
			 window_exposure_sheet, SLOT(setOn(bool)));
	
	m_ExposureSheetDialog =  new KTExposureSheet( this);
// 	m_ExposureSheetDialog->show();
	QObject::connect(m_ExposureSheetDialog, SIGNAL(activate(bool)),
			 window_exposure_sheet, SLOT(setOn(bool)));
			
	list_of_es.append( exposure_sheet_dialog );
    
	moveDockWindow(exposure_sheet_dialog, Qt::DockRight);
	exposure_sheet_dialog->undock();
	exposure_sheet_dialog->move(600,50);
	
	
	tools_dialog = new Tools( this);//, Qt::WStyle_Tool, window, id_window_tools, window_tools );
	tools_dialog -> show();
	QObject::connect(tools_dialog, SIGNAL(activate(bool)),
			   window_tools, SLOT(setOn(bool)));
	
	scenes_dialog = new Scenes( this);//, Qt::WStyle_Tool, window, id_window_scenes, window_scenes );
	scenes_dialog -> show();
	QObject::connect(scenes_dialog, SIGNAL(activate(bool)),
			 window_scenes, SLOT(setOn(bool)));
	
	color_palette_dialog = new ColorPalette( this);//, Qt::WStyle_Tool, window, id_window_color_palette, window_color_palette );

	QObject::connect(color_palette_dialog, SIGNAL(activate(bool)),
			 window_color_palette, SLOT(setOn(bool)));
    
	moveDockWindow(color_palette_dialog, Qt::DockLeft);
	color_palette_dialog->undock();
	color_palette_dialog -> show();
    
	brushes_dialog = new Brushes( this);//, Qt::WStyle_Tool/*, window, id_window_brushes, window_brushes */);
	brushes_dialog -> show();
	QObject::connect(brushes_dialog, SIGNAL(activate(bool)),
 			 window_brushes, SLOT(setOn(bool)));
	
	library_dialog = new Library( this, KTStatus->currentDrawingArea());
	QObject::connect(library_dialog, SIGNAL(activate(bool)),
			 window_library , SLOT(setOn(bool)));
	
#if 0
	m_viewDocument = new KTViewDocument(main_panel);

	QObject::connect(this, SIGNAL(changedCursor(QCursor)),
			 m_viewDocument , SLOT(setCursor(QCursor)));
#endif

	m_cameraPreview = new KTCameraPreview(main_panel);
	m_cameraPreview -> hide();
	QObject::connect(m_cameraPreview, SIGNAL(activate(bool)),
			 window_render_camera_preview, SLOT(setOn(bool)));
	
	//For animation
	
 	/*KTTimeLine *kttimeLine = */new KTTimeLine(this);
	
	timeline_dialog = new Timeline( this, Qt::WStyle_Tool, window, id_window_timeline, window_timeline );
	list_of_tl.append( timeline_dialog );
	


	//FIXME: kuadrosx
	top_camera_view = new GLTopCameraView( main_panel, KTStatus->currentDrawingArea() );
	top_camera_view -> hide();
	QObject::connect(top_camera_view, SIGNAL(activate(bool)),
			 window_top_camera_view , SLOT(setOn(bool)));
	
 	side_camera_view = new GLSideCameraView( main_panel, KTStatus->currentDrawingArea() );
	side_camera_view -> hide();
	QObject::connect(side_camera_view, SIGNAL(activate(bool)),
			 window_side_camera_view , SLOT(setOn(bool)));
}

//-------------- DESTRUCTOR -----------------

KToon::~KToon()
{
	qDebug("[Destroying KToon]");

#if 0
	// Guardamos la poscicion de los widgets
	QFile file( QDir::homeDirPath()+"/.ktmainwindow" );
	if ( file.open( IO_WriteOnly ) ) 
	{
		QTextStream stream( &file );
		stream << *this;
		file.close();
	}
#endif
	
//     delete file;
//     delete open_recent;
//     delete edit;
//     delete view;
//     delete grid;
//     delete onion_skin;
//     delete insert;
//     delete tools;
//     delete control;
//     delete window;
//     delete help;
//     delete file_new;
//     delete file_open;
//     delete file_save;
//     delete edit_undo;
//     delete edit_redo;
//     delete edit_cut;
//     delete edit_copy;
//     delete edit_paste;
//     delete view_no_grid;
//     delete view_grid12;
//     delete view_grid16;
//     delete view_subgrid;
//     delete view_no_previous;
//     delete view_previous;
//     delete view_previous2;
//     delete view_previous3;
//     delete view_no_next;
//     delete view_next;
//     delete view_next2;
//     delete view_next3;
//     delete view_front_back_grid;
//     delete view_ntsc;
//     delete view_light_table;
//     delete window_illustration;
//     delete window_animation;
//     delete window_tools;
//     delete window_brushes;
//     delete window_scenes;
//     delete window_color_palette;
//     delete window_exposure_sheet;
//     delete window_library;
//     delete window_drawing_area;
//     delete window_timeline;
//     delete window_render_camera_preview;
//     delete window_top_camera_view;
//     delete window_side_camera_view;
//     delete main_panel;
//     delete tool_bar;
//     delete tool_bar2;
	
	delete tl_default_color;
	delete tl_selection_color;
	delete tl_special_color;
	delete tl_offset_color;
	delete tl_drag_offset_color;
	delete tl_border_color;
	delete tl_use_border_color;
	delete tl_motion_color;
	delete tl_drawing_color;
	delete cursor_line ;
	delete cursor_selection ;
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
}

//------------------- PUBLIC MEMBERS ----------------

Timeline *KToon::timeline()
{
	//FIXME:usada en exposuresheet, scenes, properties, drawingarea
	Q_CHECK_PTR( timeline_dialog );
	return timeline_dialog;
}

ExposureSheet *KToon::exposureSheet()
{
	//FIXME: usada en scenes, drawngarea, tlframesequence
	Q_CHECK_PTR( exposure_sheet_dialog );
	return exposure_sheet_dialog;
}


GLRenderCameraPreview *KToon::renderCameraPreview()
{
	//FIXME: usada en timeline y en export para exportar la animacion
	//Q_CHECK_PTR( render_camera_preview );
	//return render_camera_preview;
	return m_cameraPreview->cameraPreview();
}

void KToon::loadImage( const QString &file_name, bool from_load )
{
	if ( !from_load )
	{
		KTStatus->currentDrawingArea() -> loadImage( file_name );
		library_dialog -> loadImageSymbol( file_name );
	}
	statusBar() -> message( tr( "Image loaded successfully - " ) + file_name, 2000 );
}

void KToon::loadImageSequence( const QString &file_name, bool from_load )
{
	if ( !from_load )
		KTStatus->currentDrawingArea() -> loadImageSequence( file_name );
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
    KTStatus->currentDocument() -> setLibrary( ap_library.release() );
    
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
    QPtrList<KTColor> colors;
    while ( !n_color.isNull() )
    {
	QDomElement color_tag = n_color.toElement();
	QString n = color_tag.attribute( "Name" );
	QString r = color_tag.attribute( "Red" );
	QString g = color_tag.attribute( "Green" );
	QString b = color_tag.attribute( "Blue" );
	QString a = color_tag.attribute( "Alpha" );
	KTColor *color = new KTColor( r.toFloat(), g.toFloat(), b.toFloat(), a.toFloat() );
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
    KTStatus->currentDocument() -> setPalette( ap_palette.release() );
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
    KTStatus->currentDocument() -> setBrushes( brushes );
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

    //Outline KTColor Tag
    QDomElement outline_color_tag = origin_tag.nextSibling().toElement();
    QString no = outline_color_tag.attribute( "Name" );
    QString ro = outline_color_tag.attribute( "Red" );
    QString go = outline_color_tag.attribute( "Green" );
    QString bo = outline_color_tag.attribute( "Blue" );
    QString ao = outline_color_tag.attribute( "Alpha" );
    
    KTColor outline_color( ro.toFloat(), go.toFloat(), bo.toFloat(), ao.toFloat() );
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

	    GLBrush *brush = new GLBrush( KTStatus->currentDrawingArea(), origin, outline_color, width, end );
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

	    GLPencil *pencil = new GLPencil( KTStatus->currentDrawingArea(), origin, outline_color, width, end );
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

	    GLPen *pen = new GLPen( KTStatus->currentDrawingArea(), origin, outline_color, width, end );
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

	    GLLine *line = new GLLine( KTStatus->currentDrawingArea(), origin, outline_color, width, end );
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

	    //Fill KTColor Tag
    	    QDomElement fill_color_tag = end_tag.nextSibling().toElement();
    	    QString nf = fill_color_tag.attribute( "Name" );
    	    QString rf = fill_color_tag.attribute( "Red" );
    	    QString gf = fill_color_tag.attribute( "Green" );
    	    QString bf = fill_color_tag.attribute( "Blue" );
    	    QString af = fill_color_tag.attribute( "Alpha" );
    	    
	    KTColor fill_color( rf.toFloat(), gf.toFloat(), bf.toFloat(), af.toFloat() );
    	    fill_color.setNameColor( nf );

	    GLRectangle *rectangle = new GLRectangle( KTStatus->currentDrawingArea(), origin, outline_color, width, end, fill_color );
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

            //Fill KTColor Tag
    	    QDomElement fill_color_tag = radius_tag.nextSibling().toElement();
    	    QString nf = fill_color_tag.attribute( "Name" );
    	    QString rf = fill_color_tag.attribute( "Red" );
    	    QString gf = fill_color_tag.attribute( "Green" );
    	    QString bf = fill_color_tag.attribute( "Blue" );
    	    QString af = fill_color_tag.attribute( "Alpha" );
    	    
	    KTColor fill_color( rf.toFloat(), gf.toFloat(), bf.toFloat(), af.toFloat() );
    	    fill_color.setNameColor( nf );

	    GLEllipse *ellipse = new GLEllipse( KTStatus->currentDrawingArea(), origin, outline_color, width, radius, fill_color );
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
/*
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
}*/

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
	qDebug("New document");
	QString document_number;
	document_max_value++;
	document_number.setNum( document_max_value );

	if ( KTStatus->currentDrawingArea() && ! KTStatus->currentDrawingArea()->isHidden() )
	{
		//KTStatus->closeCurrent();
	}
	{
		file_name = "";
		
		KTStatus->currentDocument() -> setNameDocument( tr( "Document" ) + document_number );
		setCaption( tr( "Document" ) + document_number );
		
		KTStatus->setupDrawingArea(main_panel);
		
		connect(KTStatus->currentDrawingArea(), SIGNAL(useTool(int)), this, SLOT(slotSelectTool(int )));
		
		exposure_sheet_dialog = new ExposureSheet( this);//, Qt::WStyle_Tool, window, id_window_exposure_sheet, window_exposure_sheet );
		show();
		scenes_dialog = new Scenes( this);//, Qt::WStyle_Tool, window, id_window_scenes, window_scenes );
		
		timeline_dialog = new Timeline( this, Qt::WStyle_Tool, window, id_window_timeline, window_timeline );
		timeline_dialog->hide();
		
		brushes_dialog = new Brushes( this, Qt::WStyle_Tool/*, window, id_window_brushes, window_brushes*/ );
	
		library_dialog = new Library( this, KTStatus->currentDrawingArea());
	
		m_cameraPreview = new KTCameraPreview(main_panel);
// 		render_camera_preview = new GLRenderCameraPreview( main_panel, this, window, id_window_render_camera_preview, window_render_camera_preview, KTStatus->currentDrawingArea() );
// 		render_camera_preview->hide();
		
		top_camera_view = new GLTopCameraView( main_panel, KTStatus->currentDrawingArea() );
		top_camera_view->hide();
		
		side_camera_view = new GLSideCameraView( main_panel, KTStatus->currentDrawingArea() );
		side_camera_view->hide();

		list_of_es.append( exposure_sheet_dialog );
		list_of_tl.append( timeline_dialog );

		if ( window_animation->isEnabled())//window -> isItemEnabled( id_window_animation ) )
		{
			KTStatus->currentDrawingArea() -> show();
			exposure_sheet_dialog -> show();
			scenes_dialog -> show();
			tools_dialog -> show();
			brushes_dialog -> show();
		}
		else if (window_illustration->isEnabled() )// window -> isItemEnabled( id_window_illustration ) )
		{
			timeline_dialog -> show();
// 			render_camera_preview -> show();
			m_cameraPreview->show();
			top_camera_view -> show();
			side_camera_view -> show();
		}
		
		KTStatus->currentDrawingArea() -> setCursor( QCursor( Qt::ForbiddenCursor ) );

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
		//FIXME:kuadrosx ktexposure
		connect( m_ExposureSheetDialog, SIGNAL( frameSelected() ), KTStatus->currentDrawingArea(), SLOT( slotSelectFrame() ) );
		
		connect(m_ExposureSheetDialog, SIGNAL(activateCursor()), this, SLOT(slotActivateCursor()));

		connect( exposure_sheet_dialog, SIGNAL( frameSelected() ), KTStatus->currentDrawingArea(), SLOT( slotSelectFrame() ) );
		
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

		connect(insert_scene, SIGNAL(activated()), scenes_dialog, SLOT(slotInsertScene()));
		connect(insert_remove_scene, SIGNAL(activated()), scenes_dialog, SLOT(slotRemoveScene()));
		
		connect(file_close, SIGNAL(activated()), KTStatus->currentDrawingArea(), SLOT(close()));
		
		connect( KTStatus->currentDrawingArea(), SIGNAL( closed() ), SLOT( slotCloseDrawingArea() ) );

		connect( color_palette_dialog, SIGNAL( outlineColorChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeOutlineColor() ) );
		connect( color_palette_dialog, SIGNAL( fillColorChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeFillColor() ) );
		connect( brushes_dialog, SIGNAL( minThicknessChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeMinThicknessBrush() ) );
		connect( brushes_dialog, SIGNAL( maxThicknessChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeMaxThicknessBrush() ) );
		connect( brushes_dialog, SIGNAL( smoothnessChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeSmoothnessBrush() ) );

		connect( scenes_dialog, SIGNAL( sceneInserted() ), SLOT( slotInsertSync() ) );
		connect( scenes_dialog, SIGNAL( sceneMovedDown( int ) ), SLOT( slotMoveDownSync( int ) ) );
		connect( scenes_dialog, SIGNAL( sceneMovedUp( int ) ), SLOT( slotMoveUpSync( int ) ) );
		connect( scenes_dialog, SIGNAL( sceneRemoved( int ) ), SLOT( slotRemoveSync( int ) ) );
		connect( scenes_dialog, SIGNAL( sceneSelected( int ) ), SLOT( slotSelectSync( int ) ) );

		connect( KTStatus->currentDrawingArea(), SIGNAL( colorGrabbed( KTColor * ) ), color_palette_dialog, SLOT( slotSetColor( KTColor * ) ) );
		connect( KTStatus->currentDrawingArea(), SIGNAL( updated() ), library_dialog -> getSymbolView(), SLOT( updateGL() ) );
		connect( KTStatus->currentDrawingArea(), SIGNAL( wasDrawn( bool ) ), timeline_dialog -> frameSequenceManager() -> frameLayout(), SLOT( slotFrameHasDrawing( bool ) ) );
		
		scenes_dialog -> selectFirstScene();
		exposure_sheet_dialog -> touchFirstFrame();
		file_save->setEnabled ( true );
// 		file -> setItemEnabled( id_file_save, true );
		file_save_as->setEnabled(true);
		file_close->setEnabled(true);
		file_import->setEnabled(true);
		file_export->setEnabled(true);
		file_properties->setEnabled(true);
		
		menuBar() -> setItemVisible( id_insert, true );
		if ( window_animation->isEnabled())//window -> isItemEnabled( id_window_animation ) )
		{
			menuBar() -> setItemVisible( id_edit, true );
			menuBar() -> setItemVisible( id_view, true );
			menuBar() -> setItemVisible( id_tools, true );
			window_exposure_sheet->setVisible(true);
			//window -> setItemVisible( id_window_exposure_sheet, true );
			window_scenes->setVisible(true);
			//window -> setItemVisible( id_window_scenes, true );
			window_tools->setVisible(true);
// 			window -> setItemVisible( id_window_tools, true );
			window_brushes->setVisible(true);
			//window -> setItemVisible( id_window_brushes, true );
// 			window -> setItemVisible( id_window_library, true );
			window_library->setVisible(true);
			//window -> setItemVisible( id_window_drawing_area, true );
			window_drawing_area->setVisible(true);

// 			window_drawing_area -> show();
// 			window_exposure_sheet -> show();
			
			//window_tools -> show();
			//window_brushes -> show();
// 			window_scenes -> show();
// 			window_library -> show();

			edit_undo->setVisible(true);
			edit_redo->setVisible(true);
			edit_cut->setVisible(true);
			edit_copy->setVisible(true);
			edit_paste->setVisible(true);
			tool_bar2 -> show();
		}
		else
		{
			menuBar() -> setItemVisible( id_control, true );
			//FIXME:
			window -> setItemVisible( id_window_timeline, true );
			
			window_render_camera_preview->setVisible(true);
			window_top_camera_view->setVisible(true);
			window_side_camera_view->setVisible(true);
			
			//FIXME:
			window_timeline -> show();
		}
		file_save->setVisible (true);
		

		color_palette_dialog -> enableCustomPalette( true );
		color_palette_dialog -> clearCustomPalette();

		slotSeeIllustration();

		statusBar() -> message( tr( "Ready." ), 2000 );
	}
}

void KToon::slotChoose()
{
	//QString fn = QFileDialog::getOpenFileName( KTOON_REPOSITORY, "KToon Project ( *.ktn )", this );
	
	KTFileDialog choose(this);
	
	if ( choose.exec() == QDialog::Rejected)
	{
		return;
	}
	
	QString fn = choose.fileName();
	
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
	QString filetmp(in_file_name);

	if ( ! filetmp.startsWith( KTOON_REPOSITORY ))
	{
		filetmp = KTOON_REPOSITORY + "/"+filetmp;
	}
	
	std::cout << "Opening file : " << filetmp << std::endl;
	
	QFile f( filetmp );
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
	
// 	slotCloseDrawingArea();
 
// 	if ( KTStatus->isValid() )
// 	{
// 		std::cout << "valid, closing..." << std::endl;
// 		
// 		KTStatus->closeCurrent();
// 		std::cout << "........." << std::endl;
// 	}
// 	else
// 	{
// 		std::cout << "No valid, crashing..." << std::endl;
// 	}
    
	//----------- Load all the file data into the storing classes created for that purpose ------------

	{

		//1. Document Tag
		QDomElement root = xml_doc.documentElement();
    
		//Drawing Area Initialization
		KTStatus->setupDrawingArea(main_panel);

		KTStatus->currentDocument() -> setNameDocument( in_file_name );
		
		connect(KTStatus->currentDrawingArea(), SIGNAL(useTool(int)), this, SLOT(slotSelectTool(int )));

		//1.1. Palette Tag
		QDomElement palette_path_tag = root.firstChild().toElement();
		QString palette_path = KTOON_REPOSITORY + "/"+palette_path_tag.attribute( "PATH" );
		loadPalette( palette_path, true );

		//1.2. Brushes Tag
		QDomElement brushes_path_tag = palette_path_tag.nextSibling().toElement();
		QString brushes_path = KTOON_REPOSITORY + "/"+brushes_path_tag.attribute( "PATH" );
		loadBrushes( brushes_path, true );

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
		KTStatus->currentDocument()->setAnimation( ap_animation.release() );

    //----------- Create the GUI -----------------

		createGUI();

		file_name = QString( in_file_name );
		statusBar() -> message( tr( "File Opened Successfully - %1" ).arg( in_file_name ), 2000 );
		std::cout << "Loaded" << std::endl;

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
    
    QString filetmp(file_name);
    
    if ( ! filetmp.startsWith( KTOON_REPOSITORY ))
    {
	    filetmp = KTOON_REPOSITORY + "/"+filetmp;
    }

    QFile f( filetmp );
    if ( !f.open( IO_WriteOnly ) )
    {
	statusBar() -> message( tr( "Couldn't Save - %1" ).arg( file_name ), 2000 );
	return;
    }

    setCaption( file_name );
    KTStatus->currentDrawingArea() -> setCaption( file_name );
    KTStatus->currentDrawingArea() -> modifyDocument( false );

    KTStatus->currentDocument()->setNameDocument( file_name );
    KTStatus->currentDocument()->save( &f );

    f.close();
    statusBar() -> message( tr( "File Saved Successfully - %1" ).arg( file_name ), 2000 );
}

void KToon::slotSaveAs()
{
	KTFileDialog save(this);
	
	if ( save.exec() == QDialog::Rejected )
	{
		return;
	}
	
	QString fn = save.fileName();; // = QFileDialog::getSaveFileName( KTOON_REPOSITORY, "KToon Project ( *.ktn )", this );
	
	if ( !fn.isEmpty() )
	{
		file_name = fn;
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
    
    if ( export_dialog->exec() != QDialog::Rejected )
    {
	    qDebug(QString("***************Exporting %1***************").arg(KTStatus->currentDocument()->nameDocument()) );
	    scenes_dialog->exportAnimation(KTStatus->currentDocument()->nameDocument(), "PNG");
    }
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
	KTStatus->currentDrawingArea() -> slotUndo();
	statusBar() -> message( tr( "Last Action Undone" ), 2000 );
	
	//QMessageBox::information( this, tr( "Info" ), tr( "Undo" ) + " - " + tr( "Coming Soon..." ) );
}

void KToon::slotRedo()
{
	KTStatus->currentDrawingArea() -> slotRedo();
	statusBar() -> message( tr( "Last Action Redone" ), 2000 );
	//QMessageBox::information( this, tr( "Info" ), tr( "Redo" ) + " - " + tr( "Coming Soon..." ) );
}

void KToon::slotCut()
{
	KTStatus->currentDrawingArea() -> slotCut();
	statusBar() -> message( tr( "Cut Action" ), 2000 );
}

void KToon::slotCopy()
{
	KTStatus->currentDrawingArea() -> slotCopy();
	statusBar() -> message( tr( "Copy Action" ), 2000 );
}

void KToon::slotPaste()
{
	KTStatus->currentDrawingArea() -> slotPaste();
	statusBar() -> message( tr( "Paste Action" ), 2000 );
}

void KToon::slotPasteInPlace()
{
	KTStatus->currentDrawingArea() -> slotPasteInPlace();
	statusBar() -> message( tr( "Paste In Place Action" ), 2000 );
}

void KToon::slotDelete()
{
    KTStatus->currentDrawingArea() -> slotDelete();
    statusBar() -> message( tr( "Delete Action" ), 2000 );
}

void KToon::slotSelectAll()
{
    KTStatus->currentDrawingArea() -> slotSelectAll();
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
    KTStatus->currentDrawingArea() -> slotZoomIn();
    statusBar() -> message( tr( "Zoom In Action" ), 2000 );
}

void KToon::slotZoomOut()
{
    KTStatus->currentDrawingArea() -> slotZoomOut();
    statusBar() -> message( tr( "Zoom Out Action" ), 2000 );
}

void KToon::slotFit()
{
    KTStatus->currentDrawingArea() -> slotFit();
    statusBar() -> message( tr( "Grid Fit Action" ), 2000 );
}

void KToon::slotSeeGrid12()
{
    KTStatus->currentDrawingArea() -> slotSeeGrid12();
    statusBar() -> message( tr( "12 Field Grid Action" ), 2000 );
}

void KToon::slotSeeGrid16()
{
    KTStatus->currentDrawingArea() -> slotSeeGrid16();
    statusBar() -> message( tr( "16 Field Grid Action" ), 2000 );
}

void KToon::slotSeeSubgrid()
{
    KTStatus->currentDrawingArea() -> slotSeeSubgrid();
    statusBar() -> message( tr( "Subgrid Action" ), 2000 );
}

void KToon::slotNoGrid()
{
    KTStatus->currentDrawingArea() -> slotNoGrid();
    statusBar() -> message( tr( "No Grid Action" ), 2000 );
}

void KToon::slotNoPreviousOnionSkin()
{
    KTStatus->currentDrawingArea() -> slotNoPreviousOnionSkin();
    KTStatus -> setCurrentPreviousOnionSkin( 0 );
//     onion_skin -> setItemChecked( previous_checked, false );
//     onion_skin -> setItemChecked( id_view_previous_none, true );
    //previous_toggled -> setDown( false );
    //view_no_previous -> setOn( true );
//     previous_toggled = view_no_previous;
//     previous_checked = id_view_previous_none;
    KTStatus->currentDrawingArea() -> updateGL();
    statusBar() -> message( tr( "No Previous Onion Skin Action" ), 2000 );
}

void KToon::slotPreviousOnionSkin()
{
    KTStatus->currentDrawingArea() -> slotPreviousOnionSkin();
    KTStatus -> setCurrentPreviousOnionSkin( 1 );
//     onion_skin -> setItemChecked( previous_checked, false );
//     onion_skin -> setItemChecked( id_view_previous, true );
//     previous_toggled -> setDown( false );
//     view_previous -> setDown( true );
//     previous_toggled = view_previous;
//     previous_checked = id_view_previous;
    KTStatus->currentDrawingArea() -> updateGL();
    statusBar() -> message( tr( "Previous Onion Skin Action" ), 2000 );
}

void KToon::slotPrevious2OnionSkin()
{
    KTStatus->currentDrawingArea() -> slotPrevious2OnionSkin();
    KTStatus -> setCurrentPreviousOnionSkin( 2 );
//     onion_skin -> setItemChecked( previous_checked, false );
//     onion_skin -> setItemChecked( id_view_previous2, true );
//     previous_toggled -> setDown( false );
//     view_previous2 -> setDown( true );
//     previous_toggled = view_previous2;
//     previous_checked = id_view_previous2;
    KTStatus->currentDrawingArea() -> updateGL();
    statusBar() -> message( tr( "Previous 2 Onion Skin Action" ), 2000 );
}

void KToon::slotPrevious3OnionSkin()
{
    KTStatus->currentDrawingArea() -> slotPrevious3OnionSkin();
    KTStatus -> setCurrentPreviousOnionSkin( 3 );
//     onion_skin -> setItemChecked( previous_checked, false );
//     onion_skin -> setItemChecked( id_view_previous3, true );
//     previous_toggled -> setDown( false );
//     view_previous3 -> setDown( true );
//     previous_toggled = view_previous3;
//     previous_checked = id_view_previous3;
    KTStatus->currentDrawingArea() -> updateGL();
    statusBar() -> message( tr( "Previous 3 Onion Skin Action" ), 2000 );
}

void KToon::slotNoNextOnionSkin()
{
    KTStatus->currentDrawingArea() -> slotNoNextOnionSkin();
    KTStatus -> setCurrentNextOnionSkin( 0 );
//     onion_skin -> setItemChecked( next_checked, false );
//     onion_skin -> setItemChecked( id_view_next_none, true );
//     next_toggled -> setDown( false );
//     view_no_next -> setDown( true );
//     next_toggled = view_no_next;
//     next_checked = id_view_next_none;
    KTStatus->currentDrawingArea() -> updateGL();
    statusBar() -> message( tr( "No Next Onion Skin Action" ), 2000 );
}

void KToon::slotNextOnionSkin()
{
    KTStatus->currentDrawingArea() -> slotNextOnionSkin();
    KTStatus -> setCurrentNextOnionSkin( 1 );
//     onion_skin -> setItemChecked( next_checked, false );
//     onion_skin -> setItemChecked( id_view_next, true );
//     next_toggled -> setDown( false );
//     view_next -> setDown( true );
//     next_toggled = view_next;
//     next_checked = id_view_next;
    KTStatus->currentDrawingArea() -> updateGL();
    statusBar() -> message( tr( "Next Onion Skin Action" ), 2000 );
}

void KToon::slotNext2OnionSkin()
{
    KTStatus->currentDrawingArea() -> slotNext2OnionSkin();
    KTStatus -> setCurrentNextOnionSkin( 2 );
//     onion_skin -> setItemChecked( next_checked, false );
//     onion_skin -> setItemChecked( id_view_next2, true );
//     next_toggled -> setDown( false );
//     view_next2 -> setDown( true );
//     next_toggled = view_next2;
//     next_checked = id_view_next2;
    KTStatus->currentDrawingArea() -> updateGL();
    statusBar() -> message( tr( "Next 2 Onion Skin Action" ), 2000 );
}

void KToon::slotNext3OnionSkin()
{
    KTStatus->currentDrawingArea() -> slotNext3OnionSkin();
    KTStatus -> setCurrentNextOnionSkin( 3 );
//     onion_skin -> setItemChecked( next_checked, false );
//     onion_skin -> setItemChecked( id_view_next3, true );
//     next_toggled -> setDown( false );
//     view_next3 -> setDown( true );
//     next_toggled = view_next3;
//     next_checked = id_view_next3;
    KTStatus->currentDrawingArea() -> updateGL();
    statusBar() -> message( tr( "Next 3 Onion Skin Action" ), 2000 );
}

void KToon::slotRotateClockwise()
{
    KTStatus->currentDrawingArea() -> slotRotateClockwise();
    statusBar() -> message( tr( "Scene Rotated Clockwise" ), 2000 );
}

void KToon::slotRotateCounterClockwise()
{
    KTStatus->currentDrawingArea() -> slotRotateCounterClockwise();
    statusBar() -> message( tr( "Scene Rotated Counter Clockwise" ), 2000 );
}

void KToon::slotRestoreRotation()
{
    KTStatus->currentDrawingArea() -> slotRestoreRotation();
    statusBar() -> message( tr( "Rotation Restored" ), 2000 );
}

void KToon::slotFrontBackGrid()
{
    KTStatus->currentDrawingArea() -> slotFrontBackGrid();
    statusBar() -> message( tr( "Grid to Front/Back Action" ), 2000 );
}

void KToon::slotSeeNTSC()
{
    KTStatus->currentDrawingArea() -> slotSeeNTSC();
    statusBar() -> message( tr( "NTSC Action" ), 2000 );
}

void KToon::slotLightTable()
{
    KTStatus->currentDrawingArea() -> slotLightTable();
    statusBar() -> message( tr( "Light Table Action" ), 2000 );
}

//------------------- SLOTS AND FUNCTIONS INSERT MENU ----------------------------

void KToon::slotInsertLayer()
{
    exposure_sheet_dialog -> slotInsertLayer();
}

void KToon::slotInsertFrame()
{
    if ( window_illustration->isEnabled())//window -> isItemEnabled( id_window_illustration ) )
        timeline_dialog -> frameSequenceManager() -> frameLayout() -> currentFrameSequence() -> slotInsertFrame();
}

void KToon::slotRemoveFrame()
{
    if ( window_illustration->isEnabled())//window -> isItemEnabled( id_window_illustration ) )
        timeline_dialog -> frameSequenceManager() -> frameLayout() -> currentFrameSequence() -> slotRemoveFrame();
}

void KToon::slotInsertKeyFrame()
{
	if ( window_animation->isEnabled()) // window -> isItemEnabled( id_window_animation ) )
        exposure_sheet_dialog -> slotInsertFrame();
}

//------------------- SLOTS AND FUNCTIONS TOOLS MENU ----------------------------

void KToon::slotGroup()
{
    KTStatus->currentDrawingArea() -> slotGroup();
    statusBar() -> message( tr( "Group Action" ), 2000 );
    QMessageBox::information( this, tr( "Info" ), tr( "Group" ) + " - " + tr( "Coming Soon..." ) );
}

void KToon::slotUngroup()
{
    KTStatus->currentDrawingArea() -> slotUngroup();
    statusBar() -> message( tr( "Ungroup Action" ), 2000 );
    QMessageBox::information( this, tr( "Info" ), tr( "Ungroup" ) + " - " + tr( "Coming Soon..." ) );
}

void KToon::slotBringToFront()
{
    KTStatus->currentDrawingArea() -> slotBringToFront();
    statusBar() -> message( tr( "Bring To Front Action" ), 2000 );
}

void KToon::slotSendToBack()
{
    KTStatus->currentDrawingArea() -> slotSendToBack();
    statusBar() -> message( tr( "Send To Back Action" ), 2000 );
}

void KToon::slotOneStepForward()
{
    KTStatus->currentDrawingArea() -> slotOneStepForward();
    statusBar() -> message( tr( "One Step Forward Action" ), 2000 );
}

void KToon::slotOneStepBackward()
{
    KTStatus->currentDrawingArea() -> slotOneStepBackward();
    statusBar() -> message( tr( "One Step Backward Action" ), 2000 );
}

void KToon::slotAlignRight()
{
    KTStatus->currentDrawingArea() -> slotAlignRight();
    statusBar() -> message( tr( "Align Right Action" ), 2000 );
}

void KToon::slotCenterVertically()
{
    KTStatus->currentDrawingArea() -> slotCenterVertically();
    statusBar() -> message( tr( "Center Vertically Action" ), 2000 );
}

void KToon::slotAlignLeft()
{
    KTStatus->currentDrawingArea() -> slotAlignLeft();
    statusBar() -> message( tr( "Align Left Action" ), 2000 );
}

void KToon::slotAlignTop()
{
    KTStatus->currentDrawingArea() -> slotAlignTop();
    statusBar() -> message( tr( "Align Top Action" ), 2000 );
}

void KToon::slotCenterHorizontally()
{
    KTStatus->currentDrawingArea() -> slotCenterHorizontally();
    statusBar() -> message( tr( "Center Horizontally Action" ), 2000 );
}

void KToon::slotAlignBottom()
{
    KTStatus->currentDrawingArea() -> slotAlignBottom();
    statusBar() -> message( tr( "Align Bottom Action" ), 2000 );
}

void KToon::slotFlipHorizontally()
{
    KTStatus->currentDrawingArea() -> slotFlipHorizontally();
    statusBar() -> message( tr( "Flip Horizontally Action" ), 2000 );
}

void KToon::slotFlipVertically()
{
    KTStatus->currentDrawingArea() -> slotFlipVertically();
    statusBar() -> message( tr( "Flip Vertically Action" ), 2000 );
}

void KToon::slotRotateCW90()
{
    KTStatus->currentDrawingArea() -> slotRotateCW90();
    statusBar() -> message( tr( "Rotate Clock Wise 90 Degrees Action" ), 2000 );
}

void KToon::slotRotateCCW90()
{
    KTStatus->currentDrawingArea() -> slotRotateCCW90();
    statusBar() -> message( tr( "Rotate Counter Clock Wise 90 Degrees Action" ), 2000 );
}

void KToon::slotRotate180()
{
    KTStatus->currentDrawingArea()->slotRotate180();
    statusBar() -> message( tr( "Rotate 180 Degrees Action" ), 2000 );
}

void KToon::slotPerspectiveSelection()
{
    KTStatus->currentDrawingArea() -> slotPerspectiveSelection();
    statusBar() -> message( tr( "Perspective Selection Action" ), 2000 );
    QMessageBox::information( this, tr( "Info" ), tr( "Perspective" ) + " - " + tr( "Coming Soon..." ) );
}

void KToon::slotNormalSelection()
{
    tools_dialog -> changeButtonImage( Tools::NORMAL_SELECTION );
    KTStatus -> setCurrentCursor( Tools::NORMAL_SELECTION );
    slotActivateCursor();
}

void KToon::slotContourSelection()
{
    tools_dialog -> changeButtonImage( Tools::CONTOUR_SELECTION );
    KTStatus -> setCurrentCursor( Tools::CONTOUR_SELECTION );
    slotActivateCursor();
}

void KToon::slotSelectTool(int toolId)
{
	switch (toolId )
	{
		case Tools::NORMAL_SELECTION: 
			slotNormalSelection();
		break;
		case Tools::CONTOUR_SELECTION: 
			slotContourSelection();
		break;
		case Tools::BRUSH: 
			slotBrush();
		break;
		case Tools::PENCIL: 
			slotPencil();
		break;
		case Tools::PEN: 
			slotPen();
		break;
		case Tools::LINE: 
			slotLine();
		break;
		case Tools::RECTANGLE: 
			slotRectangle();
		break;
		case Tools::ELLIPSE: 
			slotEllipse();
		break;
		case Tools::CONTOUR_FILL: 
			slotContourFill();
		break;
		case Tools::FILL: 
			slotFill();
		break;
		case Tools::REMOVE_FILL: 
			slotRemoveFill();
		break;
		case Tools::DROPPER: 
			slotDropper();
		break;
		case Tools::ERASER: 
			slotEraser();
		break;
		case Tools::SLICER: 
			slotSlicer();
		break;
		case Tools::MAGNIFYING_GLASS: 
			slotMagnifyingGlass();
		break;
		case Tools::HAND: 
			slotHand();
		break;
		default: break;
	}
}


void KToon::slotBrush()
{
    tools_dialog -> changeButtonImage( Tools::BRUSH );
    KTStatus -> setCurrentCursor( Tools::BRUSH );
    color_palette_dialog->slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotPencil()
{
    tools_dialog -> changeButtonImage( Tools::PENCIL );
    KTStatus -> setCurrentCursor( Tools::PENCIL );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotPen()
{
    tools_dialog -> changeButtonImage( Tools::PEN );
    KTStatus -> setCurrentCursor( Tools::PEN );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotLine()
{
    tools_dialog -> changeButtonImage( Tools::LINE );
    KTStatus -> setCurrentCursor( Tools::LINE );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotRectangle()
{
    tools_dialog -> changeButtonImage( Tools::RECTANGLE );
    KTStatus -> setCurrentCursor( Tools::RECTANGLE );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotEllipse()
{
    tools_dialog -> changeButtonImage( Tools::ELLIPSE );
    KTStatus -> setCurrentCursor( Tools::ELLIPSE );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotContourFill()
{
    tools_dialog -> changeButtonImage( Tools::CONTOUR_FILL );
    KTStatus -> setCurrentCursor( Tools::CONTOUR_FILL );
    color_palette_dialog -> slotActivateOutlineColor();
    slotActivateCursor();
}

void KToon::slotFill()
{
    tools_dialog -> changeButtonImage( Tools::FILL );
    KTStatus -> setCurrentCursor( Tools::FILL );
    color_palette_dialog -> slotActivateFillColor();
    slotActivateCursor();
}

void KToon::slotRemoveFill()
{
    tools_dialog -> changeButtonImage( Tools::REMOVE_FILL );
    KTStatus -> setCurrentCursor( Tools::REMOVE_FILL );
    slotActivateCursor();
}

void KToon::slotDropper()
{
    tools_dialog -> changeButtonImage( Tools::DROPPER );
    KTStatus -> setCurrentCursor( Tools::DROPPER );
    slotActivateCursor();
}

void KToon::slotEraser()
{
    tools_dialog -> changeButtonImage( Tools::ERASER );
    KTStatus -> setCurrentCursor( Tools::ERASER );
    slotActivateCursor();
}

void KToon::slotSlicer()
{
    tools_dialog -> changeButtonImage( Tools::SLICER );
    KTStatus -> setCurrentCursor( Tools::SLICER );
    slotActivateCursor();
}

void KToon::slotMagnifyingGlass()
{
    tools_dialog -> changeButtonImage( Tools::MAGNIFYING_GLASS );
    KTStatus -> setCurrentCursor( Tools::MAGNIFYING_GLASS );
    slotActivateCursor();
}

void KToon::slotHand()
{
    tools_dialog -> changeButtonImage( Tools::HAND );
    KTStatus -> setCurrentCursor( Tools::HAND );
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
	qDebug("-> Illustration Mode");
    //-------------- Show all illustration dialog boxes and update the window menu items -----------

    if ( KTStatus->currentDrawingArea() != NULL )
    {
        KTStatus->currentDrawingArea() -> show();
        tools_dialog -> show();
//         window -> setItemChecked( id_window_tools, true );
//         window -> setItemVisible( id_window_tools, true );
// 	window_tools -> show();
	window_tools->setVisible (true);
// 	window_tools->setDown( true );
//         window -> setItemChecked( id_window_drawing_area, true );
//         window -> setItemVisible( id_window_drawing_area, true );
	
	window_drawing_area->setVisible(true);
	window_drawing_area->setOn(true);
    }

    if ( brushes_dialog != NULL )
    {
        brushes_dialog -> show();
        //window->setItemChecked( id_window_brushes, true );
	//window -> setItemVisible( id_window_brushes, true );
	window_brushes->setVisible ( true );
//	window_brushes -> setDown( true );
    }

    if ( scenes_dialog != NULL )
    {
        scenes_dialog -> show();
//         window -> setItemChecked( id_window_scenes, true );
//         window -> setItemVisible( id_window_scenes, true );
	window_scenes->setVisible(true);
// 	window_scenes -> setDown( true );
    }

    if ( exposure_sheet_dialog != NULL )
    {
    	exposure_sheet_dialog -> show();
//         window -> setItemChecked( id_window_exposure_sheet, true );
//         window -> setItemVisible( id_window_exposure_sheet, true );
	window_exposure_sheet->setVisible(true);
// 	window_exposure_sheet -> show();
// 	window_exposure_sheet -> setDown( true );
    }

    if ( library_dialog != NULL )
    {
        library_dialog -> hide();
//         window -> setItemChecked( id_window_library, false );
//         window -> setItemVisible( id_window_library, true );
	window_library->setVisible(true);
	window_library->setOn(false);
    }

    color_palette_dialog -> show();
//     window -> setItemChecked( id_window_color_palette, true );
//     window -> setItemVisible( id_window_color_palette, true );
    //window_color_palette -> show();
    window_color_palette->setVisible(true);
//     window_color_palette -> setDown( true );

    if ( KTStatus->currentDrawingArea() != NULL )
    {
        menuBar() -> setItemVisible( id_edit, true );
        menuBar() -> setItemVisible( id_view, true );
        menuBar() -> setItemVisible( id_tools, true );

	insert_frame->setVisible(false);
	insert_remove_frame->setVisible(false);
	insert_keyframe->setVisible(true);
	insert_scene->setVisible(true);
	insert_remove_scene->setVisible(true);

	edit_undo->setVisible(true);
	edit_redo->setVisible(true);
	edit_cut->setVisible(true);
	edit_copy->setVisible(true);
	edit_paste->setVisible(true);
	tool_bar2 -> show();
    }
    menuBar() -> setItemVisible( id_control, false );
    file_export->setVisible(false);

    //-------------- Hide all animation dialog boxes and update the window menu items -------------
    
    if ( m_cameraPreview != NULL )
    {
	    m_cameraPreview -> hide();
// 	    window -> setItemChecked( id_window_render_camera_preview, false );
// 	    window -> setItemVisible( id_window_render_camera_preview, false );
	    window_render_camera_preview->setVisible(false);
// 	    window_render_camera_preview -> setDown( false );
    }

    if ( side_camera_view != NULL )
    {
        side_camera_view -> hide();
//         window -> setItemChecked( id_window_side_camera_view, false );
//         window -> setItemVisible( id_window_side_camera_view, false );
	window_side_camera_view->setVisible(false);
// 	window_side_camera_view -> setDown( false );
    }

    if ( top_camera_view != NULL )
    {
        top_camera_view -> hide();
//         window -> setItemChecked( id_window_top_camera_view, false );
//         window -> setItemVisible( id_window_top_camera_view, false );
	window_top_camera_view->setVisible(false);
// 	window_top_camera_view -> setDown( false );
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
    //window -> setItemEnabled( id_window_illustration, false );
    window_illustration->setVisible(false);
    //window -> setItemEnabled( id_window_animation, true );
    window_animation->setEnabled(true);
    window_animation->setVisible(true);
    //window_animation -> show();
}

void KToon::slotSeeAnimation()
{
	qDebug("-> Animation Mode");
    //---------- Hide all illustration dialog boxes and update the window menu items ---------
	
    tools_dialog -> hide();
    window_tools->setVisible(false);
    //window -> setItemChecked( id_window_tools, false );
    //window -> setItemVisible( id_window_tools, false );
//     window_tools -> hide();

    if ( brushes_dialog != 0 )
    {
        brushes_dialog -> hide();
	window_brushes->setVisible ( false);
    }

    if ( scenes_dialog != 0 )
    {
    	scenes_dialog -> hide();
	window_scenes->setVisible ( false);
    }

    if ( exposure_sheet_dialog )
    {
        exposure_sheet_dialog -> hide();
	window_exposure_sheet->setVisible ( false);
//         window -> setItemChecked( id_window_exposure_sheet, false );
//         window -> setItemVisible( id_window_exposure_sheet, false );
// 	window_exposure_sheet -> hide();
// 	window_exposure_sheet -> setDown( false );
    }
    

    if ( library_dialog != 0 )
    {
        library_dialog -> hide();
//         window -> setItemChecked( id_window_library, false );
//         window -> setItemVisible( id_window_library, false );
	window_library->setVisible(false);
	window_library->setOn(false);
    }

     color_palette_dialog -> hide();
    
//     window -> setItemChecked( id_window_color_palette, false );
//     window -> setItemVisible( id_window_color_palette, false );
//     window_color_palette -> hide();
//     window_color_palette -> setDown( false );
    
    window_color_palette->setVisible(false);


    if ( KTStatus->currentDrawingArea() != 0 )
    {
        KTStatus->currentDrawingArea() -> hide();
//         window -> setItemChecked( id_window_drawing_area, false );
//         window -> setItemVisible( id_window_drawing_area, false );
	window_drawing_area->setVisible(false);
	window_drawing_area ->setOn(false);
    }

    insert_frame->setVisible(true);
    insert_remove_frame->setVisible(true);
    insert_keyframe->setVisible(false);
    insert_scene->setVisible(false);
    insert_remove_scene->setVisible(false);
    file_export->setVisible(true);

    menuBar() -> setItemVisible( id_edit, false );
    menuBar() -> setItemVisible( id_view, false );
    menuBar() -> setItemVisible( id_tools, false );

    edit_undo->setVisible(false);
    edit_redo->setVisible(false);
    edit_cut->setVisible(false);
    edit_copy->setVisible(false);
    edit_paste->setVisible(false);
    tool_bar2 -> hide();

    //-------------- Show all animation dialog boxes and update the window menu items -----------

    if ( m_cameraPreview != 0 )
    {
        m_cameraPreview -> show();
//         window -> setItemChecked( id_window_render_camera_preview, true );
//         window -> setItemVisible( id_window_render_camera_preview, true );
	window_render_camera_preview->setVisible(true);
// 	window_render_camera_preview -> setDown( true );
    }

    if ( side_camera_view != 0 )
    {
        side_camera_view -> show();
//         window -> setItemChecked( id_window_side_camera_view, true );
//         window -> setItemVisible( id_window_side_camera_view, true );
	window_side_camera_view->setVisible(true);
// 	window_side_camera_view -> setDown( true );
    }

    if ( top_camera_view != 0 )
    {
        top_camera_view -> show();
//         window -> setItemChecked( id_window_top_camera_view, true );
//         window -> setItemVisible( id_window_top_camera_view, true );
	window_top_camera_view->setVisible(true);
// 	window_top_camera_view -> setDown( true );
    }

    if ( timeline_dialog != 0 )
    {
    	timeline_dialog -> show();
        window -> setItemChecked( id_window_timeline, true );
        window -> setItemVisible( id_window_timeline, true );
	window_timeline -> show();
	window_timeline -> setDown( true );

	menuBar() -> setItemVisible( id_control, true );
    }

    //Enable the illustration menu item and disable the animation menu item
    //window -> setItemEnabled( id_window_illustration, true );
    window_illustration->setEnabled(true);
    window_illustration->setVisible(true);
    window_animation->setEnabled(false);
    window_animation->setVisible(false);
//     window_illustration -> show();
//     window -> setItemEnabled( id_window_animation, false );
//     window_animation -> hide();

    if ( timeline_dialog != 0 )
        timeline_dialog -> frameSequenceManager() -> getRuler() -> slotSetOffset( timeline_dialog -> frameSequenceManager() -> getRuler() -> getOffset() );
}

void KToon::slotWindowTools()
{
    if ( tools_dialog -> isVisible() )
    {
    	tools_dialog -> hide();
	//window -> setItemChecked( id_window_tools, false );
// 	window_tools -> setDown( false );
    }
    else
    {
    	tools_dialog -> show();
// 	window -> setItemChecked( id_window_tools, true );
// 	window_tools -> setDown( true );
    }
}

void KToon::slotWindowBrushes()
{
    if ( brushes_dialog -> isVisible() )
    {
    	brushes_dialog->hide();
	//window->setItemChecked( id_window_brushes, false );
//	window_brushes -> setDown( false );
    }
    else
    {
    	brushes_dialog -> show();
	//window -> setItemChecked( id_window_brushes, true );
	//window_brushes -> setDown( true );
    }
}

void KToon::slotWindowScenes()
{
    if ( scenes_dialog -> isVisible() )
    {
    	scenes_dialog -> hide();
// 	window -> setItemChecked( id_window_scenes, false );
// 	window_scenes -> setDown( false );
    }
    else
    {
    	scenes_dialog -> show();
// 	window -> setItemChecked( id_window_scenes, true );
// 	window_scenes -> setDown( true );
    }
}

void KToon::slotWindowColorPalette()
{
    if ( color_palette_dialog -> isVisible() )
    {
    	color_palette_dialog -> hide();
// 	window -> setItemChecked( id_window_color_palette, false );
// 	window_color_palette -> setDown( false );
    }
    else
    {
    	color_palette_dialog -> show();
// 	window -> setItemChecked( id_window_color_palette, true );
// 	window_color_palette -> setDown( true );
    }
}

void KToon::slotWindowExposureSheet()
{
    if ( exposure_sheet_dialog -> isVisible() )
    {
    	exposure_sheet_dialog -> hide();
// 	window -> setItemChecked( id_window_exposure_sheet, false );
// 	window_exposure_sheet -> setDown( false );
    }
    else
    {
    	exposure_sheet_dialog -> show();
// 	window -> setItemChecked( id_window_exposure_sheet, true );
// 	window_exposure_sheet -> setDown( true );
    }
}

void KToon::slotWindowLibrary()
{
    if ( library_dialog -> isVisible() )
    {
    	library_dialog -> hide();
// 	window -> setItemChecked( id_window_library, false );
// 	window_library -> setDown( false );
    }
    else
    {
    	library_dialog -> show();
// 	window -> setItemChecked( id_window_library, true );
// 	window_library -> setDown( true );
    }
}

void KToon::slotWindowDrawingArea()
{
	if ( KTStatus->currentDrawingArea()->isVisible() )
	{
		KTStatus->currentDrawingArea()->hide();
	//window -> setItemChecked( id_window_drawing_area, false );
		window_drawing_area->setOn(false);
	}
	else
	{
		KTStatus->currentDrawingArea()->show();
// 	window -> setItemChecked( id_window_drawing_area, true );
		window_drawing_area->setOn(true);
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
    if ( m_cameraPreview -> isVisible() )
    {
    	m_cameraPreview -> hide();
// 	window -> setItemChecked( id_window_render_camera_preview, false );
	window_render_camera_preview->setOn( false );
    }
    else
    {
    	m_cameraPreview -> show();
// 	window -> setItemChecked( id_window_render_camera_preview, true );
	window_render_camera_preview->setOn( true );
    }
}

void KToon::slotWindowTopCameraView()
{
    if ( top_camera_view -> isVisible() )
    {
    	top_camera_view -> hide();
// 	window -> setItemChecked( id_window_top_camera_view, false );
	window_top_camera_view->setOn( false );
    }
    else
    {
    	top_camera_view -> show();
// 	window -> setItemChecked( id_window_top_camera_view, true );
	window_top_camera_view->setOn(true );
    }
}

void KToon::slotWindowSideCameraView()
{
    if ( side_camera_view -> isVisible() )
    {
    	side_camera_view -> hide();
// 	window -> setItemChecked( id_window_side_camera_view, false );
	window_side_camera_view->setOn( false );
    }
    else
    {
    	side_camera_view -> show();
// 	window -> setItemChecked( id_window_side_camera_view, true );
	window_side_camera_view->setOn( true );
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
	std::cout << "Closing..." << std::endl;
	if ( KTStatus->isValid() && KTStatus->currentDrawingArea()->isModified() )
	{
		slotSave();
	}
// 	ExposureSheet * loe;
	
// 	for(loe = list_of_es.first(); loe ; loe = list_of_es.next() )
// 	{
// 		if( loe )
// 		{
// 			loe->close();
// 			list_of_es.remove(loe);
// // 			delete loe;
// 		}
// 	}
	
// 	Timeline *tl = 0;
// 	for(tl = list_of_tl.first(); tl ; tl = list_of_tl.next() )
// 	{
// 		if( tl )
// 		{
// 			tl->close();
// 			list_of_tl.remove(tl);
// // 			delete loe;
// 		}
// 	}
	
	list_of_es.setAutoDelete( true );
	list_of_tl.setAutoDelete( true );
	list_of_es.clear();
	list_of_tl.clear();
	list_of_es.setAutoDelete( false );
	list_of_tl.setAutoDelete( false );

	
	delete scenes_dialog;
	delete brushes_dialog;
	delete library_dialog;
// 	delete render_camera_preview;
	delete m_cameraPreview;
	delete top_camera_view;
	delete side_camera_view;


	scenes_dialog = 0;
	tools_dialog -> hide();
	library_dialog = 0;
	brushes_dialog = 0;
	m_cameraPreview = 0;
	top_camera_view = 0;
	side_camera_view = 0;
	exposure_sheet_dialog = 0;
	timeline_dialog = 0;

	//file -> setItemEnabled( id_file_save, false );
	file_save->setEnabled(false);
	file_save_as->setEnabled(false);
	file_close->setEnabled(false);
	file_import->setEnabled(false);
	file_export->setEnabled(false);
	file_properties->setEnabled(false);
	window_exposure_sheet->setVisible(false);
// 	window -> setItemVisible( id_window_exposure_sheet, false );
// 	window -> setItemVisible( id_window_library, false );
// 	window -> setItemChecked( id_window_library, false );
	//window -> setItemVisible( id_window_scenes, false );
	window_library->setVisible(false);
	window_scenes->setVisible(false);
// 	window -> setItemVisible( id_window_drawing_area, false );
	window -> setItemVisible( id_window_timeline, false );
// 	window -> setItemVisible( id_window_tools, false );
	window_tools->setVisible(false);
	//window -> setItemVisible( id_window_brushes, false );
	window_brushes->setVisible(false);
// 	window->setItemVisible( id_window_render_camera_preview, false );
// 	window->setItemVisible( id_window_top_camera_view, false );
// 	window->setItemVisible( id_window_side_camera_view, false );
	
// 	window_exposure_sheet -> hide();
// 	window_library -> hide();
	window_library->setOn(false);
	//window_scenes -> hide();
// 	window_scenes->setVisible(false);
	window_drawing_area->setVisible(false);
	window_timeline -> hide();
// 	window_tools -> hide();
	window_tools->setVisible(false);
	//window_brushes -> hide();
	window_brushes->setVisible(false);
	window_render_camera_preview->setVisible(false);
	window_top_camera_view ->setVisible(false);
	window_side_camera_view->setVisible(false);

	KTStatus->currentDocument()->init();

	KTStatus -> setCurrentScene( 0 );
	KTStatus -> setCurrentLayer( 0 );
	KTStatus -> setCurrentKeyFrame( 0 );
	QPtrList<KeyFrame> empty;
	KTStatus -> setRenderKeyframes( empty );

	menuBar() -> setItemVisible( id_insert, false );
	menuBar() -> setItemVisible( id_control, false );
	if ( window_animation->isEnabled())//window -> isItemEnabled( id_window_animation ) )
	{
		menuBar() -> setItemVisible( id_edit, false );
		menuBar() -> setItemVisible( id_view, false );
		menuBar() -> setItemVisible( id_tools, false );

		edit_undo->setVisible(false);
		edit_redo->setVisible(false);
		edit_cut->setVisible(false);
		edit_copy->setVisible(false);
		edit_paste->setVisible(false);
		tool_bar2 -> hide();
	}
	file_save->setVisible(false);
	
	color_palette_dialog -> enableCustomPalette( false );
	color_palette_dialog -> clearCustomPalette();
	std::cout << "Closed" << std::endl;
}


void KToon::slotActivateCursor()
{
	ILayer *il = exposure_sheet_dialog -> currentLayerObj();
	ESFrame *sf = il -> selectedFrame();
	bool locked = false;
	if ( sf != NULL )
		locked = sf -> isLocked();
	if ( KTStatus -> currentKeyFrame() != NULL && !locked )
	{
		switch ( KTStatus -> currentCursor() )
		{
			case Tools::NORMAL_SELECTION:
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_selection );
				emit(changedCursor(*cursor_selection));
				break;
			}
			case Tools::CONTOUR_SELECTION:
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_nodes );
				emit(changedCursor(*cursor_nodes));
				break;
			}
			case Tools::BRUSH: 
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_brush );
				emit(changedCursor(*cursor_brush));
				break;
			}
			case Tools::PENCIL: 
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_pencil );
				emit(changedCursor(*cursor_pencil));
				break;
			}
			case Tools::PEN:
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_pen );
				emit(changedCursor(*cursor_pen));
				break;
			}
			case Tools::LINE: 
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_line );
				emit(changedCursor(*cursor_line));
				break;
			}
			case Tools::RECTANGLE: 
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_rectangle );
				emit(changedCursor(*cursor_rectangle));
				break;
			}
			case Tools::ELLIPSE: 
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_ellipse );
				emit(changedCursor(*cursor_ellipse));
				break;
			}
			case Tools::CONTOUR_FILL: 
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_contour_fill );
				emit(changedCursor(*cursor_contour_fill));
				break;
			}
			case Tools::FILL: 
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_fill );
				emit(changedCursor(*cursor_fill));
				break;
			}
			case Tools::REMOVE_FILL: 
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_remove_fill );
				emit(changedCursor(*cursor_remove_fill));
				break;
			}
			case Tools::DROPPER: 
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_dropper );
				emit(changedCursor(*cursor_dropper));
				break;
			}
			case Tools::ERASER: 
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_eraser );
				emit(changedCursor(*cursor_eraser));
				break;
			}
			case Tools::SLICER: 
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_slicer );
				emit(changedCursor(*cursor_slicer));
				break;
			}
			case Tools::MAGNIFYING_GLASS:
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_magnifying_glass );
				emit(changedCursor(*cursor_magnifying_glass));
				break;
			}
			case Tools::HAND: 
			{
				KTStatus->currentDrawingArea() -> setCursor( *cursor_hand );
				emit(changedCursor(*cursor_hand));
				break;
			}
			default:
			{
				    break;
			}
		}
	}
	else
		KTStatus->currentDrawingArea() -> setCursor( QCursor( Qt::ForbiddenCursor ) );

}

//**************

void KToon::slotSelectSync( int sp )
{
	//FIXME:
	exposure_sheet_dialog->hide();
	exposure_sheet_dialog = list_of_es.at( sp );
	if ( /*window_exposure_sheet->isOn() &&*/ window_animation->isEnabled())//window -> isItemChecked( id_window_exposure_sheet ) && window_animation->isEnabled())//window -> isItemEnabled( id_window_animation ) )
		exposure_sheet_dialog -> show();

	timeline_dialog = list_of_tl.at( sp );

	slotActivateCursor();
}

void KToon::slotInsertSync()
{
	ExposureSheet *new_exposure_sheet = new ExposureSheet( this);//, Qt::WStyle_Tool, window, id_window_exposure_sheet, window_exposure_sheet );
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
	connect( new_exposure_sheet, SIGNAL( frameSelected() ), KTStatus->currentDrawingArea(), SLOT( slotSelectFrame() ) );
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

	connect( KTStatus->currentDrawingArea(), SIGNAL( wasDrawn( bool ) ), new_timeline -> frameSequenceManager() -> frameLayout(), SLOT( slotFrameHasDrawing( bool ) ) );
}

void KToon::slotRemoveSync( int sp )
{
    ExposureSheet *es_to_remove = list_of_es.at( sp );
    list_of_es.remove( es_to_remove );
    delete es_to_remove;
    if ( sp == ( int )list_of_es.count() )
    {
        exposure_sheet_dialog = list_of_es.getLast();
	if ( /*window_exposure_sheet->isOn() &&*/ window_animation->isEnabled() )//window -> isItemEnabled( id_window_animation ) )
	    exposure_sheet_dialog -> show();
    }
    else
    {
        exposure_sheet_dialog = list_of_es.at( sp );
	if ( /* window_exposure_sheet->isOn() &&*/ window_animation->isEnabled() )//window -> isItemEnabled( id_window_animation ) )
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
	if ( KTStatus->isValid() )
	{
		if ( ! KTStatus->currentDrawingArea()->close() )
		{
			close_event -> ignore();
			return;
		}
		else
		{
			KTCONFIG->configDocument()->addRecentFiles(recent_names);
			KTCONFIG->sync();
		}
	}
	close_event -> accept();
}

void KToon::createGUI()
{
	std::cout << "Creating GUI" << std::endl;
	setCaption( KTStatus->currentDocument()->nameDocument() );
	
	QPtrList<KTColor> custom_colors = KTStatus->currentDocument()->getPalette() -> getColors();
	color_palette_dialog -> enableCustomPalette( true );
	color_palette_dialog -> loadCustomColors( custom_colors );
	
	QPtrList<Brush> brushes = KTStatus->currentDocument()->getBrushes();
	brushes_dialog = new Brushes( this, Qt::WStyle_Tool);
	brushes_dialog -> loadBrushes( brushes );
	
	QPtrList<Scene> scenes = KTStatus->currentDocument()->getAnimation() -> getScenes();
	scenes_dialog = new Scenes( this);//, Qt::WStyle_Tool, window, id_window_scenes, window_scenes );
	scenes_dialog -> loadScenes( scenes );
	
	library_dialog = new Library( this, KTStatus->
			currentDrawingArea());
	library_dialog -> loadItems( KTStatus->currentDocument()->getLibrary() -> getItems() );
	
	m_cameraPreview = new KTCameraPreview(main_panel);
	//     render_camera_preview = new GLRenderCameraPreview( main_panel, this, window, id_window_render_camera_preview, window_render_camera_preview, KTStatus->currentDrawingArea() );
	m_cameraPreview->hide();
	top_camera_view = new GLTopCameraView( main_panel, KTStatus->currentDrawingArea() );
	side_camera_view = new GLSideCameraView( main_panel, KTStatus->currentDrawingArea() );
	
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
	if ( window_animation->isEnabled() )//window -> isItemEnabled( id_window_animation ) )
	{
		window_exposure_sheet->setVisible(true);
	//         window -> setItemVisible( id_window_exposure_sheet, true );
	// 	window -> setItemChecked( id_window_exposure_sheet, true );
	// 	window_exposure_sheet -> show();
	// 	window_exposure_sheet -> setDown( true );
		exposure_sheet_dialog -> show();
	//         window -> setItemVisible( id_window_drawing_area, true );
	// 	window -> setItemChecked( id_window_drawing_area, true );
		window_drawing_area->setVisible(true);
		window_drawing_area->setOn(true);
		KTStatus->currentDrawingArea() -> show();
	//         window -> setItemVisible( id_window_scenes, true );
	// 	window -> setItemChecked( id_window_scenes, true );
		window_scenes->setVisible(true);
	// 	window_scenes -> show();
	// 	window_scenes -> setDown( true );
		scenes_dialog -> show();
	//         window -> setItemVisible( id_window_tools, true );
	// 	window -> setItemChecked( id_window_tools, true );
		window_tools->setVisible(true);
		//window_tools -> show();
		//window_tools -> setDown( true );
		tools_dialog -> show();
	//         window -> setItemVisible( id_window_library, true );
	// 	window -> setItemChecked( id_window_library, false );
		window_library->setVisible(true);
		window_library->setOn( false );
		//window -> setItemVisible( id_window_brushes, true );
		//window -> setItemChecked( id_window_brushes, true );
		//window_brushes -> show();
		window_brushes->setVisible(true);
	// 	window_brushes -> setDown( true );
		brushes_dialog -> show();
	}
	else if ( window_illustration->isEnabled() )//window -> isItemEnabled( id_window_illustration ) )
	{
		window -> setItemVisible( id_window_timeline, true );
		window -> setItemChecked( id_window_timeline, true );
		window_timeline -> show();
		window_timeline -> setDown( true );
		timeline_dialog -> show();
	//         window -> setItemVisible( id_window_render_camera_preview, true );
	// 	window -> setItemChecked( id_window_render_camera_preview, true );
		window_render_camera_preview->setVisible(true);
	// 	window_render_camera_preview -> setDown( true );
		m_cameraPreview->show();
	//         window -> setItemVisible( id_window_side_camera_view, true );
	// 	window -> setItemChecked( id_window_side_camera_view, true );
		window_side_camera_view->setVisible(true);
	// 	window_side_camera_view -> setDown( true );
		side_camera_view-> show();
	//         window -> setItemVisible( id_window_top_camera_view, true );
	// 	window -> setItemChecked( id_window_top_camera_view, true );
		window_top_camera_view->setVisible(true);
	// 	window_top_camera_view -> setDown( true );
		top_camera_view -> show();
	}
	
	KTStatus->currentDrawingArea() -> setCursor( QCursor( Qt::ForbiddenCursor ) );
	
	connect( KTStatus->currentDrawingArea(), SIGNAL( closed() ), SLOT( slotCloseDrawingArea() ) );
	connect( color_palette_dialog, SIGNAL( outlineColorChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeOutlineColor() ) );
	connect( color_palette_dialog, SIGNAL( fillColorChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeFillColor() ) );
	connect( brushes_dialog, SIGNAL( minThicknessChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeMinThicknessBrush() ) );
	connect( brushes_dialog, SIGNAL( maxThicknessChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeMaxThicknessBrush() ) );
	connect( brushes_dialog, SIGNAL( smoothnessChanged() ), KTStatus->currentDrawingArea(), SLOT( slotChangeSmoothnessBrush() ) );
	connect( KTStatus->currentDrawingArea(), SIGNAL( colorGrabbed( KTColor * ) ), color_palette_dialog, SLOT( slotSetColor( KTColor * ) ) );
	connect(insert_scene, SIGNAL(activated()), scenes_dialog, SLOT(slotInsertScene()));
	connect(insert_remove_scene, SIGNAL(activated()), scenes_dialog, SLOT(slotRemoveScene()));
	
	connect(file_close, SIGNAL(activated()), KTStatus->currentDrawingArea(), SLOT(close()));
	connect( scenes_dialog, SIGNAL( sceneInserted() ), SLOT( slotInsertSync() ) );
	connect( scenes_dialog, SIGNAL( sceneMovedDown( int ) ), SLOT( slotMoveDownSync( int ) ) );
	connect( scenes_dialog, SIGNAL( sceneMovedUp( int ) ), SLOT( slotMoveUpSync( int ) ) );
	connect( scenes_dialog, SIGNAL( sceneRemoved( int ) ), SLOT( slotRemoveSync( int ) ) );
	connect( scenes_dialog, SIGNAL( sceneSelected( int ) ), SLOT( slotSelectSync( int ) ) );
	
	KTStatus -> setCurrentScene( ( KTStatus->currentDocument()->getAnimation() -> getScenes() ).first() );
	KTStatus -> setCurrentLayer( ( KTStatus -> currentScene() -> getLayers() ).first() );
	KTStatus -> setCurrentKeyFrame( ( ( KTStatus -> currentScene() -> getLayers() ).first() -> keyFrames() ).first() );
	
	exposure_sheet_dialog -> touchFirstFrame();
	scenes_dialog -> selectFirstScene();
	
	timeline_dialog -> frameSequenceManager() -> frameLayout() -> slotUpdateMaxUsedFrames();
	exposure_sheet_dialog -> updateIndicators( exposure_sheet_dialog -> currentLayerObj() );
	
	//file -> setItemEnabled( id_file_save, true );
	file_save->setEnabled(true);
	file_save_as->setEnabled(true);
	file_close->setEnabled(true);
	file_import->setEnabled(true);
	file_export->setEnabled(true);
	file_properties->setEnabled(true);
	
	
	menuBar() -> setItemVisible( id_insert, true );
	if (window_animation->isEnabled() )// window -> isItemEnabled( id_window_animation ) )
	{
		menuBar() -> setItemVisible( id_edit, true );
		menuBar() -> setItemVisible( id_view, true );
		menuBar() -> setItemVisible( id_tools, true );
	
		edit_undo->setVisible(true);
		edit_redo->setVisible(true);
		edit_cut->setVisible(true);
		edit_copy->setVisible(true);
		edit_paste->setVisible(true);
		tool_bar2 -> show();
	}
	else
		menuBar() -> setItemVisible( id_control, true );
	file_save->setVisible(true);
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
