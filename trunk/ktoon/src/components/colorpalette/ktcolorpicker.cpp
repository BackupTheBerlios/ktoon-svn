#include <QPoint>
#include <QColor>
#include <QImage>
#include <QSizePolicy>
#include <QPixmap>
#include <QSize>
#include <QRect>
#include <QPainter>
#include <QMouseEvent>

#include "ktcolorpicker.h"
#include <kcore/kdebug.h>

// static int pWidth = 220;
// static int pHeight = 200;
static int pWidth = 100;
static int pHeight = 80;
// static int pWidth = 150;
// static int pHeight = 130;

struct KTColorPicker::Private
{
	int hue;
	int sat;
	QPixmap *pix;
	~Private()
	{
		delete pix;
	};
};


QPoint KTColorPicker::colPt()
{ 
	return QPoint((360-d->hue)*(pWidth-1)/360, (255-d->sat)*(pHeight-1)/255); 
}
int KTColorPicker::huePt(const QPoint &pt)
{ 
	return 360 - pt.x()*360/(pWidth-1); 
}

int KTColorPicker::satPt(const QPoint &pt)
{ 
	return 255 - pt.y()*255/(pHeight-1) ; 
}

void KTColorPicker::setCol(const QPoint &pt)
{ 
	setCol(huePt(pt), satPt(pt)); 
}

KTColorPicker::KTColorPicker(QWidget* parent) : QFrame(parent), d( new Private)
{
	d->hue = 0;
	d->sat = 0;
	setCol(150, 255);

	QImage img(pWidth, pHeight, QImage::Format_RGB32);
	int x,y;
	for (y = 0; y < pHeight; y++)
	{
		for (x = 0; x < pWidth; x++) 
		{
			QPoint p(x, y);
			QColor c;
			c.setHsv(huePt(p), satPt(p), 200);
			img.setPixel(x, y, c.rgb());
		}
	}
    
	d->pix = new QPixmap(QPixmap::fromImage(img));
	setAttribute(Qt::WA_NoSystemBackground);
	setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed) );

}

KTColorPicker::~KTColorPicker()
{
	delete d;
	DEND;
}

QSize KTColorPicker::sizeHint() const
{
	return QSize(pWidth + 2*frameWidth(), pHeight + 2*frameWidth());
}

void KTColorPicker::setCol(int h, int s)
{
	int nhue = qMin(qMax(0,h), 359);
	int nsat = qMin(qMax(0,s), 255);
	if (nhue == d->hue && nsat == d->sat)
		return;
	QRect r(colPt(), QSize(20,20));
	d->hue = nhue; d->sat = nsat;
	r = r.unite(QRect(colPt(), QSize(20,20)));
	r.translate(contentsRect().x()-9, contentsRect().y()-9);
    //    update(r);
	repaint(r);
}

void KTColorPicker::setH(int h)
{
	int nhue = qMin(qMax(0,h), 359);
	if (nhue == d->hue )
		return;
	QRect r(colPt(), QSize(20,20));
	d->hue = nhue;
	r = r.unite(QRect(colPt(), QSize(20,20)));
	r.translate(contentsRect().x()-9, contentsRect().y()-9);
	repaint(r);
	
}

void KTColorPicker::setS(int s)
{
	int nsat = qMin(qMax(0,s), 255);
	if ( nsat == d->sat)
		return;
	QRect r(colPt(), QSize(20,20));
	d->sat = nsat;
	r = r.unite(QRect(colPt(), QSize(20,20)));
	r.translate(contentsRect().x()-9, contentsRect().y()-9);
    //    update(r);
	repaint(r);
}


void KTColorPicker::mouseMoveEvent(QMouseEvent *m)
{
	QPoint p = m->pos() - contentsRect().topLeft();
	setCol(p);
	emit newCol(d->hue, d->sat);
}

void KTColorPicker::mousePressEvent(QMouseEvent *m)
{
	QPoint p = m->pos() - contentsRect().topLeft();
	setCol(p);
	emit newCol(d->hue, d->sat);
}

void KTColorPicker::paintEvent(QPaintEvent* )
{
	QPainter p(this);
	drawFrame(&p);
	QRect r = contentsRect();

	p.drawPixmap(r.topLeft(), *d->pix);
	QPoint pt = colPt() + r.topLeft();
	p.setPen(Qt::black);

	p.fillRect(pt.x()-9, pt.y(), 20, 2, Qt::black);
	p.fillRect(pt.x(), pt.y()-9, 2, 20, Qt::black);
}

int KTColorPicker::hue()
{
	return d->hue;
}

int KTColorPicker::sat()
{
	return d->sat;
}
