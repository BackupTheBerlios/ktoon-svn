/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "ktchat.h"

#include <QTextBrowser>
#include <QLineEdit>

#include <QGridLayout>

class KTChat::Private
{
	public:
		Private()
		{
		}
		
		~Private()
		{
			delete lineEdit;
			delete browser;
		}
		
		QLineEdit *lineEdit;
		QTextBrowser *browser;
};

KTChat::KTChat(QWidget *parent) : QDialog(parent), d(new Private)
{
	QGridLayout *layout = new QGridLayout(this);
	
	
	d->browser = new QTextBrowser;
	layout->addWidget(d->browser, 0, 1);
	
	d->lineEdit = new QLineEdit;
	layout->addWidget(d->lineEdit, 1, 1);
	
	
	connect(d->lineEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
}


KTChat::~KTChat()
{
	delete d;
}


void KTChat::addMessage(const QString &from, const QString &message)
{
	d->browser->append(QString("<%1> %2").arg(from).arg(message));
}

void KTChat::sendMessage()
{
	QString text = d->lineEdit->text();
	d->lineEdit->clear();
	
	emit requestSendMessage(text);
}

