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
#include "ktoon.h"
#include "images.h"

//--------------- CONSTRUCTOR --------------------

ColorPalette::ColorPalette( QWidget *parent, WFlags style, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button )
    : QDialog( parent, tr( "Color Palette" ), false, style )
{
    //Initializations
    setCaption( tr( "Color Palette" ) );
    setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    resize( 185, 647 );
    setMinimumSize( 185, 647 );
    setMaximumSize( 185, 647 );
    move( 200, 35 );
    setFont( QFont( "helvetica", 8 ) );
    parent_widget = parent;
    assigned_menu = in_assigned_menu;
    assigned_item = id_assigned_item;
    assigned_tb_button = assig_tb_button;
    k_toon = ( Ktoon * )parent_widget;

    //Icon initializations
    i_add_color = QPixmap( plussign_xpm );
    i_remove_color = QPixmap( minussign_xpm );

    //------------ Standard Palette Block --------------

    text_std_palette = new QLabel( tr( "Standard Palette" ), this );
    text_std_palette -> move( 5, 5 );
    text_std_palette -> resize( 170, text_std_palette -> height() );

    color_cells = new ColorCells( this );
    color_cells -> resize( 175, 113 );
    color_cells -> move( 5, text_std_palette -> y() + text_std_palette -> height() - 3 );

    separator1 = new QFrame( this );
    separator1 -> resize( width(), 2 );
    separator1 -> move( 0, color_cells -> y() + color_cells -> height() + 5 );
    separator1 -> setFrameStyle( QFrame::Panel | QFrame::Sunken );

    //------------ Color Mixer Block --------------

    text_color_mixer = new QLabel( tr( "Color Mixer" ), this );
    text_color_mixer -> move( 5, separator1 -> y() + separator1 -> height() );
    text_color_mixer -> resize( 170, text_color_mixer -> height() );

    outline_color = new OutlineColor( this );
    outline_color -> move( 5, text_color_mixer -> y() + text_color_mixer -> height() );
    outline_color -> setActive( true );
    QToolTip::add( outline_color, tr( "Stroke Color" ) );

    fill_color = new FillColor( this );
    fill_color -> move( 5, outline_color -> y() + outline_color -> height() + 5 );
    QToolTip::add( fill_color, tr( "Fill Color" ) );

    text_r = new QLabel( "R:", this );
    text_r -> resize( 10, 20 );
    text_r -> move( outline_color -> x() + outline_color -> width() + 10, outline_color -> y() );

    text_g = new QLabel( "G:", this );
    text_g -> resize( 10, 20 );
    text_g -> move( text_r -> x(), text_r -> y() + text_r -> height() + 5 );

    text_b = new QLabel( "B:", this );
    text_b -> resize( 10, 20 );
    text_b -> move( text_g -> x(), text_g -> y() + text_g -> height() + 5 );

    text_alpha = new QLabel( "A:", this );
    text_alpha -> resize( 10, 20 );
    text_alpha -> move( text_b -> x(), text_b -> y() + text_b -> height() + 5 );

    value_r = new QSpinBox( 0, 255, 1, this );
    value_r -> resize( 40, 20 );
    value_r -> move( text_r -> x() + text_r -> width() + 5, text_r -> y() );

    value_g = new QSpinBox( 0, 255, 1, this );
    value_g -> resize( 40, 20 );
    value_g -> move( text_g -> x() + text_g -> width() + 5, text_g -> y() );

    value_b = new QSpinBox( 0, 255, 1, this );
    value_b -> resize( 40, 20 );
    value_b -> move( text_b -> x() + text_b -> width() + 5, text_b -> y() );

    value_alpha = new QSpinBox( 0, 100, 1, this );
    value_alpha -> setValue( 100 );
    value_alpha -> resize( 40, 20 );
    value_alpha -> move( text_alpha -> x() + text_alpha -> width() + 5, text_alpha -> y() );

    text_h = new QLabel( "H:", this );
    text_h -> resize( 10, 20 );
    text_h -> move( value_r -> x() + value_r -> width() + 10, value_r -> y() );

    text_s = new QLabel( "S:", this );
    text_s -> resize( 10, 20 );
    text_s -> move( text_h -> x(), text_h -> y() + text_h -> height() + 5 );

    text_v = new QLabel( "V:", this );
    text_v -> resize( 10, 20 );
    text_v -> move( text_s -> x(), text_s -> y() + text_s -> height() + 5 );

    value_h = new QSpinBox( 0, 360, 1, this );
    value_h -> resize( 40, 20 );
    value_h -> move( text_h -> x() + text_h -> width() + 5, text_h -> y() );

    value_s = new QSpinBox( 0, 255, 1, this );
    value_s -> resize( 40, 20 );
    value_s -> move( text_s -> x() + text_s -> width() + 5, text_s -> y() );

    value_v = new QSpinBox( 0, 255, 1, this );
    value_v -> resize( 40, 20 );
    value_v -> move( text_v -> x() + text_v -> width() + 5, text_v -> y() );

    color_display = new ColorDisplay( this );
    color_display -> resize( 47, 47 );
    color_display -> move( 5, text_alpha -> y() + text_alpha -> height() + 5 );

    value_rgb = new QLineEdit( "#000000", this );
    value_rgb -> resize( color_display -> width(), 20 );
    value_rgb -> move( color_display -> x(), color_display -> y() + color_display -> height() + 5 );

    color_mixer = new ColorMixer( this );
    color_mixer -> move( color_display -> x() + color_display -> width() + 5, color_display -> y() );

    value_selector = new ValueSelector( this );
    value_selector -> move( color_mixer -> x() + color_mixer -> width() + 5, color_mixer -> y() - 3 );

    separator2 = new QFrame( this );
    separator2 -> resize( width(), 2 );
    separator2 -> move( 0, color_mixer -> y() + color_mixer -> height() + 5 );
    separator2 -> setFrameStyle( QFrame::Panel | QFrame::Sunken );

    //------------ Gradient Block --------------

    text_gradient = new QLabel( tr( "Gradient:" ), this );
    text_gradient -> resize( 50, 20 );
    text_gradient -> move( 5, separator2 -> y() + separator2 -> height() + 10 );

    QStringList str_list;
    str_list << tr( "None" ) << tr( "Linear" ) << tr( "Radial" );
    gradient_types = new QComboBox( this );
    gradient_types -> insertStringList( str_list );
    gradient_types -> resize( 60, 20 );
    gradient_types -> move( text_gradient -> x() + text_gradient -> width() + 5, text_gradient -> y() );

    grad_viewer = new GradientViewer( this );
    grad_viewer -> move( gradient_types -> x() + gradient_types -> width() + 10, gradient_types -> y() );
    grad_viewer -> hide();

    gradient = new ColorGradientSelector( this );
    gradient -> move( 5, grad_viewer -> y() + grad_viewer -> height() + 10 );
    gradient -> hide();

    separator3 = new QFrame( this );
    separator3 -> resize( width(), 2 );
    separator3 -> move( 0, gradient -> y() + gradient -> height() + 10 );
    separator3 -> setFrameStyle( QFrame::Panel | QFrame::Sunken );

    //------------ Custom Palette Block ------------------

    text_custom_palette = new QLabel( tr( "Custom Palette" ), this );
    text_custom_palette -> move( 5, separator3 -> y() + separator3 -> height() );
    text_custom_palette -> resize( 170, text_custom_palette -> height() );

    custom_palette = new CustomPalette( this );
    custom_palette -> resize( 175, 113 );
    custom_palette -> move( 5, text_custom_palette -> y() + text_custom_palette -> height() - 3 );

    add_color = new QPushButton( i_add_color, tr( "" ), this );
    add_color -> resize( 20, 20 );
    add_color -> move( 5, custom_palette -> y() + custom_palette -> height() + 5 );
    add_color -> setAutoDefault( false );
    add_color -> setFlat( true );
    QToolTip::add( add_color, tr( "Add Color" ) );

    remove_color = new QPushButton( i_remove_color, tr( "" ), this );
    remove_color -> resize( 20, 20 );
    remove_color -> move( add_color -> x() + add_color -> width() + 5, add_color -> y() );
    remove_color -> setAutoDefault( false );
    remove_color -> setFlat( true );
    QToolTip::add( remove_color, tr( "Remove Color" ) );

    text_color_name = new QLabel( tr( "Color Name:" ), this );
    text_color_name -> setFont( QFont( "helvetica", 7 ) );
    text_color_name -> resize( 60, 15 );
    text_color_name -> move( remove_color -> x() + remove_color -> width() + 5, remove_color -> y() );

    value_color_name = new QLineEdit( "", this );
    value_color_name -> setFont( QFont( "helvetica", 7 ) );
    value_color_name -> resize( 60, 15 );
    value_color_name -> move( text_color_name -> x() + text_color_name -> width() + 5, text_color_name -> y() );

    //-------------- Status Update -----------------

    makeConnections();

    Color *o_color = new Color( 0.0, 0.0, 0.0, 1.0 );
    Color *f_color = new Color( 1.0, 1.0, 1.0, 1.0 );
    k_toon -> currentStatus() -> setCurrentOutlineColor( o_color );
    k_toon -> currentStatus() -> setCurrentFillColor( f_color );

}

//-------------- DESTRUCTOR -----------------

ColorPalette::~ColorPalette()
{

}

//-------------- PUBLIC MEMBERS -------------------

void ColorPalette::enableCustomPalette( bool state )
{
    if ( state )
    {
        setMinimumSize( 185, 647 );
        setMaximumSize( 185, 647 );
        resize( 185, 647 );
	custom_palette -> show();
	add_color -> show();
	remove_color -> show();
	text_custom_palette -> show();
	text_color_name -> show();
	value_color_name -> show();
	separator3 -> show();
    }
    else
    {
        setMinimumSize( 185, 647 - custom_palette -> height() - add_color -> height() - text_custom_palette -> height() - 5 );
        setMaximumSize( 185, 647 - custom_palette -> height() - add_color -> height() - text_custom_palette -> height() - 5 );
        resize( 185, 647 - custom_palette -> height() - add_color -> height() - text_custom_palette -> height() - 5 );
	custom_palette -> hide();
	add_color -> hide();
	remove_color -> hide();
	text_custom_palette -> hide();
	text_color_name -> hide();
	value_color_name -> hide();
	separator3 -> hide();
    }
}

void ColorPalette::loadCustomColors( QPtrList<Color> colors )
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
    k_toon -> drawingArea() -> modifyDocument( true );
}

void ColorPalette::slotRemoveFromCustomColors()
{
    custom_palette -> slotRemoveColor();
    k_toon -> drawingArea() -> modifyDocument( true );
}

void ColorPalette::slotSetColor( const QColor &new_color )
{
    disconnectMembers();
    connect( gradient, SIGNAL( gradientChanged( const QColor &, const QColor &, int, int, int, int ) ), grad_viewer, SLOT( slotUpdateGradient( const QColor &, const QColor &, int, int, int, int ) ) );
    connect( value_v, SIGNAL( valueChanged( int ) ), value_selector, SLOT( slotSetValue( int ) ) );

    Color *n_color = new Color( new_color.red() / 255.0, new_color.green() / 255.0, new_color.blue() / 255.0, value_alpha -> value() / 100.0 );
    if ( outline_color -> isActive() )
    {
        outline_color -> slotSetColor( new_color );
        k_toon -> currentStatus() -> setCurrentOutlineColor( n_color );
	emit outlineColorChanged();
    }
    else
    {
        fill_color -> slotSetColor( new_color );
        k_toon -> currentStatus() -> setCurrentFillColor( n_color );
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

void ColorPalette::slotSetColor( Color *new_color )
{
    QColor the_color = QColor( ( int )( new_color -> colorRed() * 255.0 ), ( int )( new_color -> colorGreen() * 255.0 ), ( int )( new_color -> colorBlue() * 255.0 ) );
    int the_alpha = ( int )( new_color -> colorAlpha() * 100.0 );
    slotSetColor( the_color );
    value_alpha -> setValue( the_alpha );

    if ( outline_color -> isActive() )
       k_toon -> currentStatus() -> setCurrentCursor( Tools::CONTOUR_FILL );
    else
       k_toon -> currentStatus() -> setCurrentCursor( Tools::FILL );

    k_toon -> slotActivateCursor();
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

    Color *n_color = new Color( new_color.red() / 255.0, new_color.green() / 255.0, new_color.blue() / 255.0, value_alpha -> value() / 100.0 );
    if ( outline_color -> isActive() )
    {
        k_toon -> currentStatus() -> setCurrentOutlineColor( n_color );
	emit outlineColorChanged();
    }
    else
    {
        k_toon -> currentStatus() -> setCurrentFillColor( n_color );
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

    Color *n_color = new Color( new_color.red() / 255.0, new_color.green() / 255.0, new_color.blue() / 255.0, value_alpha -> value() / 100.0 );
    if ( outline_color -> isActive() )
    {
        k_toon -> currentStatus() -> setCurrentOutlineColor( n_color );
	emit outlineColorChanged();
    }
    else
    {
        k_toon -> currentStatus() -> setCurrentFillColor( n_color );
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
    }
    else if ( value == tr( "Radial" ) )
    {
	gradient -> show();
	grad_viewer -> show();
	grad_viewer -> slotSetType( GradientViewer::RADIAL );
    }
    else
    {
	gradient -> hide();
	grad_viewer -> hide();
    }
}

void ColorPalette::slotChangeRGB( const QString &rgb )
{
    QColor intermediate;
    intermediate.setNamedColor( rgb );
    disconnect( value_rgb, 0, this, 0 );
    if ( intermediate.isValid() )
    {
        Color *n_color = new Color( intermediate.red() / 255.0, intermediate.green() / 255.0, intermediate.blue() / 255.0, value_alpha -> value() / 100.0 );
    	if ( outline_color -> isActive() )
    	{
            k_toon -> currentStatus() -> setCurrentOutlineColor( n_color );
	    emit outlineColorChanged();
    	}
    	else
    	{
            k_toon -> currentStatus() -> setCurrentFillColor( n_color );
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
        Color *o_color = k_toon -> currentStatus() -> currentOutlineColor();
	o_color -> setColorAlpha( new_alpha / 100.0 );
        k_toon -> currentStatus() -> setCurrentOutlineColor( o_color );
	emit outlineColorChanged();
    }
    else
    {
        Color *f_color = k_toon -> currentStatus() -> currentFillColor();
	f_color -> setColorAlpha( new_alpha / 100.0 );
        k_toon -> currentStatus() -> setCurrentFillColor( f_color );
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

void ColorPalette::closeEvent( QCloseEvent* close_event )
{
    assigned_menu -> setItemChecked( assigned_item, false );
    assigned_tb_button -> setDown( false );
    close_event -> accept();
}

void ColorPalette::makeConnections()
{
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
