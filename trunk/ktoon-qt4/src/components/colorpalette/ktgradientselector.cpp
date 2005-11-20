#include "ktgradientselector.h"

#include <QPolygon>
#include <qdrawutil.h>

#include "kimageeffect.h"

KTGradientSelector::KTGradientSelector( QWidget *parent ) : QAbstractSlider( parent )
{
	_orientation = Qt::Horizontal;
	_indent = true;
	init();
}

KTGradientSelector::KTGradientSelector( Qt::Orientation o, QWidget *parent )
	: QAbstractSlider( parent )
{
	_orientation = o;
	_indent = true;
	init();
}

void KTGradientSelector::init()
{
	color1.setRgb( 0, 0, 0 );
	color2.setRgb( 255, 255, 255 );

	text1 = text2 = "";
	
	connect(this, SIGNAL(valueChanged (int)), this, SLOT(valueChange(int)));
	
	show();
	
// 	QPalette pal;
// 	pal.setColor(QPalette::Background, Qt::blue);
// 	setPalette(pal);
// 	if ( sizeHint().isValid())
// 	setMinimumSize(sizeHint());
	setMinimumSize(100,30);
}


KTGradientSelector::~KTGradientSelector()
{}


QRect KTGradientSelector::contentsRect() const
{
	if ( orientation() == Qt::Vertical )
		return QRect( 2, 5, width()-9, height()-10 );
	else
		return QRect( 5, 2, width()-10, height()-9 );
}

void KTGradientSelector::paintEvent( QPaintEvent * )
{
	QPainter painter;

	painter.begin( this );

// 	drawContents( &painter );

	QBrush brush; 

	if ( indent() )
	{
		if ( orientation() == Qt::Vertical )
			qDrawShadePanel( &painter, 0, 3, width()-5, height()-6,
					  colorGroup(), true, 2, &brush );
		else
			qDrawShadePanel( &painter, 3, 0, width()-6, height()-5,
					  colorGroup(), true, 2, &brush );
	}

	QPoint pos = calcArrowPos( value() );
	drawArrow( &painter, true, pos );

	painter.end();
}

void KTGradientSelector::mousePressEvent( QMouseEvent *e )
{
	moveArrow( e->pos() );
}

void KTGradientSelector::mouseMoveEvent( QMouseEvent *e )
{
	moveArrow( e->pos() );
}

void KTGradientSelector::wheelEvent( QWheelEvent *e )
{
	int val = value() + e->delta()/120;
	setValue( val );
}

void KTGradientSelector::valueChange( int newV)
{
	QPoint prevPos, newPos;

	prevPos = calcArrowPos( value() );
	newPos = calcArrowPos( newV );
	update(QRect(prevPos, newPos));

	emit newValue( newV );
}

void KTGradientSelector::moveArrow( const QPoint &pos )
{
	int val;

	if ( orientation() == Qt::Vertical )
		val = ( maxValue() - minValue() ) * (height()-pos.y()-3)
				/ (height()-10) + minValue();
	else
		val = ( maxValue() - minValue() ) * (width()-pos.x()-3)
				/ (width()-10) + minValue();

	setValue( val );
}

QPoint KTGradientSelector::calcArrowPos( int val )
{
	QPoint p;

	if ( orientation() == Qt::Vertical )
	{
		p.setY( height() - ( (height()-10) * val
				/ ( maxValue() - minValue() ) + 5 ) );
		p.setX( width() - 5 );
	}
	else
	{
		p.setX( width() - ( (width()-10) * val
				/ ( maxValue() - minValue() ) + 5 ) );
		p.setY( height() - 5 );
	}

	return p;
}

void KTGradientSelector::drawContents( QPainter *painter )
{
	QImage image( contentsRect().width(), contentsRect().height(), 32 );

	QColor col;
	float scale;

	int redDiff   = color2.red() - color1.red();
	int greenDiff = color2.green() - color1.green();
	int blueDiff  = color2.blue() - color1.blue();

	if ( orientation() == Qt::Vertical )
	{
		for ( int y = 0; y < image.height(); y++ )
		{
			scale = 1.0 * y / image.height();
			col.setRgb( color1.red() + int(redDiff*scale),
				    color1.green() + int(greenDiff*scale),
				    color1.blue() + int(blueDiff*scale) );

			unsigned int *p = (uint *) image.scanLine( y );
			for ( int x = 0; x < image.width(); x++ )
				*p++ = col.rgb();
		}
	}
	else
	{
		unsigned int *p = (uint *) image.scanLine( 0 );

		for ( int x = 0; x < image.width(); x++ )
		{
			scale = 1.0 * x / image.width();
			col.setRgb( color1.red() + int(redDiff*scale),
				    color1.green() + int(greenDiff*scale),
				    color1.blue() + int(blueDiff*scale) );
			*p++ = col.rgb();
		}

		for ( int y = 1; y < image.height(); y++ )
			memcpy( image.scanLine( y ), image.scanLine( y - 1),
				sizeof( unsigned int ) * image.width() );
	}

	QColor ditherPalette[8];

	for ( int s = 0; s < 8; s++ )
		ditherPalette[s].setRgb( color1.red() + redDiff * s / 8,
					 color1.green() + greenDiff * s / 8,
					 color1.blue() + blueDiff * s / 8 );

	KImageEffect::dither( image, ditherPalette, 8 );

	QPixmap p;
	p.convertFromImage( image );

	painter->drawPixmap( contentsRect().x(), contentsRect().y(), p );

	if ( orientation() == Qt::Vertical )
	{
		int yPos = contentsRect().top() + painter->fontMetrics().ascent() + 2;
		int xPos = contentsRect().left() + (contentsRect().width() -
				painter->fontMetrics().width( text2 )) / 2;
		QPen pen( color2 );
		painter->setPen( pen );
		painter->drawText( xPos, yPos, text2 );

		yPos = contentsRect().bottom() - painter->fontMetrics().descent() - 2;
		xPos = contentsRect().left() + (contentsRect().width() -
				painter->fontMetrics().width( text1 )) / 2;
		pen.setColor( color1 );
		painter->setPen( pen );
		painter->drawText( xPos, yPos, text1 );
	}
	else
	{
		int yPos = contentsRect().bottom()-painter->fontMetrics().descent()-2;

		QPen pen( color2 );
		painter->setPen( pen );
		painter->drawText( contentsRect().left() + 2, yPos, text1 );

		pen.setColor( color1 );
		painter->setPen( pen );
		painter->drawText( contentsRect().right() -
				painter->fontMetrics().width( text2 ) - 2, yPos, text2 );
	}
}

void KTGradientSelector::drawArrow( QPainter *painter, bool show, const QPoint &pos )
{
	if ( show )
	{
		QPolygon array(3);

		painter->setPen( QPen() );
		painter->setBrush( QBrush( colorGroup().buttonText() ) );
		if ( orientation() == Qt::Vertical )
		{
			array.setPoint( 0, pos.x()+0, pos.y()+0 );
			array.setPoint( 1, pos.x()+5, pos.y()+5 );
			array.setPoint( 2, pos.x()+5, pos.y()-5 );
		}
		else
		{
			array.setPoint( 0, pos.x()+0, pos.y()+0 );
			array.setPoint( 1, pos.x()+5, pos.y()+5 );
			array.setPoint( 2, pos.x()-5, pos.y()+5 );
		}

		painter->drawPolygon( array );
	}
	else
	{
		if ( orientation() == Qt::Vertical )
		{
			repaint(pos.x(), pos.y()-5, 6, 11, true);
		}
		else
		{
			repaint(pos.x()-5, pos.y(), 11, 6, true);
		}
	}
}
