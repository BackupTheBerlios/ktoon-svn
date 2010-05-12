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

#include "connectdialog.h"

#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>

#include <QDialogButtonBox>

#include <kcore/kconfig.h>
#include <kgui/kformfactory.h>

class ConnectDialog::Private
{
	public:
		Private() {}
		~Private()
		{
			delete login;
			delete password;
			delete server;
			delete port;
		}
		
		QLineEdit *login;
		QLineEdit *password;
		QLineEdit *server;
		QSpinBox *port;
		
		QCheckBox *storePassword;
		
};

ConnectDialog::ConnectDialog(QWidget *parent) : QDialog(parent), d(new Private())
{
	d->login = new QLineEdit;
	d->password = new QLineEdit;
	d->password->setEchoMode( QLineEdit::Password);
	
	d->server = new QLineEdit;
	d->port = new QSpinBox;
	d->port->setMinimum(1024);
	d->port->setMaximum(65000);
	
	QGridLayout *layout = KFormFactory::makeGrid(QStringList() << tr("Login") << tr("Password") << tr("Server") << tr("Port"), QWidgetList() << d->login << d->password << d->server << d->port);
	
	d->storePassword = new QCheckBox(tr("Store password"));
	layout->addWidget(d->storePassword, 5, 1);
	
	QDialogButtonBox *box = new QDialogButtonBox;
	
	QPushButton *ok = box->addButton(QDialogButtonBox::Ok);
	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	QPushButton *cancel = box->addButton(QDialogButtonBox::Cancel);
	connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	
	mainLayout->addLayout(layout);
	mainLayout->addWidget(box);
	
	setLayout(mainLayout);
	
	loadSettings();
}


ConnectDialog::~ConnectDialog()
{
	saveSettings();
}

void ConnectDialog::setServer(const QString &server)
{
	d->server->setText(server);
}

void ConnectDialog::setPort(int port)
{
	d->port->setValue(port);
}


QString ConnectDialog::login() const
{
	return d->login->text();
}

QString ConnectDialog::password() const
{
	return d->password->text();
}

QString ConnectDialog::server() const
{
	return d->server->text();
}

int ConnectDialog::port() const
{
	return d->port->value();
}


void ConnectDialog::loadSettings()
{
	KCONFIG->beginGroup("Network");
	d->server->setText(KCONFIG->value("server", "localhost").toString());
	d->port->setValue(KCONFIG->value("port", 6502).toInt());
	d->login->setText(KCONFIG->value("login", "").toString());
	d->password->setText(KCONFIG->value("password", "").toString());
	
	d->storePassword->setChecked(KCONFIG->value("storePassword").toInt());
}

void ConnectDialog::saveSettings()
{
	KCONFIG->beginGroup("Network");
	
	KCONFIG->setValue("server", d->server->text() );
	KCONFIG->setValue("port", d->port->value());
	KCONFIG->setValue("login", d->login->text());
	
	if ( d->storePassword->isChecked() )
	{
		KCONFIG->setValue("password", d->password->text());
	}
	else
	{
		KCONFIG->setValue("password", "");
	}
	
	KCONFIG->setValue("storePassword", d->storePassword->isChecked() ? 1 : 0);
	
	KCONFIG->sync();
	
}

