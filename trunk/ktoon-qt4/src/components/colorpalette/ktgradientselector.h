
#ifndef KTGRADIENTSELECTOR_H
#define KTGRADIENTSELECTOR_H

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

class KTGradientArrow : public QObject
{
	public:
		KTGradientArrow(QPoint pos, QColor color);
		~KTGradientArrow();
		double position();
		bool contains ( const QPoint & pt );
		void moveArrow( const QPoint &pos );
		QPainterPath form();
		
	private:
		QPainterPath m_form;
		QColor m_color;
		
};


class KTGradientSelector : public QAbstractSlider
{
	Q_OBJECT;
	
	public:

		KTGradientSelector( QWidget *parent=0 );

		KTGradientSelector( Qt::Orientation o, QWidget *parent = 0 );

		~KTGradientSelector();

		void setColors( const QColor &col1, const QColor &col2 )
		{	color1 = col1; color2 = col2; update(); }
		void setText( const QString &t1, const QString &t2 )
		{	text1 = t1; text2 = t2; update(); }

		void setFirstColor( const QColor &col )
		{ color1 = col; update(); }
		void setSecondColor( const QColor &col )
		{ color2 = col; update(); }

		void setFirstText( const QString &t )
		{ text1 = t; update(); }
		void setSecondText( const QString &t )
		{ text2 = t; update(); }

		const QColor firstColor() const
		{ return color1; }
		const QColor secondColor() const
		{ return color2; }

		const QString firstText() const
		{ return text1; }
		const QString secondText() const
		{ return text2; }


		Qt::Orientation orientation() const
		{	return _orientation; }


		QRect contentsRect() const;

		void setIndent( bool i )
		{	_indent = i; }

		bool indent() const
		{	return _indent; }

		void setValue(int value)
		{
			QAbstractSlider::setValue(value);
		}

		int value() const
		{
			return QAbstractSlider
				::value();
		}

		void setMinValue(int value)
		{ QAbstractSlider::setMinValue(value); }

		int minValue() const
		{
			return QAbstractSlider::minValue();
		}


		void setMaxValue(int value)
		{
			QAbstractSlider::setMaxValue(value);
		}
		int maxValue() const
		{
			return QAbstractSlider::maxValue();
		}
		
		qreal valueToGradien(int _value) const;

	signals:
		void newValue( int value );

	protected:
		virtual void drawContents( QPainter * );
		virtual void drawArrow( QPainter *painter, bool show, const QPoint &pos );
		virtual void drawArrows( QPainter *painter, bool show, QList<qreal> values );

		virtual void valueChange( int value);
		virtual void paintEvent( QPaintEvent * );
		virtual void mousePressEvent( QMouseEvent *e );
		virtual void mouseMoveEvent( QMouseEvent *e );
		virtual void wheelEvent( QWheelEvent * );

		virtual QSize minimumSize() const
		{ return sizeHint(); }

	private:
		QPoint calcArrowPos( int val );
		void moveArrow( const QPoint &pos );

		Qt::Orientation _orientation;
		bool _indent;

	private:
		void init();
// 		QList<qreal> m_values;
		QColor color1;
		QColor color2;
		QString text1;
		QString text2;
		int m_currentArrowIndex;
		QLinearGradient m_gradient;
		
		QList<KTGradientArrow*> m_arrows;
};

#endif
