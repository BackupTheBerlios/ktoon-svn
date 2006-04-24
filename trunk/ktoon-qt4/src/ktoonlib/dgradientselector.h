
#ifndef DGRADIENTSELECTOR_H
#define DGRADIENTSELECTOR_H

#include <QAbstractSlider>
#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QLinearGradient>
#include <QPainterPath>

#include <QList>



/**
 * @if english
 * @short translate me
 * @elseif spanish
 * @short Esta clase provee de una simple interfaz grafica para generar "GradientStops".
 * @endif
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 */
class DGradientSelector : public QAbstractSlider
{
	Q_OBJECT;
	
	private:
		class DGradientArrow : public QObject
		{
			public:
				DGradientArrow(QPoint pos, const QColor& color): m_color(color)
				{
					QPolygon array(6);
				// 	if ( orientation() == Qt::Vertical )
				// 	{
				// 		array.setPoint( 0, pos.x()+0, pos.y()+0 );
				// 		array.setPoint( 1, pos.x()+5, pos.y()+5 );
				// 		array.setPoint( 2, pos.x()+5, pos.y()-5 );
									// 		
						
				// 	}
				// 	else
				// 	{
					array.setPoint( 0, pos.x()+0, pos.y()+0 );
					array.setPoint( 1, pos.x()+5, pos.y()+5 );
					array.setPoint( 2, pos.x()+5, pos.y()+9 );
					array.setPoint( 3, pos.x()-5, pos.y()+9 );
					array.setPoint( 4, pos.x()-5, pos.y()+5 );
					array.setPoint( 5, pos.x()+0, pos.y()+0 );
// 					}
					m_form.addPolygon(array);
				}
				~DGradientArrow(){}
				double position() 
				{
					return m_form.currentPosition().x();
				}
				bool contains ( const QPoint & pt )
				{
					return m_form.contains (pt);
				}
				void moveArrow( const QPoint &pos )
				{	
					QMatrix matrix;
	
					matrix.translate(pos.x() - m_form.currentPosition().x(), 0);
	
					m_form = matrix.map(m_form);
				}
				QPainterPath form()
				{
					return m_form;
				}
				QColor color() const
				{
					return m_color;
				}
				void setColor(const QColor &color)
				{
					m_color = color;
				}
				void moveVertical(const QPoint &pos)
				{
					QMatrix matrix;
	
					matrix.translate(0, pos.y() - m_form.currentPosition().y());
	
					m_form = matrix.map(m_form);
				}
				QPainterPath m_form;
				QColor m_color;
		
		};;
		
	public:

		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Constructor por defecto.
		 * @endif
		 */
		DGradientSelector( QWidget *parent=0 );
		
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Crea un DGradientSelector, con una orientacion definida.
		 * @endif
		 * @see Qt::Orientation
		 */
		DGradientSelector( Qt::Orientation o, QWidget *parent = 0 );
		/**
		 * Destructor
		 */
		~DGradientSelector();
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Pone un GradienStops, para ser editado.
		 * @endif
		 */
		void setStops(const QGradientStops &);
		
// 		void setColors( const QColor &col1, const QColor &col2 )
// 		{	color1 = col1; color2 = col2; update(); }
// 		void setText( const QString &t1, const QString &t2 )
// 		{	text1 = t1; text2 = t2; update(); }

// 		void setFirstColor( const QColor &col )
// 		{ color1 = col; update(); }
// 		void setSecondColor( const QColor &col )
// 		{ color2 = col; update(); }

// 		void setFirstText( const QString &t )
// 		{ text1 = t; update(); }
// 		void setSecondText( const QString &t )
// 		{ text2 = t; update(); }

// 		const QColor firstColor() const
// 		{ return color1; }
// 		const QColor secondColor() const
// 		{ return color2; }

// 		const QString firstText() const
// 		{ return text1; }
// 		const QString secondText() const
// 		{ return text2; }


		Qt::Orientation orientation() const
		{	return _orientation; }


		QRect contentsRect() const;

		void setValue(int value)
		{
			QAbstractSlider::setValue(value);
		}

		int value() const
		{
			return QAbstractSlider
				::value();
		}

		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Pone el numero maximo de flechas, que definen el "GradienStops".
		 * @endif
		 */
		void setMaxArrows(int value);
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Devuelve el "GradienStops" actual.
		 * @endif
		 */
		QGradientStops  gradientStops() const
		{
			return m_gradient.stops();
		}
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Crea el "GradienStops" actual apartir de las flechas acutales.
		 * @endif
		 */
		void  createGradient();
// 		void setMinValue(int value)
// 		{ QAbstractSlider::setMaximum(value); }

// 		int minValue() const
// 		{
// 			return QAbstractSlider::minValue();
// 		}


// 		void setMaxValue(int value)
// 		{
// 			QAbstractSlider::setMaximum(value);
// 		}
		
// 		int maxValue() const
// 		{
// 			return QAbstractSlider::maxValue();
// 		}
		
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Cambia el color de la flecha actualmente seleccionada.
		 * @endif
		 */
		void setCurrentColor(const QColor& color);
	signals:
		void newValue( int value );
		void gradientChanged(  const QGradientStops& );
		void arrowAdded();
		
	public slots:
		void addArrow(QPoint position, QColor color);
		
	protected:
		virtual void drawContents( QPainter * );
		virtual void valueChange( int value);
		virtual void paintEvent( QPaintEvent * );
		virtual void mousePressEvent( QMouseEvent *e );
		virtual void mouseMoveEvent( QMouseEvent *e );
		virtual void wheelEvent( QWheelEvent * );

		virtual QSize minimumSize() const
		{ return sizeHint(); }
		virtual void resizeEvent ( QResizeEvent * event );
		virtual QSize sizeHint() const
		{
			return QSize(width(), 35);
		}
		
	private:
		QPoint calcArrowPos( int val );
		void moveArrow( const QPoint &pos );

		double valueToGradient(int _value) const;
		Qt::Orientation _orientation;
		
	private:
		void init();
		int m_currentArrowIndex;
		QLinearGradient m_gradient;
		QList<DGradientArrow*> m_arrows;
		bool m_update;
		int m_maxArrows;
		QImage m_buffer;
};

#endif
