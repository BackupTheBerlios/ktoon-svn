
#include "dclicklineedit.h"

#include <QPainter>


struct DClickLineEdit::Private
{
	QString clickMessage;
	bool drawClickMsg;
};

DClickLineEdit::DClickLineEdit( const QString &msg, QWidget *parent) : QLineEdit(parent), d(new Private)
{
	d->drawClickMsg = true;
	setClickMessage( msg );
	
	setFocusPolicy ( Qt::ClickFocus );
	
	setStyleSheet(QString(":enabled { padding-right: %1; }").arg(8));
}

DClickLineEdit::~DClickLineEdit()
{
	delete d;
}

void DClickLineEdit::setClickMessage( const QString &msg )
{
	d->clickMessage = msg;
	repaint();
}

QString DClickLineEdit::clickMessage() const
{
	return d->clickMessage;
}


void DClickLineEdit::setText( const QString &txt )
{
	d->drawClickMsg = txt.isEmpty();
	repaint();
	QLineEdit::setText( txt );
}

void DClickLineEdit::paintEvent( QPaintEvent *e )
{
	QLineEdit::paintEvent(e);
	
	QPainter p(this);
	if ( d->drawClickMsg == true && !hasFocus() )
	{
		QPen tmp = p.pen();
		p.setPen( palette().color( QPalette::Disabled, QPalette::Text ) );
		QRect cr = contentsRect();
		
		cr.adjust(3, 0, 0 ,0);
		p.drawText( cr, Qt::AlignVCenter, d->clickMessage );
		p.setPen( tmp );
	}
}


void DClickLineEdit::focusInEvent( QFocusEvent *ev )
{
	if ( d->drawClickMsg == true ) 
	{
		d->drawClickMsg = false;
		repaint();
	}
	QLineEdit::focusInEvent( ev );
}


void DClickLineEdit::focusOutEvent( QFocusEvent *ev )
{
	if ( text().isEmpty() ) 
	{
		d->drawClickMsg = true;
		repaint();
	}
	QLineEdit::focusOutEvent( ev );
}




