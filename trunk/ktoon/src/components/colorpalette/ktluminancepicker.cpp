
#include "ktluminancepicker.h"

#include <QPainter>
#include <qdrawutil.h>
#include <dcore/ddebug.h>

struct KTLuminancePicker::Private
{
	int val;
	int hue;
	int sat;
	QPixmap *pix;
	
	~Private()
	{
		delete pix;
	};
	
};

int KTLuminancePicker::y2val(int y)
{
	int d = height() - 2*coff - 1;
	return 255 - (y - coff)*255/d;
}

int KTLuminancePicker::val2y(int v)
{
	int d = height() - 2*coff - 1;
	return coff + (255-v)*d/255;
}

KTLuminancePicker::KTLuminancePicker(QWidget* parent)
	:QWidget(parent), d( new Private)
{
	d->hue = 100; d->val = 100; d->sat = 100;
	d->pix = 0;
    //    setAttribute(WA_NoErase, true);
}

KTLuminancePicker::~KTLuminancePicker()
{
	delete d;
	DEND;
}

void KTLuminancePicker::mouseMoveEvent(QMouseEvent *m)
{
	setVal(y2val(m->y()));
	
}
void KTLuminancePicker::mousePressEvent(QMouseEvent *m)
{
	setVal(y2val(m->y()));
}

void KTLuminancePicker::setVal(int v)
{
	if (d->val == v)
		return;
	d->val = qMax(0, qMin(v,255));
	delete d->pix; d->pix=0;
	repaint();
	emit newHsv(d->hue, d->sat, d->val);
}

//receives from a d->hue,d->sat chooser and relays.
void KTLuminancePicker::setCol(int h, int s)
{
	setCol(h, s, d->val);
	emit newHsv(h, s, d->val);
}

void KTLuminancePicker::paintEvent(QPaintEvent *)
{
	int w = width() - 5;

	QRect r(0, foff, w, height() - 2*foff);
	int wi = r.width() - 2;
	int hi = r.height() - 2;
	if (!d->pix || d->pix->height() != hi || d->pix->width() != wi) {
		delete d->pix;
		QImage img(wi, hi, QImage::Format_RGB32);
		int y;
		for (y = 0; y < hi; y++) {
			QColor c;
			c.setHsv(d->hue, d->sat, y2val(y+coff));
			QRgb r = c.rgb();
			int x;
			for (x = 0; x < wi; x++)
				img.setPixel(x, y, r);
		}
		d->pix = new QPixmap(QPixmap::fromImage(img));
	}
	QPainter p(this);
	p.drawPixmap(1, coff, *d->pix);
	const QPalette &g = palette();
	qDrawShadePanel(&p, r, g, true);
	p.setPen(g.foreground().color());
	p.setBrush(g.foreground());
	QPolygon a;
	int y = val2y(d->val);
	a.setPoints(3, w, y, w+5, y+5, w+5, y-5);
	p.eraseRect(w, 0, 5, height());
	p.drawPolygon(a);
}

void KTLuminancePicker::setCol(int h, int s , int v)
{
	d->val = v;
	d->hue = h;
	d->sat = s;
	delete d->pix; d->pix=0;
	repaint();
}

int  KTLuminancePicker::value()
{
	return d->val;
}

