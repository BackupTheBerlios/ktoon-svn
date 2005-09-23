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
 
#define DEBUG_BRUSHES 1

#include <qtooltip.h>
#include <qbrush.h>
#include <qcolor.h> 
#include <qvalidator.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3PtrList>
#include <QLabel>

#include "brushes.h"
#include "images.h"
#include "ktapplication.h"
#include "status.h"

#include <q3dockwindow.h>
#include <memory>


//--------------- CONSTRUCTOR --------------------

Brushes::Brushes( QWidget *parent, Qt::WFlags style/*, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button*/ )
	: KTModuleWidgetBase( parent, "Brushes" )
{
	Q_CHECK_PTR( parent );
// 	Q_CHECK_PTR( in_assigned_menu );
// 	Q_CHECK_PTR( assig_tb_button );

	setCaption( tr( "Brushes" ) );

	move( 800, 88 );
	
// 	assigned_menu = in_assigned_menu;
// 	assigned_item = id_assigned_item;
// 	assigned_tb_button = assig_tb_button;
	
	brush_max_value = 0;
	number_of_brushes = 1;

    //Icon Initializations
	i_add_brush = QPixmap( plussign_xpm );
	i_remove_brush = QPixmap( minussign_xpm );
	
	setupButtons();
	setupTableBruches();
	setupStaticText();
	setupPrevisualizationArea();
	adjustSize();
    
}

//-------------- DESTRUCTOR -----------------

Brushes::~Brushes()
{
    delete add_brush;
    delete remove_brush;
    delete table_brushes;
    delete m_displayMinThickness;
    delete m_displayMaxThickness;
    delete m_displaySmoothness;
    delete text_name;
    delete value_name;
    delete circle_min_thickness;
    delete circle_max_thickness;
    delete previsualization;
    delete previsualization_container;
}



void Brushes::setupButtons()
{
    //----------- Operations on the Buttons -----------------
	Q3HBox *containerButtons = new Q3HBox(this);
	add_brush = new QPushButton( i_add_brush, tr( "" ), containerButtons );
	add_brush -> setAutoDefault( false );
	add_brush -> setFlat( true );
	connect( add_brush, SIGNAL( clicked() ), SLOT( slotAddBrush() ) );
	QToolTip::add( add_brush, tr( "Add Brush" ) );

	remove_brush = new QPushButton( i_remove_brush, tr( "" ), containerButtons );
	remove_brush -> setAutoDefault( false );
	remove_brush -> setFlat( true );
	connect( remove_brush, SIGNAL( clicked() ), SLOT( slotRemoveBrush() ) );
	QToolTip::add( remove_brush, tr( "Remove Brush" ) );
	addChild(containerButtons);
}

void Brushes::setupTableBruches()
{
	    //------------ Operations on the Table of Brushes -------------
	Q3VBox *containerTableBruches = new Q3VBox(this);
	table_brushes = new Q3ListView( containerTableBruches, "", Qt::WStyle_NoBorder );
	table_brushes -> setSelectionMode( Q3ListView::Single );
	table_brushes -> setFont( QFont( QApplication::font().family(), 8 ) );
	table_brushes -> addColumn( tr( "Min" ), 30 ); //Min Thickness Column
	table_brushes -> addColumn( tr( "Max" ), 30 ); //Min Thickness Column
	table_brushes -> addColumn( tr( "Smo" ), 30 ); //Smoothness Column
	table_brushes -> addColumn( tr( "Name" ), 95 ); //Name Column
	table_brushes -> setResizeMode( Q3ListView::NoColumn );
	table_brushes -> setSorting( 10 ); //Not automatic sorting (10 > 4)
	connect( table_brushes, SIGNAL( selectionChanged() ), SLOT( slotSelectBrush() ) );
	default_brush = new Q3ListViewItem( table_brushes, "2", "5", "2", tr( "Brush" ) + QString( "0" ) );
    
	std::auto_ptr<Brush> ap_brush(new Brush( 2, 5, 2 ) );
    
	ap_brush.get() -> setNameBrush( tr( "Brush" ) + QString( "0" ) );
	KTStatus -> setCurrentBrush( ap_brush.get() );
	Q3PtrList<Brush> br;
	br.append( ap_brush.get() );
	KTStatus->currentDocument() -> setBrushes( br );
    
	ap_brush.release();
	addChild(containerTableBruches);
}

void Brushes::setupStaticText()
{
	m_displayMinThickness = new KTEditSpinBox(2, THICKNESS_MIN_MIN, THICKNESS_MIN_MAX,1, tr("Minimum Thickness"), this,"Display Min Thickness");
	addChild(m_displayMinThickness);
	connect( m_displayMinThickness, SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueMinThickness() ) );
	
	m_displayMaxThickness = new KTEditSpinBox(5, THICKNESS_MAX_MIN, THICKNESS_MAX_MAX,1, tr("Maximum Thickness"), this,"Display Max Thickness");
	addChild(m_displayMaxThickness);
	connect( m_displayMaxThickness, SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueMaxThickness() ) );
	
	m_displaySmoothness = new KTEditSpinBox(2,  SMOOTHNESS_MIN, SMOOTHNESS_MAX,1, tr("Smoothness"), this,"Display Smoothness");
	addChild(m_displaySmoothness);
	connect(m_displaySmoothness , SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueSmoothness() ) );
}

void Brushes::setupPrevisualizationArea()
{
    //---------------- Operations on the  -----------------------

	Q3HBox *containerPrevisualizationArea = new Q3HBox(this);
	Q3VBox *containerNameBrush = new Q3VBox(containerPrevisualizationArea);
	value_name = new QLineEdit( tr( "Brush0" ),  containerNameBrush);
	value_name -> setFont( QFont( font().family(), 8) );
	value_name -> setAlignment( Qt::AlignLeft );
	value_name -> setMaxLength( 10 );
	connect( value_name, SIGNAL( lostFocus() ), SLOT( slotChangeValueName() ) );
	connect( value_name, SIGNAL( returnPressed() ), SLOT( slotChangeValueName() ) );
	
	text_name = new QLabel( tr( "Brush Name" ), containerNameBrush );
	text_name -> setFont( QFont( font().family(), 7 ) );
	text_name -> setAlignment( Qt::AlignTop );
	
	std::auto_ptr<Q3Canvas> ap_previsualization(new Q3Canvas( 99, 99 ) );
	previsualization = ap_previsualization.get(); // DEBUG
    
	previsualization_container = new Q3CanvasView( /*ap_*/previsualization/*.get()*/, containerPrevisualizationArea );
	previsualization_container->adjustSize();
	std::auto_ptr<Q3CanvasEllipse> ap_circle_max_thickness(new Q3CanvasEllipse( 5, 5, /*ap_*/previsualization/*.get()*/) );
	circle_max_thickness = ap_circle_max_thickness.get();
    
	QBrush gray_brush = QBrush( QColor( 200, 200, 200 ) ); //For the circle_max_thickness
	circle_max_thickness -> setBrush( gray_brush );
	circle_max_thickness -> move( 50.0, 50.0 );
	circle_max_thickness -> setZ( 0.0 );
	circle_max_thickness -> show();

	std::auto_ptr<Q3CanvasEllipse> ap_circle_min_thickness(new Q3CanvasEllipse( 2, 2, /*ap_*/previsualization/*.get()*/) );
	circle_min_thickness = ap_circle_min_thickness.get();
    
	QBrush black_brush = QBrush( QColor( 0, 0, 0 ), Qt::SolidPattern ); //For the circle_min_thickness
	circle_min_thickness -> setBrush( black_brush );
	circle_min_thickness -> move( 50.0, 50.0 );
	circle_min_thickness -> setZ( 0.5 );
	circle_min_thickness -> show();

	table_brushes -> setSelected( default_brush, true ); //Select the default brush
    
	ap_previsualization.release();
	ap_circle_max_thickness.release();
	ap_circle_min_thickness.release();
	
	addChild(containerPrevisualizationArea);
}

//--------------- PUBLIC MEMBERS ---------------

void Brushes::loadBrushes( Q3PtrList<Brush> brushes )
{
    brush_max_value = 0;
    number_of_brushes = 0;

    table_brushes -> clear();

    Brush *b_it = brushes.first();
    Q3ListViewItem *b = new Q3ListViewItem( table_brushes, QString::number( b_it -> thicknessMinBrush() ),
    					  QString::number( b_it -> thicknessMaxBrush() ),
					  QString::number( b_it -> smoothnessBrush() ),
					  b_it -> nameBrush() );
    b_it = brushes.next();
    brush_max_value++;
    number_of_brushes++;
    for ( ; b_it; b_it = brushes.next() )
    {
        new Q3ListViewItem( table_brushes, table_brushes -> lastItem(),
	                   QString::number( b_it -> thicknessMinBrush() ),
    			   QString::number( b_it -> thicknessMaxBrush() ),
			   QString::number( b_it -> smoothnessBrush() ),
			   b_it -> nameBrush() );
        brush_max_value++;
        number_of_brushes++;
    }

    KTStatus->currentDocument() -> setBrushes( brushes );
    table_brushes -> setSelected( b, true );
    KTStatus -> setCurrentBrush( brushes.getFirst() );
}

//------------------- SLOTS ----------------------------

void Brushes::slotAddBrush()
{
    QString brush_number;
    brush_max_value++;
    brush_number.setNum( brush_max_value );
    number_of_brushes++;

    new Q3ListViewItem( table_brushes, table_brushes -> lastItem(), "2", "5", "2", tr( "Brush" ) + brush_number );

    Q3PtrList<Brush> br = KTStatus->currentDocument() -> getBrushes();
    
    Brush* n_brush = new Brush( 2, 5, 2 );
    try {
    n_brush -> setNameBrush( tr( "Brush" ) + brush_number );
    br.append( n_brush );
    KTStatus->currentDocument() -> setBrushes( br );
    }
    catch(...)
      {
	delete n_brush;
	throw;
	}
    
    KTStatus->currentDrawingArea() -> modifyDocument( true );
}

void Brushes::slotRemoveBrush()
{
   Q3ListViewItem *brush_to_remove = table_brushes -> selectedItem();
   int brush_pos = table_brushes -> itemPos( brush_to_remove );

   if ( number_of_brushes > 1 )
   {
      if ( QMessageBox::warning( this, tr( "Confirm Brush Elimination" ), tr( "Are you sure you want to delete the selected brush?" ),
           QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton ) == QMessageBox::Yes )
      {
          delete brush_to_remove;
          table_brushes -> setSelected( table_brushes -> currentItem(), true );
          table_brushes -> triggerUpdate();
      	  number_of_brushes--;

      	  Q3PtrList<Brush> br = KTStatus->currentDocument() -> getBrushes();
          br.setAutoDelete( true );
          br.remove( brush_pos / 16 );
	  br.setAutoDelete( false );
          KTStatus->currentDocument() -> setBrushes( br );

          Q3ListViewItem *selected_brush = table_brushes -> selectedItem();
          brush_pos = table_brushes -> itemPos( selected_brush );
          Q3PtrList<Brush> br1 = KTStatus->currentDocument() -> getBrushes();
          KTStatus -> setCurrentBrush( br1.at( brush_pos / 16 ) );
          KTStatus->currentDrawingArea() -> modifyDocument( true );
      }
   }
}


void Brushes::slotChangeValueMinThickness()
{
	int mint;

	Q3ListViewItem *current_brush = table_brushes -> selectedItem();

    //Validate that min is not greater than max
	if ( m_displayMinThickness->value() > m_displayMaxThickness -> value() )
	{
		m_displayMaxThickness->setValue( m_displayMinThickness-> value() );
	}

	current_brush -> setText( 0, QString::number(m_displayMinThickness->value()) );
	circle_min_thickness -> setSize( m_displayMinThickness-> value(), m_displayMinThickness-> value() );
	previsualization -> update();

	mint = ( current_brush -> text( 0 ) ).toInt();
	Brush *s_brush = KTStatus -> currentBrush();
	s_brush -> setThicknessMinBrush( mint );
	KTStatus->currentDrawingArea() -> modifyDocument( true );
	emit minThicknessChanged();
}

void Brushes::slotChangeValueMaxThickness()
{
	
    int maxt;
    Q3ListViewItem *current_brush = table_brushes -> selectedItem();

    //Validate that max is not less than min
    if ( m_displayMaxThickness->value() < m_displayMinThickness-> value() )
    {
	m_displayMinThickness->setValue( m_displayMaxThickness-> value() );
    }

    current_brush -> setText( 1, QString::number(m_displayMinThickness-> value()));
    circle_max_thickness -> setSize( m_displayMaxThickness-> value(),  m_displayMaxThickness-> value() );
    previsualization -> update();

    maxt = ( current_brush -> text( 1 ) ).toInt();
    Brush *s_brush = KTStatus -> currentBrush();
    s_brush -> setThicknessMaxBrush( maxt );
    KTStatus->currentDrawingArea() -> modifyDocument( true );

	emit maxThicknessChanged();
}

void Brushes::slotChangeValueSmoothness()
{
    int smo;
    Q3ListViewItem *current_brush = table_brushes -> selectedItem();
    current_brush -> setText( 2, QString::number(m_displaySmoothness->value()));

    smo = ( current_brush -> text( 2 ) ).toInt();
    Brush *s_brush = KTStatus -> currentBrush();
    s_brush -> setSmoothnessBrush( smo );
    KTStatus->currentDrawingArea() -> modifyDocument( true );

    emit smoothnessChanged();
#ifdef DEBUG_BRUSHES
std::cout << "smoothnessChanged()" << std::endl;
#endif
}

void Brushes::slotChangeValueName()
{
    Q3ListViewItem *current_brush = table_brushes -> selectedItem();

    //If the new name is the empty string, leave the name that it had before
    if ( value_name -> text() == "" )
    {
        current_brush -> setText( 3, current_name );
	value_name -> setText( current_name );
    }
    else
    {
        current_brush -> setText( 3, value_name -> text() );

        Brush *s_brush = KTStatus -> currentBrush();
        s_brush -> setNameBrush( value_name -> text() );
    }
    KTStatus->currentDrawingArea() -> modifyDocument( true );
}


void Brushes::slotSelectBrush()
{
    Q3ListViewItem *selected_brush = table_brushes -> selectedItem();
    int brush_pos = table_brushes -> itemPos( selected_brush );

    //Don't allow deselection
    if ( selected_brush == 0 )
    {
        table_brushes -> setSelected( table_brushes -> currentItem(), true );
	return;
    }

    Brush *s_brush = ( KTStatus->currentDocument() -> getBrushes() ).at( brush_pos / 16 );
    KTStatus -> setCurrentBrush( s_brush );

    current_name = selected_brush -> text( 3 );

    //Disconnect the signals of the slots to prevent chained callbacks
    disconnect( m_displayMinThickness, 0, 0, 0 );
    disconnect( m_displayMaxThickness, 0, 0, 0 );
    disconnect( m_displaySmoothness,   0, 0, 0 );
    disconnect( value_name, 0, 0, 0 );

    m_displayMinThickness->setValue( (selected_brush -> text( 0 )).toInt());
    m_displayMaxThickness->setValue( (selected_brush -> text( 1 )).toInt());
    m_displaySmoothness-> setValue( ( selected_brush -> text( 2 ) ).toInt() );

    circle_min_thickness -> setSize( ( selected_brush -> text( 0 ) ).toInt(), ( selected_brush -> text( 0 ) ).toInt() );
    circle_max_thickness -> setSize( ( selected_brush -> text( 1 ) ).toInt(), ( selected_brush -> text( 1 ) ).toInt() );
    previsualization -> update();

    //Connect again the signals to the slots
    connect( value_name, SIGNAL( lostFocus() ), SLOT( slotChangeValueName() ) );
    connect( value_name, SIGNAL( returnPressed() ), SLOT( slotChangeValueName() ) );
    connect( m_displayMinThickness, SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueMinThickness() ) );
    connect( m_displayMaxThickness, SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueMaxThickness() ) );
    connect( m_displaySmoothness  , SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueSmoothness()   ) );

}

//--------------------- EVENTS AND OTHER FUNCTIONS --------------------------------

// void Brushes::closeEvent( QCloseEvent* close_event )
// {
// 	qDebug("This Closed");
//     Q_CHECK_PTR( close_event );
// //     assigned_menu -> setItemChecked( assigned_item, false );
// //     assigned_tb_button -> setDown( false );
//      close_event -> accept();
// }

