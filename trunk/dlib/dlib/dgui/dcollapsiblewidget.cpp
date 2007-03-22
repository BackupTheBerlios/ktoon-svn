/*
   This file is part of the KDE libraries
   Copyright (C) 2005 Daniel Molkentin <molkentin@kde.org>
   Copyright (C) 2006 David Cuadrado <krawek@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <QTextDocument>
#include <QMimeData>
#include <QPainter>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QVBoxLayout>
#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QPainterPath>
#include <QStyleOption>
#include <QGroupBox>
#include <QFontMetrics>

#include <dcollapsiblewidget.h>

#include <ddebug.h>

/******************************************************************
 * Helper classes
 *****************************************************************/


class CollapsibleMimeData : public QMimeData
{
	public:
		CollapsibleMimeData();
		~CollapsibleMimeData();
		
		void setCollapsibleWidget(DCollapsibleWidget *w);
		DCollapsibleWidget *collapsibleWidget() const;
		
	private:
		DCollapsibleWidget *m_cw;
};


CollapsibleMimeData::CollapsibleMimeData() : QMimeData(), m_cw(0)
{
}

CollapsibleMimeData::~CollapsibleMimeData()
{
}


void CollapsibleMimeData::setCollapsibleWidget(DCollapsibleWidget *w)
{
	m_cw = w;
}

DCollapsibleWidget *CollapsibleMimeData::collapsibleWidget() const
{
	return m_cw;
}


DClickableLabel::DClickableLabel( QWidget* parent )
	: QWidget( parent ), m_isEnter(false), m_isDragging(false), m_checked(false)
{
	setMouseTracking( false );
	
	m_text = new QTextDocument(this);
}

DClickableLabel::~DClickableLabel()
{
}

QSize DClickableLabel::sizeHint() const
{
	return m_text->size().toSize();
}

void DClickableLabel::setText(const QString &text)
{
	m_text->setHtml(text);
}

QString DClickableLabel::text() const
{
	return m_text->toPlainText();
}

void DClickableLabel::setChecked(bool c)
{
	m_checked = c;
}

bool DClickableLabel::isChecked() const
{
	return m_checked;
}

void DClickableLabel::mousePressEvent (QMouseEvent *e)
{
	m_isDragging = false;
	
	m_position = e->pos();
	QWidget::mousePressEvent( e );
}

void DClickableLabel::mouseReleaseEvent( QMouseEvent *e )
{
	if ( ! m_isDragging )
	{
		m_checked = !m_checked;
		
		emit clicked();
	}
}

void DClickableLabel::mouseMoveEvent(QMouseEvent* e)
{
	QWidget::mouseMoveEvent( e );

	if ((e->pos() - m_position).manhattanLength() <  QApplication::startDragDistance())
	{
		return;
	}
	
	QDrag *drag = new QDrag( this );
	CollapsibleMimeData *mimeData = new CollapsibleMimeData;
	DCollapsibleWidget *parent = dynamic_cast<DCollapsibleWidget *>(parentWidget());
	
	if ( ! parent ) return;
	QWidget *inner = parent->innerWidget();
	if ( ! inner ) return;
	
	mimeData->setCollapsibleWidget( parent );
	
	QPixmap wpx = QPixmap::grabWidget(parent);
	QPainter p(&wpx);
	
	p.drawRoundRect(wpx.rect(), 10, 10);
	
	drag->setPixmap( wpx );
	
	drag->setMimeData(mimeData);
	
	
	Qt::DropAction dropAction = drag->start(Qt::MoveAction);
	
	m_isDragging = true;
}

void DClickableLabel::paintEvent (QPaintEvent *e)
{
	QPainter painter(this);
	if ( m_isEnter )
	{
		painter.save();
		
		QRect r = rect();
	
		double h = r.height();
		double h2 = r.height() / 2.0;
		QPainterPath path;
		path.addRect(r.x() + h2, r.y() + 0, r.width() - h2 * 2, r.height());
		path.addEllipse(r.x(), r.y(), h, h);
		path.addEllipse(r.x() + r.width() - h, r.y(), h, h);
		path.setFillRule(Qt::WindingFill);
		painter.setPen(Qt::NoPen);
		painter.setBrush( palette().brush(QPalette::Highlight) );
		painter.setRenderHint(QPainter::Antialiasing);
		painter.drawPath(path);
		
		painter.restore();
	}
	
	QRect r = rect();
	r.setX( (r.x() + m_text->textWidth())/2 );
	
#if QT_VERSION >= 0x040200
	m_text->drawContents(&painter, r);
#else
	painter.drawText(r, text() );
#endif
	
	painter.end();
	QWidget::paintEvent(e);
}

void DClickableLabel::enterEvent ( QEvent * e)
{
	m_isEnter = true;
	update();
	QWidget::enterEvent(e);
}

void DClickableLabel::leaveEvent(QEvent *e)
{
	m_isEnter = false;
	update();
	QWidget::leaveEvent(e);
}


/******************************************************************
 * Private classes
 *****************************************************************/

class DCollapsibleWidget::Private
{
	public:
		QGridLayout    *cwlayout;
		QWidget        *innerWidget;
		DClickableLabel *label;
};

class DSettingsContainer::Private
{
	public:
		QVBoxLayout *layout;
		QList<DCollapsibleWidget *> collapsibles;
};

/******************************************************************
 * Implementation
 *****************************************************************/

DSettingsContainer::DSettingsContainer(QWidget *parent)
	: QScrollArea( parent ), d(new DSettingsContainer::Private)
{
	QWidget *w = new QWidget;
	QVBoxLayout *helperLay = new QVBoxLayout(w);
	d->layout = new QVBoxLayout;
	helperLay->addLayout( d->layout );
	helperLay->addStretch(2);
	setWidget(w);
	setWidgetResizable(true);
	
	setAcceptDrops(true);
}

DSettingsContainer::~DSettingsContainer()
{
	delete d;
}

DCollapsibleWidget* DSettingsContainer::insertWidget( QWidget *w, const QString& name )
{
	if (w && w->layout())
	{
		QLayout *lay = w->layout();
		lay->setMargin(2);
		lay->setSpacing(0);
	}

	DCollapsibleWidget *cw = new DCollapsibleWidget( name );
	
	d->layout->addWidget( cw );
	cw->setInnerWidget( w );
	
	d->collapsibles << cw;
	
	cw->show();
	
	return cw;
}

void DSettingsContainer::removeWidget(QWidget *w )
{
	foreach( DCollapsibleWidget *cw, d->collapsibles )
	{
		if ( cw->innerWidget() == w )
		{
			d->collapsibles.removeAll(cw );
			d->layout->removeWidget(cw);
			
			break;
		}
	}
}

void DSettingsContainer::dragEnterEvent ( QDragEnterEvent * event )
{
	setFocus();
	
	if ( const CollapsibleMimeData *mimeData = dynamic_cast<const CollapsibleMimeData *>(event->mimeData()) )
	{
		if (event->source() == this)
		{
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else
		{
			event->acceptProposedAction();
		}
	}
	else
	{
		event->ignore();
	}
}

void DSettingsContainer::dragMoveEvent(QDragMoveEvent* event)
{
	if ( const CollapsibleMimeData *mimeData = dynamic_cast<const CollapsibleMimeData *>(event->mimeData()) )
	{
		event->acceptProposedAction();
	}
	else
	{
		event->ignore();
	}
}


void DSettingsContainer::dropEvent(QDropEvent* event)
{
	if (const CollapsibleMimeData *mimeData = dynamic_cast<const CollapsibleMimeData *>(event->mimeData()))
	{
		d->layout->removeWidget( mimeData->collapsibleWidget());
		
		
		QWidget *child = childAt(event->pos());
		
		if (child )
		{
			if ( DCollapsibleWidget *prev = dynamic_cast< DCollapsibleWidget *>(child) )
			{
				d->layout->insertWidget( d->layout->indexOf(prev)+1, mimeData->collapsibleWidget());
			}
			else if ( DCollapsibleWidget *prev = dynamic_cast< DCollapsibleWidget *>(child->parentWidget()) )
			{
				d->layout->insertWidget( d->layout->indexOf(prev)+1, mimeData->collapsibleWidget());
			}
			else
			{
				d->layout->addWidget(mimeData->collapsibleWidget());
			}
		}
		else
		{
			d->layout->addWidget(mimeData->collapsibleWidget());
		}
		
		if (event->source() == this) 
		{
			event->setDropAction(Qt::MoveAction);
			event->accept();
		} 
		else 
		{
			event->acceptProposedAction();
		}
	} 
	else 
	{
		event->ignore();
	}
}


DCollapsibleWidget::DCollapsibleWidget(QWidget *parent)
	: QWidget(parent), d(new DCollapsibleWidget::Private)
{
	init();
}
DCollapsibleWidget::DCollapsibleWidget(const QString& caption, QWidget *parent)
	: QWidget(parent), d(new DCollapsibleWidget::Private)
{
	init();
	setCaption(caption);
}

void DCollapsibleWidget::init()
{
	d->innerWidget = 0;
	d->cwlayout = new QGridLayout( this );
	d->cwlayout->setMargin(0);
	
	d->label = new DClickableLabel;
	d->label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
	
	
	d->cwlayout->addWidget(d->label, 1, 1);
	
// 	d->label->show();
	
	connect(d->label, SIGNAL(clicked()), this, SLOT(toggleExpanded()));
	
	setExpanded(false);
	setEnabled(false);
}

DCollapsibleWidget::~DCollapsibleWidget()
{
	delete d;
}

QWidget* DCollapsibleWidget::innerWidget() const
{
	return d->innerWidget;
}

void DCollapsibleWidget::setInnerWidget(QWidget *w)
{
	if (!w) return;
	
	QGroupBox *container = new QGroupBox(this);
	w->setParent(container);
	
	QVBoxLayout *containerLayout = new QVBoxLayout(container);
	d->innerWidget = w;
	
	containerLayout->addWidget(w);

	d->cwlayout->addWidget(container, 2, 1);
	d->cwlayout->setRowStretch(2, 1);
	
	setEnabled( true );
	setExpanded(isExpanded());
}

void DCollapsibleWidget::setCaption(const QString& caption)
{
	d->label->setText(QString("<b>%1</b>").arg(caption));
}

QString DCollapsibleWidget::caption() const
{
	return d->label->text();
}

void DCollapsibleWidget::setExpanded(bool expanded)
{
	if ( d->innerWidget )
	{
		setUpdatesEnabled( false );
		d->innerWidget->parentWidget()->setVisible(expanded);
		d->innerWidget->setVisible(expanded);
		
		d->label->setChecked(expanded);
		
		setUpdatesEnabled( true );
	}
}

bool DCollapsibleWidget::isExpanded() const
{
	return d->label->isChecked();
}

void DCollapsibleWidget::toggleExpanded()
{
	setExpanded( d->label->isChecked() );
}


