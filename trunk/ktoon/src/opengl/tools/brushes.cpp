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

#include "brushes.h"
#include "ktoon.h"
#include "images.h"

#include <memory>


//--------------- CONSTRUCTOR --------------------

Brushes::Brushes( QWidget *parent, WFlags style, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button )
    : QDialog( parent, "Brushes", false, style )
{
    Q_CHECK_PTR( parent );
    Q_CHECK_PTR( in_assigned_menu );
    Q_CHECK_PTR( assig_tb_button );

    //Initializations
    setCaption( tr( "Brushes" ) );
    setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    resize( 200, 360 );
    setMinimumSize( 200, 360 );
    setMaximumSize( 200, 360 );
    move( 800, 88 );
    parent_widget = parent;
    assigned_menu = in_assigned_menu;
    assigned_item = id_assigned_item;
    assigned_tb_button = assig_tb_button;
    brush_max_value = 0;
    number_of_brushes = 1;
    k_toon = ( KToon * )parent_widget;

    //Icon Initializations
    i_add_brush = QPixmap( plussign_xpm );
    i_remove_brush = QPixmap( minussign_xpm );

    //----------- Operations on the Buttons -----------------

    add_brush = new QPushButton( i_add_brush, tr( "" ), this );
    add_brush -> resize( 20, 20 );
    add_brush -> move( 5, 5 );
    add_brush -> setAutoDefault( false );
    add_brush -> setFlat( true );
    connect( add_brush, SIGNAL( clicked() ), SLOT( slotAddBrush() ) );
    QToolTip::add( add_brush, tr( "Add Brush" ) );

    remove_brush = new QPushButton( i_remove_brush, tr( "" ), this );
    remove_brush -> resize( 20, 20 );
    remove_brush -> move( 30, 5 );
    remove_brush -> setAutoDefault( false );
    remove_brush -> setFlat( true );
    connect( remove_brush, SIGNAL( clicked() ), SLOT( slotRemoveBrush() ) );
    QToolTip::add( remove_brush, tr( "Remove Brush" ) );

    //------------ Operations on the Table of Brushes -------------

    table_brushes = new QListView( this, "", Qt::WStyle_NoBorder );
    table_brushes -> resize( 190, 120 );
    table_brushes -> move( 5, 30 );
    table_brushes -> setSelectionMode( QListView::Single );
    table_brushes -> setFont( QFont( "helvetica", 8 ) );
    table_brushes -> addColumn( tr( "Min" ), 30 ); //Min Thickness Column
    table_brushes -> addColumn( tr( "Max" ), 30 ); //Min Thickness Column
    table_brushes -> addColumn( tr( "Smo" ), 30 ); //Smoothness Column
    table_brushes -> addColumn( tr( "Name" ), 95 ); //Name Column
    table_brushes -> setResizeMode( QListView::NoColumn );
    table_brushes -> setSorting( 10 ); //Not automatic sorting (10 > 4)
    connect( table_brushes, SIGNAL( selectionChanged() ), SLOT( slotSelectBrush() ) );

    QListViewItem *default_brush = new QListViewItem( table_brushes, "2", "5", "2", tr( "Brush" ) + QString( "0" ) );
    
    std::auto_ptr<Brush> ap_brush(new Brush( 2, 5, 2 ) );
    
    ap_brush.get() -> setNameBrush( tr( "Brush" ) + QString( "0" ) );
    k_toon -> currentStatus() -> setCurrentBrush( ap_brush.get() );
    QPtrList<Brush> br;
    br.append( ap_brush.get() );
    k_toon -> document() -> setBrushes( br );
    
    ap_brush.release();

    //------------- Operations on the Textfields -------------

    value_min_thickness = new QLineEdit( "2", this );
    value_min_thickness -> resize( 20, 20 );
    value_min_thickness -> move( 5, table_brushes -> height() + table_brushes -> y() + 10 );
    value_min_thickness -> setFont( QFont( "helvetica", 6 ) );
    value_min_thickness -> setAlignment( Qt::AlignRight );
    value_min_thickness -> setMaxLength( 2 );
    value_min_thickness -> setValidator( new QIntValidator( THICKNESS_MIN_MIN, THICKNESS_MIN_MAX, this ) );
    connect( value_min_thickness, SIGNAL( lostFocus() ), SLOT( slotChangeSliderMinThickness() ) );
    connect( value_min_thickness, SIGNAL( returnPressed() ), SLOT( slotChangeSliderMinThickness() ) );

    value_max_thickness = new QLineEdit( "5", this );
    value_max_thickness -> resize( 20, 20 );
    value_max_thickness -> move( 5, value_min_thickness -> height() + value_min_thickness -> y() + 10 );
    value_max_thickness -> setFont(QFont( "helvetica", 6 ) );
    value_max_thickness -> setAlignment( Qt::AlignRight );
    value_max_thickness -> setMaxLength( 2 );
    value_max_thickness -> setValidator( new QIntValidator( THICKNESS_MAX_MIN, THICKNESS_MAX_MAX, this ) );
    connect( value_max_thickness, SIGNAL( lostFocus() ), SLOT( slotChangeSliderMaxThickness() ) );
    connect( value_max_thickness, SIGNAL( returnPressed() ), SLOT( slotChangeSliderMaxThickness() ) );

    value_smoothness = new QLineEdit( "2", this );
    value_smoothness -> resize( 20, 20 );
    value_smoothness -> move( 5, value_max_thickness -> height() + value_max_thickness -> y() + 10 );
    value_smoothness -> setFont( QFont( "helvetica", 6 ) );
    value_smoothness -> setAlignment( Qt::AlignRight );
    value_smoothness -> setMaxLength( 1 );
    value_smoothness -> setValidator( new QIntValidator( SMOOTHNESS_MIN, SMOOTHNESS_MAX, this ) );
    connect( value_smoothness, SIGNAL( textChanged( const QString & ) ), SLOT( slotChangeSliderSmoothness() ) );

    value_name = new QLineEdit( tr( "Brush0" ), this );
    value_name -> resize( 80, 20 );
    value_name -> move( 5, value_smoothness -> height() + value_smoothness -> y() + 10 + 30 );
    value_name -> setFont( QFont( "helvetica", 8) );
    value_name -> setAlignment( Qt::AlignLeft );
    value_name -> setMaxLength( 10 );
    connect( value_name, SIGNAL( lostFocus() ), SLOT( slotChangeValueName() ) );
    connect( value_name, SIGNAL( returnPressed() ), SLOT( slotChangeValueName() ) );

    //------------- Operations on Static Texts ----------

    text_min_thickness = new QLabel( tr( "Minimum Thickness" ), this );
    text_min_thickness -> setFont( QFont( "helvetica", 6 ) );
    text_min_thickness -> move( value_min_thickness -> width() + value_min_thickness -> x() + 5, value_min_thickness -> y() );

    text_max_thickness = new QLabel( tr( "Maximum Thickness" ), this);
    text_max_thickness -> setFont( QFont( "helvetica", 6 ) );
    text_max_thickness -> move( value_max_thickness -> width() + value_max_thickness -> x() + 5, value_max_thickness -> y() );

    text_smoothness = new QLabel( tr( "Smoothness" ), this );
    text_smoothness -> setFont( QFont( "helvetica", 6 ) );
    text_smoothness -> move( value_smoothness -> width() + value_smoothness -> x() + 5, value_smoothness -> y() );

    text_name = new QLabel( tr( "Brush Name" ), this );
    text_name -> setFont( QFont( "helvetica", 7 ) );
    text_name -> resize( 100, 25 );
    text_name -> setAlignment( Qt::AlignTop );
    text_name -> move( 5, value_name -> height() + value_name -> y() + 1 );

    //------------- Operations on the Sliders -------------

    slider_min_thickness = new QSlider( THICKNESS_MIN_MIN, THICKNESS_MIN_MAX, 1, 2, Qt::Horizontal, this );
    slider_min_thickness -> setTickmarks( QSlider::Below );
    slider_min_thickness -> setTickInterval( 10 );
    slider_min_thickness -> resize(165, 10);
    slider_min_thickness -> move( text_min_thickness -> x(), text_min_thickness -> y() - 2 );
    connect( slider_min_thickness, SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueMinThickness() ) );

    slider_max_thickness = new QSlider( THICKNESS_MAX_MIN, THICKNESS_MAX_MAX, 1, 5, Qt::Horizontal, this );
    slider_max_thickness -> setTickmarks( QSlider::Below );
    slider_max_thickness -> setTickInterval( 10 );
    slider_max_thickness -> resize( 165, 10 );
    slider_max_thickness -> move( text_max_thickness -> x(), text_max_thickness -> y() - 2) ;
    connect( slider_max_thickness, SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueMaxThickness() ) );

    slider_smoothness = new QSlider( SMOOTHNESS_MIN, SMOOTHNESS_MAX, 1, 2, Qt::Horizontal, this );
    slider_smoothness -> setTickmarks( QSlider::Below );
    slider_smoothness -> resize( 165, 10 );
    slider_smoothness -> move( text_smoothness -> x(), text_smoothness -> y() - 2 );
    connect( slider_smoothness, SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueSmoothness() ) );

    //---------------- Operations on the previsulization area -----------------------

    std::auto_ptr<QCanvas> ap_previsualization(new QCanvas( 99, 99 ) );
    previsualization = ap_previsualization.get(); // DEBUG
    
    previsualization_container = new QCanvasView( /*ap_*/previsualization/*.get()*/, this );
    previsualization_container -> resize( 103, 103 );
    previsualization_container -> move( value_name -> x() + value_name -> width() + 5, value_smoothness -> height() + value_smoothness -> y() + 10 );

    std::auto_ptr<QCanvasEllipse> ap_circle_max_thickness(new QCanvasEllipse( 5, 5, /*ap_*/previsualization/*.get()*/) );
    circle_max_thickness = ap_circle_max_thickness.get();
    
    QBrush gray_brush = QBrush( QColor( 200, 200, 200 ) ); //For the circle_max_thickness
    circle_max_thickness -> setBrush( gray_brush );
    circle_max_thickness -> move( 50.0, 50.0 );
    circle_max_thickness -> setZ( 0.0 );
    circle_max_thickness -> show();

    std::auto_ptr<QCanvasEllipse> ap_circle_min_thickness(new QCanvasEllipse( 2, 2, /*ap_*/previsualization/*.get()*/) );
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
  
}

//-------------- DESTRUCTOR -----------------

Brushes::~Brushes()
{
    delete add_brush;
    delete remove_brush;
    delete table_brushes;
    delete text_min_thickness;
    delete text_max_thickness;
    delete text_smoothness;
    delete text_name;
    delete value_min_thickness;
    delete value_max_thickness;
    delete value_smoothness;
    delete value_name;
    delete slider_min_thickness;
    delete slider_max_thickness;
    delete slider_smoothness;
    delete circle_min_thickness;
    delete circle_max_thickness;
    delete previsualization;
    delete previsualization_container;
}

//--------------- PUBLIC MEMBERS ---------------

void Brushes::loadBrushes( QPtrList<Brush> brushes )
{
    brush_max_value = 0;
    number_of_brushes = 0;

    table_brushes -> clear();

    Brush *b_it = brushes.first();
    QListViewItem *b = new QListViewItem( table_brushes, QString::number( b_it -> thicknessMinBrush() ),
    					  QString::number( b_it -> thicknessMaxBrush() ),
					  QString::number( b_it -> smoothnessBrush() ),
					  b_it -> nameBrush() );
    b_it = brushes.next();
    brush_max_value++;
    number_of_brushes++;
    for ( ; b_it; b_it = brushes.next() )
    {
        new QListViewItem( table_brushes, table_brushes -> lastItem(),
	                   QString::number( b_it -> thicknessMinBrush() ),
    			   QString::number( b_it -> thicknessMaxBrush() ),
			   QString::number( b_it -> smoothnessBrush() ),
			   b_it -> nameBrush() );
        brush_max_value++;
        number_of_brushes++;
    }

    k_toon -> document() -> setBrushes( brushes );
    table_brushes -> setSelected( b, true );
    k_toon -> currentStatus() -> setCurrentBrush( brushes.getFirst() );
}

//------------------- SLOTS ----------------------------

void Brushes::slotAddBrush()
{
    QString brush_number;
    brush_max_value++;
    brush_number.setNum( brush_max_value );
    number_of_brushes++;

    new QListViewItem( table_brushes, table_brushes -> lastItem(), "2", "5", "2", tr( "Brush" ) + brush_number );

    QPtrList<Brush> br = k_toon -> document() -> getBrushes();
    
    Brush* n_brush = new Brush( 2, 5, 2 );
    try {
    n_brush -> setNameBrush( tr( "Brush" ) + brush_number );
    br.append( n_brush );
    k_toon -> document() -> setBrushes( br );
    }
    catch(...)
      {
	delete n_brush;
	throw;
	}
    
    k_toon -> drawingArea() -> modifyDocument( true );
}

void Brushes::slotRemoveBrush()
{
   QListViewItem *brush_to_remove = table_brushes -> selectedItem();
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

      	  QPtrList<Brush> br = k_toon -> document() -> getBrushes();
          br.setAutoDelete( true );
          br.remove( brush_pos / 16 );
	  br.setAutoDelete( false );
          k_toon -> document() -> setBrushes( br );

          QListViewItem *selected_brush = table_brushes -> selectedItem();
          brush_pos = table_brushes -> itemPos( selected_brush );
          QPtrList<Brush> br1 = k_toon -> document() -> getBrushes();
          k_toon -> currentStatus() -> setCurrentBrush( br1.at( brush_pos / 16 ) );
          k_toon -> drawingArea() -> modifyDocument( true );
      }
   }
}

void Brushes::slotChangeValueMinThickness()
{
    QString conversion;
    int mint;
    conversion.setNum( slider_min_thickness -> value() );
    value_min_thickness -> setText( conversion );

    QListViewItem *current_brush = table_brushes -> selectedItem();

    //Validate that min is not greater than max
    if ( slider_min_thickness -> value() > slider_max_thickness -> value() )
    {
        value_max_thickness -> setText( conversion );
	slider_max_thickness -> setValue( slider_min_thickness -> value() );
    }

    current_brush -> setText( 0, conversion );
    circle_min_thickness -> setSize( slider_min_thickness -> value(), slider_min_thickness -> value() );
    previsualization -> update();

    mint = ( current_brush -> text( 0 ) ).toInt();
    Brush *s_brush = k_toon -> currentStatus() -> currentBrush();
    s_brush -> setThicknessMinBrush( mint );
    k_toon -> drawingArea() -> modifyDocument( true );

    emit minThicknessChanged();
}

void Brushes::slotChangeValueMaxThickness()
{
    QString conversion;
    int maxt;
    conversion.setNum( slider_max_thickness -> value() );
    value_max_thickness -> setText( conversion );

    QListViewItem *current_brush = table_brushes -> selectedItem();

    //Validate that max is not less than min
    if ( slider_max_thickness -> value() < slider_min_thickness -> value() )
    {
        value_min_thickness -> setText( conversion );
	slider_min_thickness -> setValue( slider_max_thickness -> value() );
    }

    current_brush -> setText( 1, conversion );
    circle_max_thickness -> setSize( slider_max_thickness -> value(), slider_max_thickness -> value() );
    previsualization -> update();

    maxt = ( current_brush -> text( 1 ) ).toInt();
    Brush *s_brush = k_toon -> currentStatus() -> currentBrush();
    s_brush -> setThicknessMaxBrush( maxt );
    k_toon -> drawingArea() -> modifyDocument( true );

    emit maxThicknessChanged();
}

void Brushes::slotChangeValueSmoothness()
{
    QString conversion;
    int smo;
    conversion.setNum( slider_smoothness -> value() );
    value_smoothness -> setText( conversion );

    QListViewItem *current_brush = table_brushes -> selectedItem();
    current_brush -> setText( 2, conversion );

    smo = ( current_brush -> text( 2 ) ).toInt();
    Brush *s_brush = k_toon -> currentStatus() -> currentBrush();
    s_brush -> setSmoothnessBrush( smo );
    k_toon -> drawingArea() -> modifyDocument( true );

    emit smoothnessChanged();
#ifdef DEBUG_BRUSHES
std::cout << "smoothnessChanged()" << std::endl;
#endif
}

void Brushes::slotChangeValueName()
{
    QListViewItem *current_brush = table_brushes -> selectedItem();

    //If the new name is the empty string, leave the name that it had before
    if ( value_name -> text() == "" )
    {
        current_brush -> setText( 3, current_name );
	value_name -> setText( current_name );
    }
    else
    {
        current_brush -> setText( 3, value_name -> text() );

        Brush *s_brush = k_toon -> currentStatus() -> currentBrush();
        s_brush -> setNameBrush( value_name -> text() );
    }
    k_toon -> drawingArea() -> modifyDocument( true );
}

void Brushes::slotChangeSliderMinThickness()
{
    int conversion;
    int mint;
    conversion = ( value_min_thickness -> text() ).toInt();

    //Validate that value is not zero or the empty string
    if ( conversion == 0 || ( value_min_thickness -> text() ).isEmpty() )
    {
        conversion = 1;
	value_min_thickness -> setText( "1" );
    }

    //Validate that min is not greater than max
    if ( conversion > ( value_max_thickness -> text() ).toInt() )
    {
        value_max_thickness -> setText( value_min_thickness -> text() );
	slider_max_thickness -> setValue( conversion );
    }

    slider_min_thickness -> setValue( conversion );
    circle_min_thickness -> setSize( conversion, conversion );
    previsualization -> update();

    QListViewItem *current_brush = table_brushes -> selectedItem();
    current_brush -> setText( 0, value_min_thickness -> text() );

    mint = ( current_brush -> text( 0 ) ).toInt();
    Brush *s_brush = k_toon -> currentStatus() -> currentBrush();
    s_brush -> setThicknessMinBrush( mint );
    k_toon -> drawingArea() -> modifyDocument( true );

    emit minThicknessChanged();
}

void Brushes::slotChangeSliderMaxThickness()
{
    int conversion;
    int maxt;
    conversion = ( value_max_thickness -> text() ).toInt();

    //Validate that value is not zero or the empty string
    if ( conversion == 0 || ( value_max_thickness -> text() ).isEmpty() )
    {
        conversion = 1;
	value_max_thickness -> setText( "1" );
    }

    //Validate that max is not less than min
    if ( conversion < ( value_min_thickness -> text() ).toInt() )
    {
        value_min_thickness -> setText( value_max_thickness -> text() );
	slider_min_thickness -> setValue( conversion );
    }

    slider_max_thickness -> setValue( conversion );
    circle_max_thickness -> setSize( conversion, conversion );
    previsualization -> update();

    QListViewItem *current_brush = table_brushes -> selectedItem();
    current_brush -> setText( 1, value_max_thickness -> text() );

    maxt = ( current_brush -> text( 1 ) ).toInt();
    Brush *s_brush = k_toon -> currentStatus() -> currentBrush();
    s_brush -> setThicknessMaxBrush( maxt );
    k_toon -> drawingArea() -> modifyDocument( true );

    emit maxThicknessChanged();
}

void Brushes::slotChangeSliderSmoothness()
{
    int conversion;
    int smo;
    conversion = ( value_smoothness -> text() ).toInt();

    //Validate that value is not the empty string
    if ( ( value_smoothness -> text() ).isEmpty() )
    {
        conversion = 0;
	value_smoothness -> setText( "0" );
    }

    slider_smoothness -> setValue( conversion );

    QListViewItem *current_brush = table_brushes -> selectedItem();
    current_brush -> setText( 2, value_smoothness -> text() );

    smo = ( current_brush -> text( 2 ) ).toInt();
    Brush *s_brush = k_toon -> currentStatus() -> currentBrush();
    s_brush -> setSmoothnessBrush( smo );
    k_toon -> drawingArea() -> modifyDocument( true );

    emit smoothnessChanged();
}

void Brushes::slotSelectBrush()
{
    QListViewItem *selected_brush = table_brushes -> selectedItem();
    int brush_pos = table_brushes -> itemPos( selected_brush );

    //Don't allow deselection
    if ( selected_brush == 0 )
    {
        table_brushes -> setSelected( table_brushes -> currentItem(), true );
	return;
    }

    Brush *s_brush = ( k_toon -> document() -> getBrushes() ).at( brush_pos / 16 );
    k_toon -> currentStatus() -> setCurrentBrush( s_brush );

    current_name = selected_brush -> text( 3 );

    //Disconnect the signals of the slots to prevent chained callbacks
    disconnect( value_min_thickness, 0, 0, 0 );
    disconnect( value_max_thickness, 0, 0, 0 );
    disconnect( value_smoothness, 0, 0, 0 );
    disconnect( value_name, 0, 0, 0 );
    disconnect( slider_min_thickness, 0, 0, 0 );
    disconnect( slider_max_thickness, 0, 0, 0 );
    disconnect( slider_smoothness, 0, 0, 0 );

    value_min_thickness -> setText( selected_brush -> text( 0 ) );
    value_max_thickness -> setText( selected_brush -> text( 1 ) );
    value_smoothness -> setText( selected_brush -> text( 2 ) );
    value_name -> setText( selected_brush -> text( 3 ) );

    slider_min_thickness -> setValue( ( selected_brush -> text( 0 ) ).toInt() );
    slider_max_thickness -> setValue( ( selected_brush -> text( 1 ) ).toInt() );
    slider_smoothness -> setValue( ( selected_brush -> text( 2 ) ).toInt() );

    circle_min_thickness -> setSize( ( selected_brush -> text( 0 ) ).toInt(), ( selected_brush -> text( 0 ) ).toInt() );
    circle_max_thickness -> setSize( ( selected_brush -> text( 1 ) ).toInt(), ( selected_brush -> text( 1 ) ).toInt() );
    previsualization -> update();

    //Connect again the signals to the slots
    connect( value_min_thickness, SIGNAL( lostFocus() ), SLOT( slotChangeSliderMinThickness() ) );
    connect( value_min_thickness, SIGNAL( returnPressed() ), SLOT( slotChangeSliderMinThickness() ) );
    connect( value_max_thickness, SIGNAL( lostFocus() ), SLOT( slotChangeSliderMaxThickness() ) );
    connect( value_max_thickness, SIGNAL( returnPressed() ), SLOT( slotChangeSliderMaxThickness() ) );
    connect( value_smoothness, SIGNAL( textChanged( const QString & ) ), SLOT( slotChangeSliderSmoothness() ) );
    connect( value_name, SIGNAL( lostFocus() ), SLOT( slotChangeValueName() ) );
    connect( value_name, SIGNAL( returnPressed() ), SLOT( slotChangeValueName() ) );
    connect( slider_min_thickness, SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueMinThickness() ) );
    connect( slider_max_thickness, SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueMaxThickness() ) );
    connect( slider_smoothness, SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueSmoothness() ) );

}

//--------------------- EVENTS AND OTHER FUNCTIONS --------------------------------

void Brushes::closeEvent( QCloseEvent* close_event )
{
    Q_CHECK_PTR( close_event );
    assigned_menu -> setItemChecked( assigned_item, false );
    assigned_tb_button -> setDown( false );
    close_event -> accept();
}
