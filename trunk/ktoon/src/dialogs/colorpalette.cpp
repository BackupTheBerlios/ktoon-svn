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
#include <qstringlist.h>

#include "colorpalette.h"
#include "images.h"
#include "tools.h"
#include "status.h"

#include <new>


//--------------- CONSTRUCTOR --------------------

ColorPalette::ColorPalette( QWidget *parent)//, WFlags style, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button )
	: KTDialogBase(QDockWindow::OutsideDock, parent, tr( "Color Palette" ))
{
	Q_CHECK_PTR( parent );
// 	Q_CHECK_PTR( in_assigned_menu );
// 	Q_CHECK_PTR( assig_tb_button );

	setHorizontallyStretchable(true);
			
	//Initializations
	
	setCaption( tr( "Color Palette" ) );
	//     setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
// 	resize( 185, 640 );
// 	setMinimumSize( 185, 640 );
// 	setMaximumSize( 185, 640 );
	move( 200, 35 );
	//     setFont( QFont( "helvetica", 8 ) );
// 	assigned_menu = in_assigned_menu;
// 	assigned_item = id_assigned_item;
// 	assigned_tb_button = assig_tb_button;
	new_outline_color = NULL;
	new_fill_color = NULL;
	
	//Icon initializations
	i_add_color = QPixmap( plussign_xpm );
	i_remove_color = QPixmap( minussign_xpm );

	setOrientation (Qt::Vertical );
	setupBlockStandardPalette();
    
	setupBlockColorMixer();
   
	setupBlockGradient();
 
	setupBlockCustomPalette();
    
    

    //-------------- Status Update -----------------

	makeConnections();
    

	KTColor *o_color = new KTColor( 0.0, 0.0, 0.0, 1.0 );
	KTColor *f_color = new(std::nothrow) KTColor( 1.0, 1.0, 1.0, 1.0 );
	if(!f_color)
	{
		delete o_color;
		throw std::bad_alloc();
	}
    
	
    KTStatus -> setCurrentOutlineColor( o_color );
    KTStatus -> setCurrentFillColor( f_color );

    adjustSize();
}

//-------------- DESTRUCTOR -----------------

ColorPalette::~ColorPalette()
{
    delete add_color;
    delete remove_color;
    delete color_cells;
    delete color_mixer;
    delete color_display;
    delete value_selector;
    delete outline_color;
    delete fill_color;
    delete gradient;
    delete grad_viewer;
    delete custom_palette;
    delete gradient_types;
    delete text_std_palette;
    delete text_gradient;
    delete text_custom_palette;
    delete text_color_name;
    delete text_color_mixer;
    delete value_color_name;
    delete value_rgb;
    delete value_r;
    delete value_g;
    delete value_b;
    delete value_h;
    delete value_s;
    delete value_v;
    delete value_alpha;
}

void ColorPalette::setupBlockStandardPalette()
{
	//------------ Standard Palette Block --------------
	QVBox *blockStandardPalette = new QVBox(this);
	text_std_palette = new QLabel( tr( "Standard Palette" ), blockStandardPalette );
	
	color_cells = new ColorCells( blockStandardPalette );
	addChild(blockStandardPalette);
	color_cells->setVScrollBarMode(QScrollView::AlwaysOff);
	color_cells->setHScrollBarMode(QScrollView::AlwaysOff);
}

void ColorPalette::setupBlockColorMixer()
{
	 //------------ Color Mixer Block --------------
	text_color_mixer = new QLabel( tr( "Color Mixer" ), this );
	addChild(text_color_mixer);
	
	QHBox *lineOrfill = new QHBox(this);
	
	outline_color = new OutlineColor( lineOrfill );
	outline_color->setActive( true );
	QToolTip::add( outline_color, tr( "Stroke Color" ) );

	fill_color = new FillColor( lineOrfill);
	QToolTip::add( fill_color, tr( "Fill Color" ) );
	addChild(lineOrfill);
	QHBox *colorMixerBlock = new QHBox(this);
	addChild(colorMixerBlock);
	QVBox *textRGBA = new QVBox(colorMixerBlock);
	QString rgba("RGBA");
	
	for(int i  = 0; i < 4; i++)
	{
		/*QLabel *tmpLabel = */new QLabel( QString(rgba[i]) + ":",  textRGBA);
	}
	QVBox *spinsRGBA = new QVBox(colorMixerBlock);
	value_r = new QSpinBox( 0, 255, 1, spinsRGBA );
	
	value_g = new QSpinBox( 0, 255, 1, spinsRGBA );
	
	value_b = new QSpinBox( 0, 255, 1, spinsRGBA );

	value_alpha = new QSpinBox( 0, 100, 1, spinsRGBA);
	value_alpha -> setValue( 100 );
	
	QVBox *textHSV= new QVBox(colorMixerBlock);
	QString hsv("HSV");
	for(int i  = 0; i < 3; i++)
	{
		/*QLabel *tmpLabels = */new QLabel( QString(hsv[i]) + ":", textHSV );
		
	}
	
	QVBox *spinsHSV= new QVBox(colorMixerBlock);
	value_h = new QSpinBox( 0, 360, 1, spinsHSV );

	value_s = new QSpinBox( 0, 255, 1, spinsHSV );

	value_v = new QSpinBox( 0, 255, 1, spinsHSV );

	QHBox *tmpBox = new QHBox(this);
	tmpBox->setSpacing(3);
	QVBox *displayColor = new QVBox(tmpBox);
	
	color_display = new ColorDisplay( displayColor );
	
	color_display->setSizePolicy(QSizePolicy::Expanding  ,QSizePolicy::Expanding , false);
	
	value_rgb = new QLineEdit( "#000000", displayColor );
	//FIXME: kuadrosx: hacer una expresion regular para validar la entrada de colores.
	value_rgb->setMaxLength(7);
	value_rgb ->setMaximumSize( color_display -> width()+20, 20 );
	color_mixer = new ColorMixer( tmpBox );
	value_selector = new ValueSelector( tmpBox );
	addChild(tmpBox);
}

void ColorPalette::setupBlockGradient()
{
	//------------ Gradient Block --------------
	QHBox *blockGradient = new QHBox(this);
	
	text_gradient = new QLabel( tr( "Gradient:" ), blockGradient );

	QStringList str_list;
	str_list << tr( "None" ) << tr( "Linear" ) << tr( "Radial" );
	gradient_types = new QComboBox( blockGradient );
	gradient_types -> insertStringList( str_list );
	gradient_types -> resize( 60, 20 );
	addChild(blockGradient);
	
	QHBox *blockgGradViewer = new QHBox(this);
	
	grad_viewer = new GradientViewer( blockgGradViewer );
	grad_viewer -> hide();
	
	gradient = new ColorGradientSelector( blockgGradViewer );
	gradient -> hide();
	
	addChild(blockgGradViewer);

}


void ColorPalette::setupBlockCustomPalette()
{
	//------------ Custom Palette Block ------------------
	QVBox *customPaletteV = new QVBox(this);
	text_custom_palette = new QLabel( tr( "Custom Palette" ),  customPaletteV);

	custom_palette = new CustomPalette( customPaletteV );

	QHBox *customPaletteButtons = new QHBox(customPaletteV);
	
	add_color = new QPushButton( i_add_color, tr( "" ), customPaletteButtons );
	add_color -> setAutoDefault( false );
	add_color -> setFlat( true );
	QToolTip::add( add_color, tr( "Add Color" ) );
	
	
	remove_color = new QPushButton( i_remove_color, tr( "" ), customPaletteButtons );
	remove_color -> setAutoDefault( false );
	remove_color -> setFlat( true );
	QToolTip::add( remove_color, tr( "Remove Color" ) );

	QHBox *customPaletteH = new QHBox(customPaletteV);
	text_color_name = new QLabel( tr( "Color Name:" ), customPaletteH );

	value_color_name = new QLineEdit( "", customPaletteH );
	value_color_name -> setFont( QFont( font().family(), 7 ));
	value_color_name->adjustSize();
	addChild(customPaletteV);
}
//-------------- PUBLIC MEMBERS -------------------
void ColorPalette::enableCustomPalette( bool state )
{
    if ( state )
    {
        //setMinimumSize( 185, 647 );
        //setMaximumSize( 185, 647 );
        //resize( 185, 647 );
	    adjustSize();
	custom_palette -> show();
	add_color -> show();
	remove_color -> show();
	text_custom_palette -> show();
	text_color_name -> show();
	value_color_name -> show();
	adjustSize();
    }
    else
    {
        //setMinimumSize( 185, 647 - custom_palette -> height() - add_color -> height() - text_custom_palette -> height() - 5 );
        //setMaximumSize( 185, 647 - custom_palette -> height() - add_color -> height() - text_custom_palette -> height() - 5 );
        //resize( 185, 647 - custom_palette -> height() - add_color -> height() - text_custom_palette -> height() - 5 );
	custom_palette -> hide();
	add_color -> hide();
	remove_color -> hide();
	text_custom_palette -> hide();
	text_color_name -> hide();
	value_color_name -> hide();
	adjustSize();
    }
}

void ColorPalette::loadCustomColors( QPtrList<KTColor> colors )
{
    custom_palette -> loadColors( colors );
}

void ColorPalette::clearCustomPalette()
{
    custom_palette -> clear();
}

//------------------- SLOTS -------------------------

void ColorPalette::slotAddToCustomColors()
{
    custom_palette -> slotAddColor( color_display -> currentColor(), value_alpha -> value() );
    value_color_name -> setText( "" );
    //k_toon -> drawingArea() -> modifyDocument( true );
    emit documentModified(true);
    emit documentModified(true);
}

void ColorPalette::slotRemoveFromCustomColors()
{
    custom_palette -> slotRemoveColor();
    emit documentModified(true);
    emit documentModified(true);
}

void ColorPalette::slotSetColor( const QColor &new_color )
{
    Q_ASSERT( new_color.isValid() );
    disconnectMembers();
    connect( gradient, SIGNAL( gradientChanged( const QColor &, const QColor &, int, int, int, int ) ), grad_viewer, SLOT( slotUpdateGradient( const QColor &, const QColor &, int, int, int, int ) ) );
    connect( value_v, SIGNAL( valueChanged( int ) ), value_selector, SLOT( slotSetValue( int ) ) );

    if ( outline_color -> isActive() )
    {
    	if ( new_outline_color != NULL )
           delete new_outline_color;
        new_outline_color = new KTColor( new_color.red() / 255.0, new_color.green() / 255.0, new_color.blue() / 255.0, value_alpha -> value() / 100.0 );
        outline_color -> slotSetColor( new_color );
        KTStatus -> setCurrentOutlineColor( new_outline_color );
	emit outlineColorChanged();
    }
    else
    {
    	if ( new_fill_color != NULL )
           delete new_fill_color;
    	new_fill_color = new KTColor( new_color.red() / 255.0, new_color.green() / 255.0, new_color.blue() / 255.0, value_alpha -> value() / 100.0 );
        fill_color -> slotSetColor( new_color );
        KTStatus -> setCurrentFillColor( new_fill_color );
	emit fillColorChanged();
    }

    int old_value = value_selector -> value();

    color_display -> slotSetColor( new_color );
    value_rgb -> setText( new_color.name() );
    value_selector -> setColor( new_color );

    GradientSwitch *c_switch = gradient -> currentSwitch();
    if ( c_switch != NULL && gradient_types -> currentText() != tr( "None" ) )
    {
	c_switch -> slotSetColor( new_color );
	gradient -> update();
    }

    value_r -> setValue( new_color.red() );
    value_g -> setValue( new_color.green() );
    value_b -> setValue( new_color.blue() );
    int n_h, n_s, n_v;
    new_color.getHsv( &n_h, &n_s, &n_v );
    value_h -> setValue( n_h );
    value_s -> setValue( n_s );
    value_v -> setValue( n_v );

    color_mixer -> updateCross( value_s -> value(), value_h -> value() );

    disconnect( add_color, 0, 0, 0 );
    disconnect( remove_color, 0, 0, 0 );
    makeConnections();
    bool test = false;
    if ( ( ColorMixer * )sender() == color_mixer )
    {
        value_selector -> slotSetValue( old_value );
        value_selector -> setColor( new_color );
	test = true;
    }
    /*
     * The purpose of this code block is only to optimize the color selection, avoiding innecessary signal emitions
     * and slot calls
     */
    if ( test )
    {
        disconnectMembers();
	value_v -> setValue( old_value );
        makeConnections();
    }
}

void ColorPalette::slotSetColor( KTColor *new_color )
{
    Q_CHECK_PTR( new_color );
    QColor the_color = QColor( ( int )( new_color -> colorRed() * 255.0 ), ( int )( new_color -> colorGreen() * 255.0 ), ( int )( new_color -> colorBlue() * 255.0 ) );
    int the_alpha = ( int )( new_color -> colorAlpha() * 100.0 );
    slotSetColor( the_color );
    value_alpha -> setValue( the_alpha );

    if ( outline_color -> isActive() )
       KTStatus -> setCurrentCursor( Tools::CONTOUR_FILL );
    else
       KTStatus -> setCurrentCursor( Tools::FILL );

//     k_toon -> slotActivateCursor(); FIXME:
}

void ColorPalette::slotActivateOutlineColor()
{
    outline_color -> setActive( true );
    fill_color -> setActive( false );

    QColor outline = outline_color -> currentColor();

    disconnectMembers();

    value_r -> setValue( outline.red() );
    value_g -> setValue( outline.green() );
    value_b -> setValue( outline.blue() );
    value_alpha -> setValue( outline_color -> getAlpha() );
    int n_h, n_s, n_v;
    outline.getHsv( &n_h, &n_s, &n_v );
    value_h -> setValue( n_h );
    value_s -> setValue( n_s );
    value_v -> setValue( n_v );
    color_display -> slotSetColor( outline );
    value_rgb -> setText( outline.name() );

    disconnect( add_color, 0, 0, 0 );
    disconnect( remove_color, 0, 0, 0 );
    makeConnections();
}

void ColorPalette::slotActivateFillColor()
{
    outline_color -> setActive( false );
    fill_color -> setActive( true );

    QColor fill = fill_color -> currentColor();

    disconnectMembers();

    value_r -> setValue( fill.red() );
    value_g -> setValue( fill.green() );
    value_b -> setValue( fill.blue() );
    value_alpha -> setValue( fill_color -> getAlpha() );
    int n_h, n_s, n_v;
    fill.getHsv( &n_h, &n_s, &n_v );
    value_h -> setValue( n_h );
    value_s -> setValue( n_s );
    value_v -> setValue( n_v );
    color_display -> slotSetColor( fill );
    value_rgb -> setText( fill.name() );

    disconnect( add_color, 0, 0, 0 );
    disconnect( remove_color, 0, 0, 0 );
    makeConnections();
}

void ColorPalette::slotSyncRGB()
{
    QColor new_color;
    new_color.setHsv( value_h -> value(), value_s -> value(), value_v -> value() );
    value_rgb -> setText( new_color.name() );
    value_selector -> slotSetValue( value_v -> value() );

    KTColor *n_color = new KTColor( new_color.red() / 255.0, new_color.green() / 255.0, new_color.blue() / 255.0, value_alpha -> value() / 100.0 );
    if ( outline_color -> isActive() )
    {
        KTStatus -> setCurrentOutlineColor( n_color );
	emit outlineColorChanged();
    }
    else
    {
        KTStatus -> setCurrentFillColor( n_color );
	emit fillColorChanged();
    }

    GradientSwitch *c_switch = gradient -> currentSwitch();
    if ( c_switch != NULL && gradient_types -> currentText() != tr( "None" ) )
    {
	c_switch -> slotSetColor( new_color );
	gradient -> update();
    }

    disconnect( value_r, 0, 0, 0 );
    disconnect( value_g, 0, 0, 0 );
    disconnect( value_b, 0, 0, 0 );
    disconnect( value_h, 0, 0, 0 );
    disconnect( value_s, 0, 0, 0 );
    disconnect( value_v, 0, 0, 0 );

    value_r -> setValue( new_color.red() );
    value_g -> setValue( new_color.green() );
    value_b -> setValue( new_color.blue() );
    value_selector -> slotSetValue( value_v -> value() );

    disconnect( add_color, 0, 0, 0 );
    disconnect( remove_color, 0, 0, 0 );
    makeConnections();
}

void ColorPalette::slotSyncHSV()
{
    QColor new_color;
    new_color.setRgb( value_r -> value(), value_g -> value(), value_b -> value() );
    value_rgb -> setText( new_color.name() );

    KTColor *n_color = new KTColor( new_color.red() / 255.0, new_color.green() / 255.0, new_color.blue() / 255.0, value_alpha -> value() / 100.0 );
    if ( outline_color -> isActive() )
    {
        KTStatus -> setCurrentOutlineColor( n_color );
	emit outlineColorChanged();
    }
    else
    {
        KTStatus -> setCurrentFillColor( n_color );
	emit fillColorChanged();
    }

    GradientSwitch *c_switch = gradient -> currentSwitch();
    if ( c_switch != NULL && gradient_types -> currentText() != tr( "None" ) )
    {
	c_switch -> slotSetColor( new_color );
	gradient -> update();
    }

    disconnect( value_r, 0, 0, 0 );
    disconnect( value_g, 0, 0, 0 );
    disconnect( value_b, 0, 0, 0 );
    disconnect( value_h, 0, 0, 0 );
    disconnect( value_s, 0, 0, 0 );
    disconnect( value_v, 0, 0, 0 );

    int h, s, v;
    new_color.getHsv( &h, &s, &v );
    value_h -> setValue( h );
    value_s -> setValue( s );
    value_v -> setValue( v );
    value_selector -> slotSetValue( value_v -> value() );

    disconnect( add_color, 0, 0, 0 );
    disconnect( remove_color, 0, 0, 0 );
    makeConnections();
}

void ColorPalette::slotActivateGradientType( const QString &value )
{
    if ( value == tr( "Linear" ) )
    {
	gradient -> show();
	grad_viewer -> show();
	grad_viewer -> slotSetType( GradientViewer::LINEAR );
	adjustSize();
    }
    else if ( value == tr( "Radial" ) )
    {
	gradient -> show();
	grad_viewer -> show();
	grad_viewer -> slotSetType( GradientViewer::RADIAL );
	adjustSize();
    }
    else
    {
	gradient -> hide();
	grad_viewer -> hide();
	adjustSize();
    }
}

void ColorPalette::slotChangeRGB( const QString &rgb )
{
    QColor intermediate;
    intermediate.setNamedColor( rgb );
    disconnect( value_rgb, 0, this, 0 );
    if ( intermediate.isValid() )
    {
        KTColor *n_color = new KTColor( intermediate.red() / 255.0, intermediate.green() / 255.0, intermediate.blue() / 255.0, value_alpha -> value() / 100.0 );
    	if ( outline_color -> isActive() )
    	{
            KTStatus -> setCurrentOutlineColor( n_color );
	    emit outlineColorChanged();
    	}
    	else
    	{
            KTStatus -> setCurrentFillColor( n_color );
	    emit fillColorChanged();
        }

        color_display -> slotSetColor( intermediate );
        value_selector -> setColor( intermediate );

        GradientSwitch *c_switch = gradient -> currentSwitch();
        if ( c_switch != NULL && gradient_types -> currentText() != "None" )
        {
	    c_switch -> slotSetColor( intermediate );
	    gradient -> update();
    	}

        value_r -> setValue( intermediate.red() );
        value_g -> setValue( intermediate.green() );
        value_b -> setValue( intermediate.blue() );
	value_selector -> slotSetValue( value_v -> value() );
        color_mixer -> updateCross( value_s -> value(), value_h -> value() );
    }
    connect( value_rgb, SIGNAL( textChanged( const QString & ) ), SLOT( slotChangeRGB( const QString & ) ) );
}

void ColorPalette::slotUpdateCurrentAlpha( int new_alpha )
{
    if ( outline_color -> isActive() )
    {
        KTColor *o_color = KTStatus -> currentOutlineColor();
	o_color -> setColorAlpha( new_alpha / 100.0 );
        KTStatus -> setCurrentOutlineColor( o_color );
	emit outlineColorChanged();
    }
    else
    {
        KTColor *f_color = KTStatus -> currentFillColor();
	f_color -> setColorAlpha( new_alpha / 100.0 );
        KTStatus -> setCurrentFillColor( f_color );
	emit fillColorChanged();
    }

    GradientSwitch *c_switch = gradient -> currentSwitch();
    if ( c_switch != NULL && gradient_types -> currentText() != tr( "None" ) )
    {
	c_switch -> slotSetAlpha( new_alpha );
	gradient -> update();
    }
}

//--------------------- EVENTS AND OTHER FUNCTIONS --------------------------------

// void ColorPalette::closeEvent( QCloseEvent* close_event )
// {
//     Q_ASSERT( close_event );
//     assigned_menu -> setItemChecked( assigned_item, false );
//     assigned_tb_button -> setDown( false );
//     close_event -> accept();
// }

void ColorPalette::makeConnections()
{
	// TODO: Utilizar QSignalMapper
    connect( color_cells, SIGNAL( colorSelected( const QColor & ) ), SLOT( slotSetColor( const QColor & ) ) );
    connect( outline_color, SIGNAL( activated() ), SLOT( slotActivateOutlineColor() ) );
    connect( fill_color, SIGNAL( activated() ), SLOT( slotActivateFillColor() ) );
    connect( value_rgb, SIGNAL( textChanged( const QString & ) ), SLOT( slotChangeRGB( const QString & ) ) );
    connect( color_mixer, SIGNAL( colorSelected( const QColor & ) ), SLOT( slotSetColor( const QColor & ) ) );
    connect( value_selector, SIGNAL( valueChanged( int ) ), value_v, SLOT( setValue( int ) ) );
    connect( value_v, SIGNAL( valueChanged( int ) ), value_selector, SLOT( slotSetValue( int ) ) );
    connect( gradient_types, SIGNAL( activated( const QString & ) ), SLOT( slotActivateGradientType( const QString & ) ) );
    connect( custom_palette, SIGNAL( colorSelected( const QColor & ) ), SLOT( slotSetColor( const QColor & ) ) );
    connect( add_color, SIGNAL( clicked() ), SLOT( slotAddToCustomColors() ) );
    connect( remove_color, SIGNAL( clicked() ), SLOT( slotRemoveFromCustomColors() ) );
    connect( value_color_name, SIGNAL( textChanged( const QString & ) ), custom_palette, SLOT( slotChangeColorName( const QString & ) ) );

    connect( value_r, SIGNAL( valueChanged( int ) ), color_display, SLOT( slotChangeR( int ) ) );
    connect( value_g, SIGNAL( valueChanged( int ) ), color_display, SLOT( slotChangeG( int ) ) );
    connect( value_b, SIGNAL( valueChanged( int ) ), color_display, SLOT( slotChangeB( int ) ) );
    connect( value_alpha, SIGNAL( valueChanged( int ) ), color_display, SLOT( slotChangeA( int ) ) );
    connect( value_h, SIGNAL( valueChanged( int ) ), color_display, SLOT( slotChangeH( int ) ) );
    connect( value_s, SIGNAL( valueChanged( int ) ), color_display, SLOT( slotChangeS( int ) ) );
    connect( value_v, SIGNAL( valueChanged( int ) ), color_display, SLOT( slotChangeV( int ) ) );

    connect( value_r, SIGNAL( valueChanged( int ) ), SLOT( slotSyncHSV() ) );
    connect( value_g, SIGNAL( valueChanged( int ) ), SLOT( slotSyncHSV() ) );
    connect( value_b, SIGNAL( valueChanged( int ) ), SLOT( slotSyncHSV() ) );
    connect( value_h, SIGNAL( valueChanged( int ) ), SLOT( slotSyncRGB() ) );
    connect( value_s, SIGNAL( valueChanged( int ) ), SLOT( slotSyncRGB() ) );
    connect( value_v, SIGNAL( valueChanged( int ) ), SLOT( slotSyncRGB() ) );

    connect( color_display, SIGNAL( colorChanged( const QColor & ) ), outline_color, SLOT( slotSetColor( const QColor & ) ) );
    connect( value_alpha, SIGNAL( valueChanged( int ) ), outline_color, SLOT( slotChangeAlpha( int ) ) );
    connect( color_display, SIGNAL( colorChanged( const QColor & ) ), fill_color, SLOT( slotSetColor( const QColor & ) ) );
    connect( value_alpha, SIGNAL( valueChanged( int ) ), fill_color, SLOT( slotChangeAlpha( int ) ) );

    connect( value_alpha, SIGNAL( valueChanged( int ) ), SLOT( slotUpdateCurrentAlpha( int ) ) );

    connect( gradient, SIGNAL( switchSelected( const QColor & ) ), SLOT( slotSetColor( const QColor & ) ) );
    connect( gradient, SIGNAL( switchSelected( int ) ), value_alpha, SLOT( setValue( int ) ) );
    connect( gradient, SIGNAL( gradientChanged( const QColor &, const QColor &, int, int, int, int ) ), grad_viewer, SLOT( slotUpdateGradient( const QColor &, const QColor &, int, int, int, int ) ) );

    connect( custom_palette, SIGNAL( colorNameChanged( const QString & ) ), value_color_name, SLOT( setText( const QString & ) ) );
    connect( custom_palette, SIGNAL( colorAlphaChanged( int ) ), value_alpha, SLOT( setValue( int ) ) );
}

void ColorPalette::disconnectMembers()
{
    disconnect( color_cells, 0, 0, 0 );
    disconnect( color_mixer, 0, 0, 0 );
    disconnect( color_display, 0, 0, 0 );
    disconnect( outline_color, 0, 0, 0 );
    disconnect( fill_color, 0, 0, 0 );
    disconnect( value_rgb, 0, 0, 0 );
    disconnect( value_r, 0, 0, 0 );
    disconnect( value_g, 0, 0, 0 );
    disconnect( value_b, 0, 0, 0 );
    disconnect( value_h, 0, 0, 0 );
    disconnect( value_s, 0, 0, 0 );
    disconnect( value_v, 0, 0, 0 );
    disconnect( value_alpha, 0, 0, 0 );
    disconnect( value_color_name, 0, 0, 0 );
    disconnect( value_selector, 0, 0, 0 );
    disconnect( grad_viewer, 0, 0, 0 );
    disconnect( gradient, 0, 0, 0 );
    disconnect( gradient_types, 0, 0, 0 );
    disconnect( custom_palette, 0, 0, 0 );
    disconnect( add_color, 0, 0, 0 );
    disconnect( remove_color, 0, 0, 0 );
}

