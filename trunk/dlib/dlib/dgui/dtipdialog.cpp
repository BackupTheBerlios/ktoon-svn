/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#include "dtipdialog.h"
#include "dalgorithm.h"

#include <QVBoxLayout>

#include <QTextBrowser>
#include <QTextFrame>
#include <QTextFrameFormat>

#include <QCheckBox>
#include <QPushButton>

#include "dseparator.h"

#include <QDomDocument>
#include <QFile>

#include "dconfig.h"
#include "ddebug.h"

DTipDatabase::DTipDatabase(const QString &file)
{
	loadTips( file );
	
	if ( !m_tips.isEmpty() )
	{
		m_currentTipIndex = DAlgorithm::random() % m_tips.count();
	}
}


DTipDatabase::~DTipDatabase()
{
}

DTip DTipDatabase::tip() const
{
	if (m_currentTipIndex >= 0 && m_currentTipIndex < m_tips.count() )
		return m_tips[m_currentTipIndex];
	return DTip();
}

void DTipDatabase::nextTip()
{
	if (m_tips.isEmpty())
		return ;
	m_currentTipIndex += 1;
	if (m_currentTipIndex >= (int) m_tips.count())
	{
		m_currentTipIndex = 0;
	}
}

void DTipDatabase::prevTip()
{
	if (m_tips.isEmpty())
		return ;
	m_currentTipIndex -= 1;
	if (m_currentTipIndex < 0)
	{
		m_currentTipIndex = m_tips.count() - 1;
	}
}

void DTipDatabase::loadTips(const QString &filePath)
{
	QDomDocument doc;
	QFile file(filePath);
	
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}
	
	if (!doc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();
	
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while(!n.isNull())
	{
		QDomElement e = n.toElement();
		if(!e.isNull())
		{
			if ( e.tagName() == "tip" )
			{
				DTip tip;
				tip.text = e.text();
				m_tips << tip;
			}
		}
		n = n.nextSibling();
	}
}

// DTipDialog

DTipDialog::DTipDialog(const QString &file, QWidget *parent) : QDialog(parent)
{
	m_database = new DTipDatabase(file);
	setupGUI();
}

DTipDialog::DTipDialog(DTipDatabase *database, QWidget *parent) : QDialog(parent), m_database(database)
{
	setupGUI();
	
}

void DTipDialog::setupGUI()
{
	setWindowTitle(tr("Tip of day"));
	
	int h,s,v;
	QColor baseColor = palette().base().color();
	baseColor.getHsv(&h,&s,&v);
	baseColor.setHsv(h, int(s*(71/76.0)), int(v*(67/93.0)));
// 	baseColor.setHsv(h, int(s*(10/6.0)), int(v*(93/99.0)));
	
	QVBoxLayout *layout = new QVBoxLayout(this);
	
	m_textArea = new QTextBrowser;
	
	QTextFrameFormat format = m_textArea->document()->rootFrame()->frameFormat();
	format.setMargin(15);
	format.setBorder(5);
	m_textArea->document()->rootFrame()->setFrameFormat(format);
	
	m_textArea->setWordWrapMode( QTextOption::WrapAtWordBoundaryOrAnywhere );
	m_textArea->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
	m_textArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
		
	QPalette pal = m_textArea->palette();
	
	pal.setBrush(QPalette::Base, baseColor);
	
	m_textArea->setPalette(pal);
	
	layout->addWidget(m_textArea);
	
	layout->addWidget(new DSeparator);
	
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	
	m_showOnStart = new QCheckBox(tr("show on start"));
	buttonLayout->addWidget(m_showOnStart);
	connect(m_showOnStart, SIGNAL(clicked()), this, SLOT(setShowOnStart()));
	
	buttonLayout->addStretch(1);
	
	QPushButton *prevTip = new QPushButton(tr("Previous tip"));
	buttonLayout->addWidget(prevTip);
	connect(prevTip, SIGNAL(clicked()), this, SLOT(showPrevTip()));
	
	QPushButton *nextTip = new QPushButton(tr("Next tip"));
	buttonLayout->addWidget(nextTip);
	connect(nextTip, SIGNAL(clicked()), this, SLOT(showNextTip()));
	
	QPushButton *close = new QPushButton(tr("close"));
	buttonLayout->addWidget(close);
	connect(close, SIGNAL(clicked()), this, SLOT(accept()));
	
	
	layout->addLayout(buttonLayout);
	
	setAttribute(Qt::WA_DeleteOnClose, true);
	
	DCONFIG->beginGroup("TipOfDay");
	m_showOnStart->setChecked(qvariant_cast<bool>(DCONFIG->value("ShowOnStart", true ) ));
	
	showNextTip();
}

DTipDialog::~DTipDialog()
{
}

void DTipDialog::showPrevTip()
{
	m_database->prevTip();
	DTip tip = m_database->tip();
	
	m_textArea->setHtml(tip.text);
}

void DTipDialog::showNextTip()
{
	m_database->nextTip();
	DTip tip = m_database->tip();
	
	m_textArea->setHtml(tip.text);
}

void DTipDialog::setShowOnStart()
{
	DCONFIG->beginGroup("TipOfDay");
	DCONFIG->setValue("ShowOnStart", m_showOnStart->isChecked());
}

