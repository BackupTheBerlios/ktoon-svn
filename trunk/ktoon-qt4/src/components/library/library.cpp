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

#include "library.h"
#include "images.h"
// #include "ktoon.h"
#include "librarydata.h"
#include "symbol.h"
#include "folder.h"

#include <qtooltip.h>
#include <qmatrix.h>
#include <qmessagebox.h>
#include <q3hbox.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3PtrList>
#include <QKeyEvent>
#include <QLabel>

#include <memory>

//------------- CONSTRUCTOR ----------------

Library::Library( QWidget *parent, QGLWidget *share)//, WFlags style)
	: KTModuleWidgetBase( parent, "Scenes"/*, false, style */) , number_of_items(0)
{
	Q_CHECK_PTR( parent );
// 	Q_CHECK_PTR( in_assigned_menu );
 	Q_CHECK_PTR( share );
// 	Q_CHECK_PTR( assig_tb_button );

    //Initializations
	setCaption( tr( "Graphic Library" ) );
//     setFont( QFont( "helvetica", 8 ) );
//     setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
// 	parent_widget = parent;
// 	assigned_menu = in_assigned_menu;
// 	assigned_item = id_assigned_item;
// 	assigned_tb_button = assig_tb_button;
// 	number_of_items = 0;
	image_count = 0;
// 	resize( 150, 350 );
// 	setMinimumSize( 150, 350 );
// 	setMaximumSize( 150, 350 );
// 	k_toon = ( KToon * )parent;

    //Icon initializations
	i_add_symbol = QPixmap( plussign_xpm );
	i_delete_symbol = QPixmap( minussign_xpm );
	i_insert_into_drawing_area = QPixmap( insert_cg_xpm );
	i_add_folder = QPixmap( addfolder_xpm );

    //----------- Component Initializations -----------------

	text_number_of_items = new QLabel( QString( "# " ) + tr( "items" ) + QString( ": " ) + QString::number( number_of_items ), this );
	addChild(text_number_of_items);
// 	text_number_of_items -> resize( 100, 20 );
// 	text_number_of_items -> move( 5, 5 );

	symbol_view = new SymbolView( this, share );
	addChild(symbol_view);
// 	symbol_view -> resize( 140, 100 );
// 	symbol_view -> move( text_number_of_items -> x(), text_number_of_items -> y() + text_number_of_items -> height() + 5 );

	table_symbols = new SymbolTable( this );
	addChild(table_symbols);
// 	table_symbols -> resize( 140, 185 );
// 	table_symbols -> move( symbol_view -> x(), symbol_view -> y() + symbol_view -> height() + 5 );
	table_symbols -> setSelectionMode( Q3ListView::Single );
//     table_symbols -> setFont( QFont( "helvetica", 8 ) );
	table_symbols -> addColumn( tr( "Name" ), 135 );
	table_symbols -> setResizeMode( Q3ListView::NoColumn );
	table_symbols -> setSorting( 10 ); //Not automatic sorting (10 > 4)
	connect( table_symbols, SIGNAL( selectionChanged() ), SLOT( slotSelectSymbol() ) );
	connect( table_symbols, SIGNAL( doubleClicked( Q3ListViewItem *, const QPoint &, int ) ), SLOT( slotStartRename( Q3ListViewItem *, const QPoint &, int ) ) );
	connect( table_symbols, SIGNAL( itemRenamed( Q3ListViewItem *, int, const QString & ) ), SLOT( slotUpdateLibraryData() ) );
	connect( table_symbols, SIGNAL( released( SymbolItem * ) ), SLOT( slotUpdateLibraryData() ) );

	Q3HBox *containerButtons = new Q3HBox(this);
	addChild(containerButtons);
	
	add_symbol = new QPushButton( i_add_symbol, "", containerButtons );
	add_symbol -> setAutoDefault( false );
	add_symbol -> setFlat( true );
	connect( add_symbol, SIGNAL( clicked() ), SLOT( slotAddSymbol() ) );
	QToolTip::add( add_symbol, tr( "Add the current graphic to Library" ) );

	delete_symbol = new QPushButton( i_delete_symbol, "", containerButtons );
	delete_symbol -> setAutoDefault( false );
	delete_symbol -> setFlat( true );
	connect( delete_symbol, SIGNAL( clicked() ), SLOT( slotDeleteSymbol() ) );
	QToolTip::add( delete_symbol, tr( "Remove the selected Symbol from Library" ) );

	insert_into_drawing_area = new QPushButton( i_insert_into_drawing_area, "", containerButtons );
	insert_into_drawing_area -> setAutoDefault( false );
	insert_into_drawing_area -> setFlat( true );
	connect( insert_into_drawing_area, SIGNAL( clicked() ), SLOT( slotInsertIntoDrawingArea() ) );
	QToolTip::add( insert_into_drawing_area, tr( "Inserts the selected symbol into the drawing area" ) );

	add_folder = new QPushButton( i_add_folder, "", containerButtons );
	add_folder -> setAutoDefault( false );
	add_folder -> setFlat( true );
	connect( add_folder, SIGNAL( clicked() ), SLOT( slotAddFolder() ) );
	QToolTip::add( add_folder, tr( "Adds a folder to the symbol list" ) );
	adjustSize();
}

//------------- DESTRUCTOR ----------------

Library::~Library()
{
	delete text_number_of_items;
	delete table_symbols;
	delete add_symbol;
	delete delete_symbol;
	delete insert_into_drawing_area;
	delete add_folder;
	delete symbol_view;
}

//------------- PUBLIC MEMBERS --------------

void Library::addSymbol( GLGraphicComponent *graphic, const QString &name )
{
	Q_CHECK_PTR( graphic );
	number_of_items++;
	text_number_of_items -> setText( QString( "# " ) + tr( "items" ) + QString( ": " ) + QString::number( number_of_items ) );

	SymbolItem *new_graphic = new SymbolItem( table_symbols, name );
	new_graphic -> setDragEnabled( true );
	new_graphic -> setKind( SymbolItem::GRAPHIC );

	LibraryData *lib = KTStatus->currentDocument() -> getLibrary();
	Q3PtrList<Item> its = lib -> getItems();
    
	std::auto_ptr<Symbol> ap_new_symbol(new Symbol);
	Symbol* new_symbol = ap_new_symbol.get();
    
	new_symbol -> setName( name );

	switch ( graphic -> kindGraphic() )
	{
		case GLGraphicComponent::GC_BRUSH:
		{
			GLBrush *b = new GLBrush( *( ( GLBrush * )graphic ) );
			new_graphic -> setGraphic( b );
			new_symbol -> setGraphic( b );
		}
		break;
		case GLGraphicComponent::GC_PENCIL:
		{
			GLPencil *pc = new GLPencil( *( ( GLPencil * )graphic ) );
			new_graphic -> setGraphic( pc );
			new_symbol -> setGraphic( pc );
		}
		break;
		case GLGraphicComponent::GC_LINE:
		{
			GLLine *l = new GLLine( *( ( GLLine * )graphic ) );
			new_graphic -> setGraphic( l );
			new_symbol -> setGraphic( l );
		}
		break;
		case GLGraphicComponent::GC_PEN:
		{
			GLPen *p = new GLPen( *( ( GLPen * )graphic ) );
			new_graphic -> setGraphic( p );
			new_symbol -> setGraphic( p );
		}
		break;
		case GLGraphicComponent::GC_RECTANGLE:
		{
			GLRectangle *r = new GLRectangle( *( ( GLRectangle * )graphic ) );
			new_graphic -> setGraphic( r );
			new_symbol -> setGraphic( r );
		}
		break;
		case GLGraphicComponent::GC_ELLIPSE:
		{
			GLEllipse *e = new GLEllipse( *( ( GLEllipse * )graphic ) );
			new_graphic -> setGraphic( e );
			new_symbol -> setGraphic( e );
		}
		break;
		default: new_graphic -> setGraphic( NULL );
		new_symbol -> setGraphic( NULL );
		break;
	}
	table_symbols -> setSelected( new_graphic, true );
	KTStatus->currentDrawingArea() -> modifyDocument( true );

	its.prepend( new_symbol );
	lib -> setItems( its );
	ap_new_symbol.release();
}

SymbolView *Library::getSymbolView() const
{
	Q_CHECK_PTR( symbol_view );
	return symbol_view;
}

void Library::loadItems( Q3PtrList<Item> items )
{
	number_of_items = 0;

	Item *item_iterator;
	for ( item_iterator = items.last(); item_iterator; item_iterator = items.prev() )
	{
		if ( item_iterator -> itemKind() == Item::ITEM_SYMBOL )
		{
			SymbolItem *symbol_item = new SymbolItem( table_symbols, item_iterator -> getName() );
			symbol_item -> setDragEnabled( true );
			symbol_item -> setKind( SymbolItem::GRAPHIC );
			symbol_item -> setGraphic( ( ( Symbol * )item_iterator ) -> getGraphic() );
			number_of_items++;
		}
		else if ( item_iterator -> itemKind() == Item::ITEM_FOLDER )
		{
			SymbolItem *folder_item = new SymbolItem( table_symbols, item_iterator -> getName() );
			folder_item -> setDragEnabled( true );
			folder_item -> setKind( SymbolItem::FOLDER );
			createSubItems( folder_item, ( Folder * )item_iterator );
		}
	}
}

void Library::loadImageSymbol( const QString &file_name )
{
	number_of_items++;
	image_count++;
	text_number_of_items -> setText( QString( "# " ) + tr( "items" ) + QString( ": " ) + QString::number( number_of_items ) );

	SymbolItem *new_graphic = new SymbolItem( table_symbols, "IMAGE " + QString::number( image_count ) );
	new_graphic -> setDragEnabled( true );
	new_graphic -> setKind( SymbolItem::GRAPHIC );

	LibraryData *lib = KTStatus->currentDocument() -> getLibrary();
	Q3PtrList<Item> its = lib -> getItems();
	Symbol *new_symbol = new Symbol();
    
	try {
		new_symbol -> setName( "IMAGE " + QString::number( image_count ) );

		GLImage *i = new GLImage( symbol_view, file_name, QPoint( 0, 0 ), QPoint() );
		new_graphic -> setGraphic( i );
		new_symbol -> setGraphic( i );

		table_symbols -> setSelected( new_graphic, true );
		KTStatus->currentDrawingArea() -> modifyDocument( true );

		its.prepend( new_symbol );
		lib -> setItems( its );
	}
	catch(...)
	{
		delete new_symbol;
		throw;
	}
}

//-------------- SLOTS ---------------

void Library::slotAddSymbol()
{
	KTStatus->currentDrawingArea() -> modifyDocument( true );
}

void Library::slotDeleteSymbol()
{
	SymbolItem *to_remove = ( SymbolItem * )table_symbols -> selectedItem();
	if ( to_remove != 0 )
	{
		if ( to_remove -> getKind() == SymbolItem::GRAPHIC )
		{
			if ( QMessageBox::warning( this, tr( "Confirm Symbol Elimination" ), tr( "Are you sure you want to delete the selected symbol?" ),
			     QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton ) == QMessageBox::Yes )
			{
				delete to_remove;
				updateNumberOfItems();
				slotUpdateLibraryData();
				text_number_of_items -> setText( QString( "# " ) + tr( "items" ) + QString( ": " ) + QString::number( number_of_items ) );
				table_symbols -> triggerUpdate();
			}
		}
		else if ( to_remove -> folderHasSymbols() )
		{
			if ( QMessageBox::warning( this, tr( "Confirm Symbol Elimination" ),
			     tr( "The folder you are going to remove has some symbols into it.\nDelete it anyway?" ),
			     QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton ) == QMessageBox::Yes )
			{
				delete to_remove;
				updateNumberOfItems();
				slotUpdateLibraryData();
				text_number_of_items -> setText( QString( "# " ) + tr( "items" ) + QString( ": " ) + QString::number( number_of_items ) );
				table_symbols -> triggerUpdate();
			}
		}
		else
		{
			delete to_remove;
			updateNumberOfItems();
			slotUpdateLibraryData();
			text_number_of_items -> setText( QString( "# " ) + tr( "items" ) + QString( ": " ) + QString::number( number_of_items ) );
			table_symbols -> triggerUpdate();
		}
	}
}

void Library::slotSelectSymbol()
{
	SymbolItem *selected_item = ( SymbolItem * )table_symbols -> selectedItem();
	if ( selected_item != 0 && selected_item -> getKind() == SymbolItem::GRAPHIC )
		symbol_view -> display( selected_item -> getGraphic() );
	else
		symbol_view -> display( NULL );
}

void Library::slotInsertIntoDrawingArea()
{
	DrawingArea *drawing_area = ( DrawingArea * )symbol_view -> shareWidget();
	SymbolItem * selected_item = ( SymbolItem * )table_symbols -> selectedItem();
	if ( selected_item != 0 )
	{
		switch ( selected_item -> getGraphic() -> kindGraphic() )
		{
			case GLGraphicComponent::GC_BRUSH:
				drawing_area -> addGraphicComponent( new GLBrush( *( ( GLBrush * )selected_item -> getGraphic() ) ) );
				break;
			case GLGraphicComponent::GC_PENCIL:
				drawing_area -> addGraphicComponent( new GLPencil( *( ( GLPencil * )selected_item -> getGraphic() ) ) );
				break;
			case GLGraphicComponent::GC_LINE:
				drawing_area -> addGraphicComponent( new GLLine( *( ( GLLine * )selected_item -> getGraphic() ) ) );
				break;
			case GLGraphicComponent::GC_PEN:
				drawing_area -> addGraphicComponent( new GLPen( *( ( GLPen * )selected_item -> getGraphic() ) ) );
				break;
			case GLGraphicComponent::GC_RECTANGLE:
				drawing_area -> addGraphicComponent( new GLRectangle( *( ( GLRectangle * )selected_item -> getGraphic() ) ) );
				break;
			case GLGraphicComponent::GC_ELLIPSE:
				drawing_area -> addGraphicComponent( new GLEllipse( *( ( GLEllipse * )selected_item -> getGraphic() ) ) );
				break;
				default: break;
		}
	}
}

void Library::slotAddFolder()
{
	text_number_of_items -> setText( QString( "# " ) + tr( "items" ) + QString( ": " ) + QString::number( number_of_items ) );

	SymbolItem *new_folder = new SymbolItem( table_symbols, tr( "New Folder" ) );
	new_folder -> setKind( SymbolItem::FOLDER );
	new_folder -> setDragEnabled( true );
	table_symbols -> setSelected( new_folder, true );
	KTStatus->currentDrawingArea() -> modifyDocument( true );

	LibraryData *lib = KTStatus->currentDocument() -> getLibrary();
	Q3PtrList<Item> its = lib -> getItems();
	Folder *new_fold = new Folder();
	try {
		new_fold -> setName( tr( "New Folder" ) );
		its.prepend( new_fold );
		lib -> setItems( its );
	}
	catch(...)
	{
		delete new_fold;
		throw;
	}
}

void Library::slotStartRename( Q3ListViewItem *to_rename, const QPoint &point, int col )
{
	Q_CHECK_PTR( to_rename );
	if ( to_rename && table_symbols -> mapFromGlobal( point ).x() > 23 )
	{
		to_rename -> setRenameEnabled( col, true );
		to_rename -> startRename( col );
	}
}

void Library::slotUpdateLibraryData()
{
	LibraryData *lib = KTStatus->currentDocument() -> getLibrary();
	Q3PtrList<Item> its;

	SymbolItem *item = ( SymbolItem * )table_symbols -> firstChild();
	while ( item )
	{
		if ( item -> getKind() == SymbolItem::FOLDER )
		{
			Folder *new_folder = createFolder( item );
			its.append( new_folder );
		}
		else if ( item -> getKind() == SymbolItem::GRAPHIC )
		{
			Symbol *new_symbol = new Symbol();
			try {
				new_symbol -> setGraphic( item -> getGraphic() );
				new_symbol -> setName( item -> text( 0 ) );
				its.append( new_symbol );
			}
			catch(...)
			{
				delete new_symbol;
				throw;
			}
		}
		item = ( SymbolItem * )item -> nextSibling();
	}

	try {
		lib -> setItems( its );
	}
	catch(...)
	{
	  // We are in trouble. There is currently no non-throwing method to get the
	  // items stored in "lib", so we can not reliably delete the objects which
	  // were created in this funtion.
		throw;
	}

	KTStatus->currentDrawingArea() -> modifyDocument( true );
}

//----------- EVENTS AND PROTECTED MEMBERS ---------------


void Library::updateNumberOfItems()
{
	number_of_items = 0;
	Q3ListViewItemIterator it( table_symbols );
	while ( it.current() )
	{
		SymbolItem *item = ( SymbolItem * )it.current();
		if ( item -> getKind() == SymbolItem::GRAPHIC )
			number_of_items++;
		++it;
	}
}

Folder *Library::createFolder( SymbolItem *item )
{
	Q_CHECK_PTR( item );
    
	std::auto_ptr<Folder> ap_folder(new Folder);
	Folder* folder = ap_folder.get();
    
	folder -> setName( item -> text( 0 ) );
	Q3PtrList<Item> its;

	SymbolItem *item_child = ( SymbolItem * )item -> firstChild();
	while ( item_child )
	{
		if ( item_child -> getKind() == SymbolItem::FOLDER )
		{
			Folder *new_folder = createFolder( item_child );
			its.append( new_folder );
		}
		else if ( item_child -> getKind() == SymbolItem::GRAPHIC )
		{
			Symbol *new_symbol = new Symbol();
			try {
				new_symbol -> setGraphic( item_child -> getGraphic() );
				new_symbol -> setName( item_child -> text( 0 ) );
				its.append( new_symbol );
			}
			catch(...)
			{
				delete new_symbol;
				throw;
			}
		}
		item_child = ( SymbolItem * )item_child -> nextSibling();
	}

    // If this operation throws, we do not have a reliable way to delete
    // the items constructed in this function.  
	folder -> setItems( its );

	return ap_folder.release();
}

void Library::createSubItems( SymbolItem *symbol_item, Folder *folder )
{
	Q_CHECK_PTR( symbol_item );
	Q_CHECK_PTR( folder );
	Q3PtrList<Item> sub_items = folder -> getItems();

	Item *item_iterator;
	for ( item_iterator = sub_items.last(); item_iterator; item_iterator = sub_items.prev() )
	{
		if ( item_iterator -> itemKind() == Item::ITEM_SYMBOL )
		{
			SymbolItem *sym_item = new SymbolItem( symbol_item, item_iterator -> getName() );
			sym_item -> setDragEnabled( true );
			sym_item -> setKind( SymbolItem::GRAPHIC );
			sym_item -> setGraphic( ( ( Symbol * )item_iterator ) -> getGraphic() );
			number_of_items++;
		}
		else if ( item_iterator -> itemKind() == Item::ITEM_FOLDER )
		{
			SymbolItem *folder_item = new SymbolItem( symbol_item, item_iterator -> getName() );
			folder_item -> setDragEnabled( true );
			folder_item -> setKind( SymbolItem::FOLDER );
			createSubItems( folder_item, ( Folder * )item_iterator );
		}
	}
}

void Library::keyPressEvent( QKeyEvent *key_event )
{
	Q_CHECK_PTR( key_event );
	switch( key_event -> key() )
	{
		case Qt::Key_Delete: slotDeleteSymbol();
		key_event -> accept();
		break;
		default: key_event -> ignore();
	}
}
