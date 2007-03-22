/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                           	   *
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


#include <QPainter>
#include <QDialog>
#include <QPushButton>
#include <QApplication>
#include <QFile>
#include <QLabel>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QtDebug>
#include <QProcess>

#include "crashwidget.h"
#include "crashhandler.h"

#include <dcore/dconfig.h>
#include <dcore/ddebug.h>


class TextArea : public QTextBrowser
{
	Q_OBJECT
	public:
		TextArea();
		~TextArea();
		
	public slots:
		void setSource ( const QUrl & name );
};

TextArea::TextArea()
{
}

TextArea::~ TextArea()
{
}

void TextArea::setSource( const QUrl &name )
{
	if ( name.scheme() == "http" )
	{
		DCONFIG->beginGroup("General");
		QString browser = DCONFIG->value("Browser").toString();

		if ( !browser.isEmpty() )
		{
			QProcess::startDetached (browser, QStringList() << name.toString() );
		}
	}
	else
	{
		QTextBrowser::setSource(name);
	}
}

#include "crashwidget.moc"

CrashWidget::CrashWidget (int sig) : QDialog(0), m_sig(sig)
{
	setModal(true);
	
	setWindowTitle( CHANDLER->title() );
	
	m_layout = new QVBoxLayout(this);
	
	
	m_tabber = new QTabWidget(this);
	
	m_layout->addWidget(m_tabber);
	
	
	QWidget *page1 = new QWidget;
	
	QVBoxLayout *page1layout = new QVBoxLayout(page1);
	
	QLabel *message = new QLabel("<font color="+CHANDLER->messageColor().name()+">"+ CHANDLER->message()+"</color>");
	
	page1layout->addWidget(message);
	
	
	QHBoxLayout *hbox = new QHBoxLayout;
	
	QString text = CHANDLER->defaultText();
	QImage img(CHANDLER->defaultImage());

	if ( CHANDLER->containsSignalEntry(sig) )
	{
		text = CHANDLER->signalText(sig);
		img = QImage(CHANDLER->signalImage(sig));
	}
	
	QLabel *sigImg = new QLabel;
	sigImg->setPixmap(QPixmap::fromImage(img));
	
	hbox->addWidget(sigImg);
	
	TextArea *sigText = new TextArea();
	sigText->setHtml(text);
	hbox->addWidget(sigText);
	
	page1layout->addLayout(hbox);
	
	m_tabber->addTab(page1, tr("What's happen?"));
	
	QPushButton *end = new QPushButton( CHANDLER->buttonText(),this );
	connect(end,SIGNAL(clicked()),SLOT(accept()));
	m_layout->addWidget(end);
	
	
	setLayout(m_layout);
}


CrashWidget::~CrashWidget ()
{
}

void CrashWidget::addBacktracePage(const QString &execInfo, const QString &backtrace)
{
	D_FUNCINFO << execInfo << " " << backtrace;
	QWidget *btPage = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(btPage);
	
	layout->addWidget(new QLabel(tr("Executable information")));
	
	TextArea *fileInfo = new TextArea;
	
// 	QFontMetrics fm(fileInfo->font());
// 	QSize fileInfoSize = fm.size( Qt::TextWordWrap, execInfo);
// 	fileInfo->setMaximumHeight(fileInfoSize.height());
	
	fileInfo->setHtml(execInfo);
	
	layout->addWidget(fileInfo);
	
	layout->addWidget(new QLabel(tr("Backtrace")));
	TextArea *btInfo = new TextArea;
	btInfo->setHtml(backtrace);
	
	layout->addWidget(btInfo);
	
	m_tabber->addTab(btPage, tr("Backtrace"));
}


