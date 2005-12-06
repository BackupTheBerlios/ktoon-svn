
#include "ktluminancepicker.h"

#include <QPainter>
#include <qdrawutil.h>
#include "ktdebug.h"
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
	:QWidget(parent)
{
	hue = 100; val = 100; sat = 100;
	pix = 0;

    //    setAttribute(WA_NoErase, true);
}

KTLuminancePicker::~KTLuminancePicker()
{
	delete pix;
	KTEND;
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
	if (val == v)
		return;
	val = qMax(0, qMin(v,255));
	delete pix; pix=0;
	repaint();
	emit newHsv(hue, sat, val);
}

//receives from a hue,sat chooser and relays.
void KTLuminancePicker::setCol(int h, int s)
{
	setCol(h, s, val);
	emit newHsv(h, s, val);
}

void KTLuminancePicker::paintEvent(QPaintEvent *)
{
	int w = width() - 5;

	QRect r(0, foff, w, height() - 2*foff);
	int wi = r.width() - 2;
	int hi = r.height() - 2;
	if (!pix || pix->height() != hi || pix->width() != wi) {
		delete pix;
		QImage img(wi, hi, QImage::Format_RGB32);
		int y;
		for (y = 0; y < hi; y++) {
			QColor c;
			c.setHsv(hue, sat, y2val(y+coff));
			QRgb r = c.rgb();
			int x;
			for (x = 0; x < wi; x++)
				img.setPixel(x, y, r);
		}
		pix = new QPixmap(QPixmap::fromImage(img));
	}
	QPainter p(this);
	p.drawPixmap(1, coff, *pix);
	const QPalette &g = palette();
	qDrawShadePanel(&p, r, g, true);
	p.setPen(g.foreground().color());
	p.setBrush(g.foreground());
	QPolygon a;
	int y = val2y(val);
	a.setPoints(3, w, y, w+5, y+5, w+5, y-5);
	p.eraseRect(w, 0, 5, height());
	p.drawPolygon(a);
}

void KTLuminancePicker::setCol(int h, int s , int v)
{
	val = v;
	hue = h;
	sat = s;
	delete pix; pix=0;
	repaint();
}
