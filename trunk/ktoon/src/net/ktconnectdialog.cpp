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

#include "ktconnectdialog.h"

#include "dformfactory.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>

#include "ktnetprojectmanagerparams.h"

#include <QDialogButtonBox>

#include <dconfig.h>

class KTConnectDialog::Private
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

KTConnectDialog::KTConnectDialog(QWidget *parent) : QDialog(parent), d(new Private())
{
	d->login = new QLineEdit;
	d->password = new QLineEdit;
	d->password->setEchoMode( QLineEdit::Password);
	
	d->server = new QLineEdit;
	d->port = new QSpinBox;
	d->port->setMinimum(1024);
	d->port->setMaximum(65000);
	
	QGridLayout *layout = DFormFactory::makeGrid(QStringList() << tr("Login") << tr("Password") << tr("Server") << tr("Port"), QWidgetList() << d->login << d->password << d->server << d->port);
	
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


KTConnectDialog::~KTConnectDialog()
{
	saveSettings();
}

void KTConnectDialog::setServer(const QString &server)
{
	d->server->setText(server);
}

void KTConnectDialog::setPort(int port)
{
	d->port->setValue(port);
}


QString KTConnectDialog::login() const
{
	return d->login->text();
}

QString KTConnectDialog::password() const
{
	return d->password->text();
}

QString KTConnectDialog::server() const
{
	return d->server->text();
}

int KTConnectDialog::port() const
{
	return d->port->value();
}


void KTConnectDialog::loadSettings()
{
	DCONFIG->beginGroup("Network");
	d->server->setText(DCONFIG->value("server", "localhost").toString());
	d->port->setValue(DCONFIG->value("port", 6502).toInt());
	d->login->setText(DCONFIG->value("login", "").toString());
	d->password->setText(DCONFIG->value("password", "").toString());
	
	d->storePassword->setChecked(DCONFIG->value("storePassword").toInt());
}

void KTConnectDialog::saveSettings()
{
	DCONFIG->beginGroup("Network");
	
	DCONFIG->setValue("server", d->server->text() );
	DCONFIG->setValue("port", d->port->value());
	DCONFIG->setValue("login", d->login->text());
	
	if ( d->storePassword->isChecked() )
	{
		DCONFIG->setValue("password", d->password->text());
	}
	else
	{
		DCONFIG->setValue("password", "");
	}
	
	DCONFIG->setValue("storePassword", d->storePassword->isChecked() ? 1 : 0);
	
	DCONFIG->sync();
	
}

