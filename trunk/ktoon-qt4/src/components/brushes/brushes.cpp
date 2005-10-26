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

#include "brushes.h"
#include "ktapplication.h"
// #include "status.h"
#include "ktdebug.h"
#include "ktimagebutton.h"

#include <QToolTip>
#include <QBrush>
#include <QColor>
#include <QValidator>
#include <QMessageBox>
#include <QPixmap>
#include <QList>
#include <QLabel>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QGroupBox>

#include <memory>
//--------------- CONSTRUCTOR --------------------

Brushes::Brushes( QWidget *parent ) : KTModuleWidgetBase( parent, "Brushes" )
{
	Q_CHECK_PTR( parent );

	setCaption( tr( "Brushes" ) );

// 	move( 800, 88 );
	
	
	m_brushMaxValue = 0;
	m_numberOfBrushes = 1;

    //Icon Initializations
	i_add_brush = QPixmap( KTOON_HOME+"/themes/default/icons/plussign.xpm" );
	i_remove_brush = QPixmap( KTOON_HOME+"/themes/default/icons/minussign.xpm" );
	
	setupButtons();
	setupTableBrushes();
	setupStaticText();
	setupPrevisualizationArea();
	adjustSize();
    
}

//-------------- DESTRUCTOR -----------------

Brushes::~Brushes()
{
	delete m_addBrush;
	delete m_removeBrush;
	delete m_tableBrushes;
	delete m_displayMinThickness;
	delete m_displayMaxThickness;
	delete m_displaySmoothness;
	// 	delete m_textName; // Causes SIGSEGV
	delete m_valueName;
}



void Brushes::setupButtons()
{
    //----------- Operations on the Buttons -----------------
	QGroupBox *containerButtons = new QGroupBox(this);
	QHBoxLayout *layout = new QHBoxLayout(containerButtons);
	layout->setMargin(0);
	m_addBrush = new KTImageButton( i_add_brush, 22, containerButtons);
	
	connect( m_addBrush, SIGNAL( clicked() ), SLOT( addBrush() ) );
	QToolTip::add( m_addBrush, tr( "Add Brush" ) );
	layout->addWidget(m_addBrush, Qt::AlignCenter);
	m_removeBrush = new KTImageButton( i_remove_brush, 22, containerButtons);
	connect( m_removeBrush, SIGNAL( clicked() ), SLOT( removeBrush() ) );
	QToolTip::add( m_removeBrush, tr( "Remove Brush" ) );

	layout->addWidget(m_removeBrush, Qt::AlignCenter);
	addChild(containerButtons);
	m_addBrush->setMinimumWidth(containerButtons->width()-10);
	m_removeBrush->setMinimumWidth(containerButtons->width()-10);
}

void Brushes::setupTableBrushes()
{
	    //------------ Operations on the Table of Brushes -------------
// 	QGroupBox *containerTableBrushes = new QGroupBox(this);
	m_tableBrushes = new KTBrushesList(this);
// 	m_tableBrushes->addBrush(2,5,2, tr("Brush")+QString( "0" ));
	connect(m_tableBrushes, SIGNAL(changeCurrentBrush(int , int , int , QString )), this, SLOT(selectBrush(int , int , int , QString )));
	addChild(m_tableBrushes);
	
	//FIXME: removere este codigo pare reducir el acoplamiento con los demas modulos
// 	std::auto_ptr<Brush> ap_brush(new Brush( 2, 5, 2 ) );
// 	ap_brush.get()->setNameBrush( tr( "Brush" ) + QString( "0" ) );
// 	KTStatus -> setCurrentBrush( ap_brush.get() );
// 	QList<Brush*> br;
// 	br.append( ap_brush.get() );
// 	KTStatus->currentDocument()->setBrushes( br );
// 	ap_brush.release();
	//
}

void Brushes::setupStaticText()
{
	m_displayMinThickness = new KTEditSpinBox(2, THICKNESS_MIN_MIN, THICKNESS_MIN_MAX,1, tr("Minimum Thickness"), this,"Display Min Thickness");
	addChild(m_displayMinThickness);
	connect( m_displayMinThickness, SIGNAL( valueChanged( int ) ), SLOT( changeValueMinThickness(int) ) );
// 	connect( m_displayMinThickness, SIGNAL( valueChanged( int ) ), m_tableBrushes, SLOT(changeCurrentValueMin(int)));
	
	m_displayMaxThickness = new KTEditSpinBox(5, THICKNESS_MAX_MIN, THICKNESS_MAX_MAX,1, tr("Maximum Thickness"), this,"Display Max Thickness");
	addChild(m_displayMaxThickness);
	connect( m_displayMaxThickness, SIGNAL( valueChanged( int ) ), SLOT( changeValueMaxThickness(int) ) );
// 	connect( m_displayMaxThickness, SIGNAL( valueChanged( int ) ), m_tableBrushes, SLOT(changeCurrentValueMax(int)));
	
	m_displaySmoothness = new KTEditSpinBox(2,  SMOOTHNESS_MIN, SMOOTHNESS_MAX,1, tr("Smoothness"), this,"Display Smoothness");
	addChild(m_displaySmoothness);
	connect(m_displaySmoothness , SIGNAL( valueChanged( int ) ), m_tableBrushes, SLOT(changeCurrentValueSmooth(int)));
	connect(m_displaySmoothness , SIGNAL( valueChanged( int ) ), SLOT( changeValueSmoothness(int) ) );
	
}

void Brushes::setupPrevisualizationArea()
{
    //---------------- Operations on the  -----------------------
	QScrollArea *area = new QScrollArea(this);
	
	m_displayBrush = new KTDisplayBrush(m_displayMinThickness->value(), m_displayMaxThickness->value(),  this);
	
	area->setWidget(m_displayBrush);
	area->setWidgetResizable ( true );
	
	m_displayBrush->setMinimumSize(THICKNESS_MAX_MAX+10,THICKNESS_MAX_MAX+10);
	addChild(area);
	
	QGroupBox *box = new QGroupBox(this);
	QHBoxLayout *layout = new QHBoxLayout;
	box->setLayout(layout);
	box->setTitle(tr( "Brush Name" ));
	
	m_valueName = new QLineEdit( tr( "Brush0" ),  box);
	
	layout->addWidget(m_valueName);
	layout->setMargin(2);
	m_valueName -> setAlignment( Qt::AlignLeft );
	m_valueName -> setMaxLength( 10 );
	connect( m_valueName, SIGNAL( lostFocus() ), SLOT( changeValueName() ) );
	connect( m_valueName, SIGNAL( returnPressed() ), SLOT( changeValueName() ) );
	addChild(box);
// 	m_textName = new QLabel( tr( "Brush Name" ), this );
// 	m_textName->setAlignment( Qt::AlignTop );
// 	addChild(m_textName);
}

//--------------- PUBLIC MEMBERS ---------------

void Brushes::loadBrushes( QList<Brush*> brushes )
{
	m_brushMaxValue = 0;
	m_numberOfBrushes = 0;

//     table_brushes -> clear();

//     Brush *b_it = brushes.first();
//     Q3ListViewItem *b = new Q3ListViewItem( table_brushes, QString::number( brushes[0] -> thicknessMinBrush() ),  QString::number( brushes[0]-> thicknessMaxBrush() ), QString::number( brushes[0]-> smoothnessBrush() ), brushes[0]-> nameBrush() );
//     b_it = brushes.next();
// 	m_brushMaxValue++;
// 	m_numberOfBrushes++;
// 	for(int i =1; i < brushes.count(); i++)
// 	{
// 	/*for ( ; b_it; b_it = brushes.next() )
// 		{*/
// 		Brush * b_it = brushes[i];
// //         new Q3ListViewItem( table_brushes, table_brushes -> lastItem(),
// // 	                   QString::number( b_it -> thicknessMinBrush() ),
// //     			   QString::number( b_it -> thicknessMaxBrush() ),
// // 			   QString::number( b_it -> smoothnessBrush() ),
// // 			   b_it -> nameBrush() );
// 		m_brushMaxValue++;
// 		m_numberOfBrushes++;
// 	}
// 
// 	KTStatus->currentDocument()->setBrushes(brushes );
// //     table_brushes -> setSelected( b, true );
// 	KTStatus -> setCurrentBrush( brushes.first() );
}

//------------------- SLOTS ----------------------------

void Brushes::addBrush()
{
	
// 	m_tableBrushes->add
	QString brush_number;
	m_brushMaxValue++;
	brush_number.setNum( m_brushMaxValue );
	m_numberOfBrushes++;

//     new Q3ListViewItem( table_brushes, table_brushes -> lastItem(), "2", "5", "2", tr( "Brush" ) + brush_number );
// 	m_tableBrushes->addBrush( 2,5,2, tr( "Brush" ) + brush_number );
	
	emit(addedBrush(2,5,2, tr( "Brush" ) + brush_number));
// 	QList<Brush*> br = KTStatus->currentDocument() -> getBrushes();
    
// 	Brush* n_brush = new Brush( 2, 5, 2 );
// 	try {
// 		n_brush -> setNameBrush( tr( "Brush" ) + brush_number );
// 		br.append( n_brush );
// 		KTStatus->currentDocument() -> setBrushes( br );
// 	}
// 	catch(...)
// 	{
// 		delete n_brush;
// 		throw;
// 	}
    
// 	KTStatus->currentDrawingArea() -> modifyDocument( true );
}

void Brushes::removeBrush()
{
//    Q3ListViewItem *brush_to_remove = table_brushes -> selectedItem();
//    int /*/*brush_pos*/*/ = table_brushes -> itemPos( brush_to_remove );

	if ( m_numberOfBrushes > 1 )
	{
		if ( QMessageBox::warning( this, tr( "Confirm Brush Elimination" ), tr( "Are you sure you want to delete the selected brush?" ),
		     QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton ) == QMessageBox::Yes )
		{
			int index = m_tableBrushes->removeCurrentBrush();
			emit(removedBrush(index));
//           delete brush_to_remove;
//           table_brushes -> setSelected( table_brushes -> currentItem(), true );
//           table_brushes -> triggerUpdate();
			m_numberOfBrushes--;

// 			QList<Brush*> br = KTStatus->currentDocument() -> getBrushes();
//           br.setAutoDelete( true );
//           br.removeAt( brush_pos / 16 );
// 	  br.setAutoDelete( false );
// 			KTStatus->currentDocument() -> setBrushes( br );

//           Q3ListViewItem *selected_brush = table_brushes -> selectedItem();
//           brush_pos = table_brushes -> itemPos( selected_brush );
// 			QList<Brush*> br1 = KTStatus->currentDocument() -> getBrushes();
//           KTStatus -> setCurrentBrush( br1.at( brush_pos / 16 ) );
// 			KTStatus->currentDrawingArea() -> modifyDocument( true );
		}
	}

}


void Brushes::changeValueMinThickness(int value)
{
	m_tableBrushes->changeCurrentValueMin( value);
// 	int mint;
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
// 	Brush *s_brush = KTStatus -> currentBrush();
// 	s_brush -> setThicknessMinBrush( mint );
// 	KTStatus->currentDrawingArea() -> modifyDocument( true );
	emit minThicknessChanged(m_tableBrushes->indexCurrentBrush(), value);
}

void Brushes::changeValueMaxThickness(int value)
{
	m_tableBrushes->changeCurrentValueMax( value);
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
// 	KTStatus->currentDrawingArea() -> modifyDocument( true );
	emit maxThicknessChanged(m_tableBrushes->indexCurrentBrush(), value);
}

void Brushes::changeValueSmoothness(int value)
{
// 	int smo;
// 	Brush *s_brush = KTStatus -> currentBrush();
// 	s_brush -> setSmoothnessBrush( smo );
// 	KTStatus->currentDrawingArea() -> modifyDocument( true );

	emit smoothnessChanged(m_tableBrushes->indexCurrentBrush(), value);
#ifdef DEBUG_BRUSHES
ktDebug() << "smoothnessChanged()" << endl;
#endif
}

void Brushes::changeValueName()
{

    //If the new name is the empty string, leave the name that it had before
	if ( m_valueName -> text() == "" )
	{
		m_valueName -> setText( current_name );
	}
	else
	{
		m_tableBrushes->changeCurrentValueName( m_valueName -> text() );
// 		Brush *s_brush = KTStatus -> currentBrush();
// 		s_brush -> setNameBrush( m_valueName -> text() );
	}
// 	KTStatus->currentDrawingArea() -> modifyDocument( true );
	emit nameChanged(m_tableBrushes->indexCurrentBrush(), m_valueName -> text());
	
}


void Brushes::selectBrush(int min, int max, int smooth, QString name)
{
	
	m_displayMinThickness->setValue( min);
	m_displayMaxThickness->setValue( max);
	m_displaySmoothness->setValue(smooth);
	m_valueName->setText(name);
	emit changedBrush(m_tableBrushes->indexCurrentBrush());
//     Q3ListViewItem *selected_brush = table_brushes -> selectedItem();
//     int brush_pos = table_brushes -> itemPos( selected_brush );

    //Don't allow deselection
//     if ( selected_brush == 0 )
//     {
// //         table_brushes -> setSelected( table_brushes -> currentItem(), true );
// 	return;
//     }

//     Brush *s_brush = ( KTStatus->currentDocument() -> getBrushes() ).at( brush_pos / 16 );
//     KTStatus -> setCurrentBrush( s_brush );

//     current_name = selected_brush -> text( 3 );

    //Disconnect the signals of the slots to prevent chained callbacks
// 	disconnect( m_displayMinThickness, 0, 0, 0 );
// 	disconnect( m_displayMaxThickness, 0, 0, 0 );
// 	disconnect( m_displaySmoothness,   0, 0, 0 );
// 	disconnect( m_valueName, 0, 0, 0 );

//     m_displayMinThickness->setValue( (selected_brush -> text( 0 )).toInt());
//     m_displayMaxThickness->setValue( (selected_brush -> text( 1 )).toInt());
//     m_displaySmoothness-> setValue( ( selected_brush -> text( 2 ) ).toInt() );
    //FIXME: hacer esto con el displayBrush
//     m_circleMinThickness -> setSize( ( selected_brush -> text( 0 ) ).toInt(), ( selected_brush -> text( 0 ) ).toInt() );
//     m_circleMaxThickness -> setSize( ( selected_brush -> text( 1 ) ).toInt(), ( selected_brush -> text( 1 ) ).toInt() );
//     m_previsualization -> update();

    //Connect again the signals to the slots
// 	connect( m_valueName, SIGNAL( lostFocus() ), SLOT( changeValueName() ) );
// 	connect( m_valueName, SIGNAL( returnPressed() ), SLOT( changeValueName() ) );
// 	connect( m_displayMinThickness, SIGNAL( valueChanged( int ) ), SLOT( changeValueMinThickness() ) );
// 	connect( m_displayMaxThickness, SIGNAL( valueChanged( int ) ), SLOT( changeValueMaxThickness() ) );
// 	connect( m_displaySmoothness  , SIGNAL( valueChanged( int ) ), SLOT( changeValueSmoothness()   ) );

}


