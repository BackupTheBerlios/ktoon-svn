#include "dgradientselector.h"

#include <QPolygon>
#include <qdrawutil.h>

#include "kimageeffect.h"
#include "ddebug.h"

#include <QMatrix>


	
// DGradientArrow::DGradientArrow(QPoint pos, const QColor& color): QObject(), m_color(color)
// {
// 	QPolygon array(6);
// // 	if ( orientation() == Qt::Vertical )
// // 	{
// // 		array.setPoint( 0, pos.x()+0, pos.y()+0 );
// // 		array.setPoint( 1, pos.x()+5, pos.y()+5 );
// // 		array.setPoint( 2, pos.x()+5, pos.y()-5 );
// // 		
// 		
// // 	}
// // 	else
// // 	{
// 		array.setPoint( 0, pos.x()+0, pos.y()+0 );
// 		array.setPoint( 1, pos.x()+5, pos.y()+5 );
// 		array.setPoint( 2, pos.x()+5, pos.y()+9 );
// 		array.setPoint( 3, pos.x()-5, pos.y()+9 );
// 		array.setPoint( 4, pos.x()-5, pos.y()+5 );
// 		array.setPoint( 5, pos.x()+0, pos.y()+0 );
// // 	}
// 		m_form.addPolygon(array);
// }
// 
// DGradientArrow::~DGradientArrow()
// {
// 	DEND;
// }
// 
// double DGradientArrow::position()
// {
// 	return m_form.currentPosition().x();
// }
// 
// bool DGradientArrow::contains ( const QPoint & pt )
// {
// 	return m_form.contains (pt);
// }
// 
// void DGradientArrow::setColor( const QColor & color)
// {
// 	m_color = color;
// }
// 
// void DGradientArrow::moveArrow( const QPoint &pos )
// {	
// 	QMatrix matrix;
// 	
// 	matrix.translate(pos.x() - m_form.currentPosition().x(), 0);
// 	
// 	m_form = matrix.map(m_form);
// }
// 
// 
// void DGradientArrow::moveVertical(const QPoint &pos)
// {
// 	QMatrix matrix;
// 	
// 	matrix.translate(0, pos.y() - m_form.currentPosition().y());
// 	
// 	m_form = matrix.map(m_form);
// }
// 
// QPainterPath DGradientArrow::form()
// {
// 	return m_form;
// }
// 
// QColor DGradientArrow::color() const
// {
// 	return m_color;
// }


DGradientSelector::DGradientSelector( QWidget *parent ) : QAbstractSlider( parent ), m_currentArrowIndex(0), m_gradient(0,0,0,0), m_update(true), m_maxArrows(10)
{
	_orientation = Qt::Horizontal;
	init();
}



DGradientSelector::DGradientSelector( Qt::Orientation o, QWidget *parent )
	: QAbstractSlider( parent ), m_currentArrowIndex(0), m_gradient(0,0,0,0)
{
	_orientation = o;
	init();
}

void DGradientSelector::init()
{
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	
	setMaximumWidth(100);
	setMinimumWidth(100);

	connect(this, SIGNAL(valueChanged (int)), this, SLOT(valueChange(int)));
	
	show();
	
	DGradientArrow *first = new DGradientArrow(calcArrowPos(1), QColor(Qt::black));
// 	DGradientArrow * second= new DGradientArrow(calcArrowPos(0), QColor(Qt::white));
	m_arrows << first ;
// 	m_arrows << second ;
	createGradient();
	emit gradientChanged(  m_gradient.stops());;
}


DGradientSelector::~DGradientSelector()
{DEND;}




QRect DGradientSelector::contentsRect() const
{
	if ( orientation() == Qt::Vertical )
		return QRect( 2, 2, width()-14, height());
	else
		return QRect( 2, 2, width(), height()-14 );
}

void  DGradientSelector::setMaxArrows(int value)
{
	m_maxArrows = value;
	while(m_maxArrows < m_arrows.count())
	{
		m_arrows.removeLast();
	}
	update();
}

void DGradientSelector::paintEvent( QPaintEvent * )
{
	QPainter painter;
	
	
	painter.begin( this );

	drawContents( &painter );

	QBrush brush;

// 	if ( indent() )
// 	{
// 		if ( orientation() == Qt::Vertical )
// 			qDrawShadePanel( &painter, 0, 0, width()-1, height()-1,
// 					  colorGroup(), true, 2, &brush );
// 		else
// 			qDrawShadePanel( &painter, 0, 0, width()-1, height()-1,
// 					  colorGroup(), true, 2, &brush );
// 	}
	
	
	for(int i = 0; i < m_arrows.count(); i++)
	{
		painter.setBrush(m_arrows[i]->color());
		if(i == m_currentArrowIndex)
		{
			painter.setPen(QPen(palette().highlight(), 3));
		}
		else
		{
			painter.setPen(Qt::black);
		}
		painter.drawPath(m_arrows[i]->form());
	}
	
	painter.end();
}

void DGradientSelector::mousePressEvent( QMouseEvent *e )
{
	
	bool select = false;
	for(int i = 0; i < m_arrows.count(); i++)
	{
		DGradientArrow *aArrow = m_arrows[i];
		if ( aArrow->contains(e->pos() ) )
		{
			
			m_currentArrowIndex = i;
// 			moveArrow( e->pos() );
			select = true;
			break;
		}
	}
	if(m_arrows.count() > 2 && e->button() == Qt::RightButton )
	{
		m_arrows.removeAt(m_currentArrowIndex);
		repaint();
	}
	else if(!select)
	{
		int val;
		if ( orientation() == Qt::Vertical )
		{
			val = ( maximum() - minimum() ) * (height()-e->y())
				/ (height()) + minimum();
		}
		else
		{
			val = ( maximum() - minimum() ) * (width()-e->x())/width() + minimum();
		}
		
		addArrow( calcArrowPos(val), m_arrows[m_currentArrowIndex]->color());
	}
}

void DGradientSelector::mouseMoveEvent( QMouseEvent *e )
{
// 	if ( orientation() == Qt::Vertical && (e->y() <= minimum() || e->y() >= maximum()) )
// 	{
// 		return;
// 	}
// 	if(  orientation() == Qt::Horizontal && (e->x() <= minimum()+7 || e->x()-7 >= maximum()) )
// 	{
// 		return;
// 	}
	moveArrow( e->pos() );
	
}

void DGradientSelector::wheelEvent( QWheelEvent *e )
{
	int val = value() + e->delta()/120;
	setValue( val );
}

void  DGradientSelector::resizeEvent ( QResizeEvent * event )
{
	QAbstractSlider::setRange(0,width() );
	QAbstractSlider::setMaximum(width());
	m_update = true;
	for(int i =0; i < m_arrows.count(); i++)
	{
		m_arrows[i]->moveVertical(calcArrowPos( m_arrows[i]->position() ));
	}

	QWidget::resizeEvent (event);
	
}

void DGradientSelector::valueChange( int newV)
{
	update();
	
	emit newValue( newV );
}

void DGradientSelector::moveArrow( const QPoint &pos )
{
	if ( orientation() == Qt::Vertical && (pos.y() <= minimum() || pos.y() >= maximum()) )
	{
		return;
	}
	if(  orientation() == Qt::Horizontal &&  (pos.x() <= minimum() || pos.x() >= maximum()) )
	{
		return;
	}
	int val;

	if ( orientation() == Qt::Vertical )
	{
		val = ( maximum() - minimum() ) * (height()-pos.y())
				/ (height()) + minimum();
	}
	else
	{
		val = ( maximum() - minimum() ) * (width()-pos.x())
				/ (width()) + minimum();
	}

	setValue( val );
	
	m_arrows[m_currentArrowIndex]->moveArrow(pos);
	m_update = true;
	
	emit gradientChanged(  m_gradient.stops());
}

QPoint DGradientSelector::calcArrowPos( int val )
{
	QPoint p;

	if ( orientation() == Qt::Vertical )
	{
		p.setY( height() - ( (height()-10) * val
				/ ( maximum() - minimum() ) + 5 ) );
		p.setX( width() - 10 );
	}
	else
	{
		p.setX( width() - ( (width()) * val
				/ ( maximum() - minimum() )  ) );
		p.setY( height() - 10 );
	}

	return p;
}

void DGradientSelector::drawContents( QPainter *painter )
{
	createGradient();
	painter->setBrush(m_gradient);
	painter->drawRect(contentsRect());
}

void  DGradientSelector::createGradient()
{
	m_gradient = QLinearGradient(contentsRect().topLeft(), contentsRect().topRight () );
	for(int i = 0; i < m_arrows.count(); i++)
	{
		m_gradient.setColorAt( valueToGradient(m_arrows[i]->position()), m_arrows[i]->color());
	}
}

double DGradientSelector::valueToGradient(int value) const
{
	const float factor = static_cast<float>( ( /*maximum ()  -*/ value ))/ maximum();
	return factor;
}

void DGradientSelector::addArrow(QPoint position, QColor color)
{
	if(m_arrows.count() < m_maxArrows)
	{
		DGradientArrow *arrow = new DGradientArrow(position, color);
		m_arrows << arrow;
		m_currentArrowIndex = m_arrows.count()-1;
		update();
		
		emit arrowAdded();
	}
	
}

void DGradientSelector::setCurrentColor(const QColor& color)
{
	if ( m_arrows.count() > 0 )
	{
		DGradientArrow *arrow  = m_arrows[m_currentArrowIndex];
		if ( arrow )
		{
			
			m_arrows[m_currentArrowIndex]->setColor(color);
			
		}
	}

	if(!isVisible())
	{
		createGradient();
	}
	else
	{
		repaint();
	}
	emit gradientChanged(  m_gradient.stops());;
}

void DGradientSelector::setStops(const QGradientStops &stops)
{
	m_gradient.setStops(stops);
	m_arrows.clear();
// 	createGradient();
	for(int i = 0; i < stops.count(); i++)
	{
		addArrow( calcArrowPos(100 - (stops[i].first *100)), stops[i].second );
		dDebug() << "value" <<stops[i].first << "color " << stops[i].second;
	}
	
	
	update();
}
