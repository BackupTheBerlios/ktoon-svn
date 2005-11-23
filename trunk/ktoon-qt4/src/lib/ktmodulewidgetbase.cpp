/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#include "ktmodulewidgetbase.h"
#include <qtooltip.h>
#include <q3dockarea.h>
//Added by qt3to4:
#include <QEvent>
#include <QBoxLayout>
#include <QDialog>

#include "ktdebug.h"

#include <qobject.h>

KTModuleWidgetBase::KTModuleWidgetBase(QWidget *parent, const char *name) : QWidget(parent), m_isChildHidden(false), m_originalParent(parent)
{
	setObjectName(name);
	
// 	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	
	m_container = new QVBoxLayout(this);
	
	m_title = new KTModuleWidgetTitle("", this, "DialogTitle");
	setMinimumSize(m_title->size());
	
	QToolTip::add(m_title, tr("Double click for roll up"));
	
	m_container->addWidget(m_title);
	m_container->setAlignment(m_title, Qt::AlignTop);
// 	m_container->setDirection ( QBoxLayout::TopToBottom);
	m_container->setMargin(5);
	m_container->setSpacing(1);
// 	m_container->setSizeConstraint(QLayout::SetFixedSize);

	
	connect(m_title, SIGNAL(doubleClicked()), SLOT(toggleView()));
	
// 	connect(this, SIGNAL(placeChanged(QDockWindow::Place)), SLOT(fixPosition(QDockWindow::Place)));
	
	adjustSize();
	hide();
}


KTModuleWidgetBase::~KTModuleWidgetBase()
{
}

void KTModuleWidgetBase::addChild(QWidget* child, Qt::Alignment alignment)
{
	m_childs.append(child);
	
	m_container->invalidate();
	m_container->addWidget(child, 0,alignment);
}

void KTModuleWidgetBase::toggleView()
{
#if 1
	if ( ! m_isChildHidden )
	{
		QPoint position = m_title->pos();
		setParent(0, Qt::WindowStaysOnTopHint );
		move(mapToGlobal(position));
		
		show();
	}
#endif
	
// 	m_title->setMinimumWidth(m_title->width());
// 	for( int i = 0; i < m_childs.count(); i++)
// 	{
// 		QObject *o = m_childs[i];
// 		if ( o && ! m_isChildHidden )
// 		{
// 			static_cast<QWidget*>(o)->hide();
// 		}
// 		else if ( o && m_isChildHidden )
// 		{
// 			static_cast<QWidget*>(o)->show();
// 		}
// 	}
// 	
// 	if ( ! m_isChildHidden )
// 	{
// 		setMinimumSize(m_title->size());
// 		resize(m_title->size());
// 	}
// 	else
// 	{
// 		adjustSize();
// 	}
	
// 	m_isChildHidden = !m_isChildHidden;
}

void KTModuleWidgetBase::setCaption(const QString &text)
{
 	m_title->setText(text);
}

void KTModuleWidgetBase::setFont( const QFont &f)
{
	QWidget::setFont(f);
	adjustSize();
}

bool KTModuleWidgetBase::event( QEvent * e )
{
	if ( e->type() == QEvent::Hide )
	{
		emit activate(false);
	}
	else if ( e->type() == QEvent::Show )
	{
		emit activate(true);
	}

	return QWidget::event(e );
}

void KTModuleWidgetBase::enterEvent(QEvent *e)
{
	QPalette pal = palette();
	pal.setColor(QPalette::Background, pal.highlight ());
	pal.setColor(QPalette::Text, pal.base ());
	m_title->setPalette(pal);
}

void KTModuleWidgetBase::leaveEvent(QEvent *e)
{
	QPalette pal = palette();
	m_title->setPalette(pal);
}

