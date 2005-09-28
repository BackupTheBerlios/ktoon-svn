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

#include <QToolTip>
#include <QBrush>
#include <QColor>
#include <QValidator>
#include <QMessageBox>
//Added by qt3to4:
#include <QPixmap>
#include <QList>
#include <QLabel>

#include "brushes.h"
// #include "images.h"
#include "ktapplication.h"
#include "status.h"

#include <q3dockwindow.h>
#include <memory>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ktdebug.h"
//--------------- CONSTRUCTOR --------------------

Brushes::Brushes( QWidget *parent, Qt::WFlags style/*, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button*/ )
	: KTModuleWidgetBase( parent, "Brushes" )
{
	Q_CHECK_PTR( parent );

	setCaption( tr( "Brushes" ) );

	move( 800, 88 );
	
	
	brush_max_value = 0;
	number_of_brushes = 1;

    //Icon Initializations
	i_add_brush = QPixmap( KTOON_HOME+"/images/icons/plussign.xpm" );
	i_remove_brush = QPixmap( KTOON_HOME+"/images/icons/minussign.xpm" );
	
	setupButtons();
	setupTableBruches();
	setupStaticText();
	setupPrevisualizationArea();
	adjustSize();
    
}

//-------------- DESTRUCTOR -----------------

Brushes::~Brushes()
{
    delete m_addBrush;
    delete m_removeBrush;
    delete table_brushes;
    delete m_displayMinThickness;
    delete m_displayMaxThickness;
    delete m_displaySmoothness;
    delete m_textName;
    delete m_valueName;
}



void Brushes::setupButtons()
{
    //----------- Operations on the Buttons -----------------
	QGroupBox *containerButtons = new QGroupBox(this);
	QHBoxLayout *layout = new QHBoxLayout(containerButtons);
	layout->setMargin(0);
	m_addBrush = new QPushButton( i_add_brush, tr( "" ), containerButtons );
	m_addBrush -> setAutoDefault( false );
// 	m_addBrush -> setFlat( true );
	connect( m_addBrush, SIGNAL( clicked() ), SLOT( slotAddBrush() ) );
	QToolTip::add( m_addBrush, tr( "Add Brush" ) );
	layout->addWidget(m_addBrush);
	m_removeBrush = new QPushButton( i_remove_brush, tr( "" ), containerButtons );
	m_removeBrush -> setAutoDefault( false );
// 	m_removeBrush -> setFlat( true );
	connect( m_removeBrush, SIGNAL( clicked() ), SLOT( slotRemoveBrush() ) );
	QToolTip::add( m_removeBrush, tr( "Remove Brush" ) );
	layout->addWidget(m_removeBrush);
	addChild(containerButtons);
	
}

void Brushes::setupTableBruches()
{
	    //------------ Operations on the Table of Brushes -------------
	QGroupBox *containerTableBrushes = new QGroupBox(this);
// 	m_tableBrushes = new QTableWidget(1,4,this);
// 	QStringList labels;
// 	labels << tr("Min") << tr( "Max" ) << tr( "Smo" ) << tr( "Name" );
// 	m_tableBrushes->setHorizontalHeaderLabels ( labels );
// 	m_tableBrushes->adjustSize ();
// 	addChild(m_tableBrushes);
	QVBoxLayout *layout = new QVBoxLayout(containerTableBrushes);
	table_brushes = new Q3ListView( containerTableBrushes, "", Qt::WStyle_NoBorder );
	layout->addWidget(table_brushes);
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
    
	ap_brush.get()->setNameBrush( tr( "Brush" ) + QString( "0" ) );
	KTStatus -> setCurrentBrush( ap_brush.get() );
	QList<Brush*> br;
	br.append( ap_brush.get() );
	KTStatus->currentDocument()->setBrushes( br );
    
	ap_brush.release();
	addChild(containerTableBrushes);
}

void Brushes::setupStaticText()
{
	m_displayMinThickness = new KTEditSpinBox(2, THICKNESS_MIN_MIN, THICKNESS_MIN_MAX,1, tr("Minimum Thickness"), this,"Display Min Thickness");
	addChild(m_displayMinThickness);
	connect( m_displayMinThickness, SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueMinThickness(int) ) );
	
	m_displayMaxThickness = new KTEditSpinBox(5, THICKNESS_MAX_MIN, THICKNESS_MAX_MAX,1, tr("Maximum Thickness"), this,"Display Max Thickness");
	addChild(m_displayMaxThickness);
	connect( m_displayMaxThickness, SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueMaxThickness(int) ) );
	m_displaySmoothness = new KTEditSpinBox(2,  SMOOTHNESS_MIN, SMOOTHNESS_MAX,1, tr("Smoothness"), this,"Display Smoothness");
	addChild(m_displaySmoothness);
// 	connect(m_displaySmoothness , SIGNAL( valueChanged( int ) ), SLOT( slotChangeValueSmoothness() ) );
	
}

void Brushes::setupPrevisualizationArea()
{
    //---------------- Operations on the  -----------------------

	m_displayBrush = new KTDisplayBrush(m_displayMinThickness->value(), m_displayMaxThickness->value(),  this);
	m_displayBrush->setMinimumSize(THICKNESS_MAX_MAX+10,THICKNESS_MAX_MAX+10);
	addChild(m_displayBrush);
	m_valueName = new QLineEdit( tr( "Brush0" ),  this);
// 	m_valueName -> setFont( QFont( font().family(), 8) );
	m_valueName -> setAlignment( Qt::AlignLeft );
	m_valueName -> setMaxLength( 10 );
	connect( m_valueName, SIGNAL( lostFocus() ), SLOT( slotChangeValueName() ) );
	connect( m_valueName, SIGNAL( returnPressed() ), SLOT( slotChangeValueName() ) );
	addChild(m_valueName);
	m_textName = new QLabel( tr( "Brush Name" ), this );
	m_textName->setAlignment( Qt::AlignTop );
	addChild(m_textName);
}

//--------------- PUBLIC MEMBERS ---------------

void Brushes::loadBrushes( QList<Brush*> brushes )
{
    brush_max_value = 0;
    number_of_brushes = 0;

    table_brushes -> clear();

//     Brush *b_it = brushes.first();
    Q3ListViewItem *b = new Q3ListViewItem( table_brushes, QString::number( brushes[0] -> thicknessMinBrush() ),  QString::number( brushes[0]-> thicknessMaxBrush() ), QString::number( brushes[0]-> smoothnessBrush() ), brushes[0]-> nameBrush() );
//     b_it = brushes.next();
    brush_max_value++;
    number_of_brushes++;
    for(int i =1; i < brushes.count(); i++)
    {
	/*for ( ; b_it; b_it = brushes.next() )
	{*/
	    Brush * b_it = brushes[i];
        new Q3ListViewItem( table_brushes, table_brushes -> lastItem(),
	                   QString::number( b_it -> thicknessMinBrush() ),
    			   QString::number( b_it -> thicknessMaxBrush() ),
			   QString::number( b_it -> smoothnessBrush() ),
			   b_it -> nameBrush() );
        brush_max_value++;
        number_of_brushes++;
    }

    KTStatus->currentDocument()->setBrushes(brushes );
    table_brushes -> setSelected( b, true );
    KTStatus -> setCurrentBrush( brushes.first() );
}

//------------------- SLOTS ----------------------------

void Brushes::slotAddBrush()
{
    QString brush_number;
    brush_max_value++;
    brush_number.setNum( brush_max_value );
    number_of_brushes++;

    new Q3ListViewItem( table_brushes, table_brushes -> lastItem(), "2", "5", "2", tr( "Brush" ) + brush_number );

    QList<Brush*> br = KTStatus->currentDocument() -> getBrushes();
    
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

      	  QList<Brush*> br = KTStatus->currentDocument() -> getBrushes();
//           br.setAutoDelete( true );
          br.removeAt( brush_pos / 16 );
// 	  br.setAutoDelete( false );
          KTStatus->currentDocument() -> setBrushes( br );

          Q3ListViewItem *selected_brush = table_brushes -> selectedItem();
          brush_pos = table_brushes -> itemPos( selected_brush );
          QList<Brush*> br1 = KTStatus->currentDocument() -> getBrushes();
          KTStatus -> setCurrentBrush( br1.at( brush_pos / 16 ) );
          KTStatus->currentDrawingArea() -> modifyDocument( true );
      }
   }
}


void Brushes::slotChangeValueMinThickness(int value)
{
	int mint;
// 	Q3ListViewItem *current_brush = table_brushes->selectedItem();
    //Validate that min is not greater than max
	if ( m_displayMinThickness->value() > m_displayMaxThickness->value() )
	{
		m_displayMaxThickness->setValue( m_displayMinThickness-> value() );
	}

// 	current_brush -> setText( 0, QString::number(m_displayMinThickness->value()) );
// 	m_circleMinThickness -> setSize( m_displayMinThickness-> value(), m_displayMinThickness-> value() );
// 	m_previsualization -> update();
	m_displayBrush->changeMinThickness(value);
// 	mint = ( current_brush -> text( 0 ) ).toInt();
	Brush *s_brush = KTStatus -> currentBrush();
	s_brush -> setThicknessMinBrush( mint );
	KTStatus->currentDrawingArea() -> modifyDocument( true );
	emit minThicknessChanged();
}

void Brushes::slotChangeValueMaxThickness(int value)
{
// 	int maxt;
// 	Q3ListViewItem *current_brush = table_brushes -> selectedItem();

    //Validate that max is not less than min
	if ( m_displayMaxThickness->value() < m_displayMinThickness-> value() )
	{
		m_displayMinThickness->setValue( m_displayMaxThickness-> value() );
	}

// 	current_brush -> setText( 1, QString::number(m_displayMinThickness-> value()));
//     m_circleMaxThickness -> setSize( m_displayMaxThickness-> value(),  m_displayMaxThickness-> value() );
//     m_previsualization -> update();

	m_displayBrush->changeMaxThickness(value);
// 	maxt = ( current_brush -> text( 1 ) ).toInt();
	Brush *s_brush = KTStatus -> currentBrush();
// 	s_brush -> setThicknessMaxBrush( maxt );
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
    if ( m_valueName -> text() == "" )
    {
        current_brush -> setText( 3, current_name );
	m_valueName -> setText( current_name );
    }
    else
    {
        current_brush -> setText( 3, m_valueName -> text() );

        Brush *s_brush = KTStatus -> currentBrush();
        s_brush -> setNameBrush( m_valueName -> text() );
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
    disconnect( m_valueName, 0, 0, 0 );

    m_displayMinThickness->setValue( (selected_brush -> text( 0 )).toInt());
    m_displayMaxThickness->setValue( (selected_brush -> text( 1 )).toInt());
    m_displaySmoothness-> setValue( ( selected_brush -> text( 2 ) ).toInt() );
    //FIXME: hacer esto con el displayBrush
//     m_circleMinThickness -> setSize( ( selected_brush -> text( 0 ) ).toInt(), ( selected_brush -> text( 0 ) ).toInt() );
//     m_circleMaxThickness -> setSize( ( selected_brush -> text( 1 ) ).toInt(), ( selected_brush -> text( 1 ) ).toInt() );
//     m_previsualization -> update();

    //Connect again the signals to the slots
    connect( m_valueName, SIGNAL( lostFocus() ), SLOT( slotChangeValueName() ) );
    connect( m_valueName, SIGNAL( returnPressed() ), SLOT( slotChangeValueName() ) );
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

