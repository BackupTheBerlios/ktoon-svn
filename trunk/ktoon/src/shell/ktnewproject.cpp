/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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

#include "ktnewproject.h"

#include "ktnetprojectmanagerparams.h"
#include <dgui/dformfactory.h>
#include <dcore/dconfig.h>
#include <dgui/dapplication.h>
#include <dgui/dosd.h>

#include <QLineEdit>
#include <QCheckBox>
//TODO añadir un campo para ingresar la descripcion del proyecto 

struct KTNewProject::Private
{
		QLineEdit *projectName;
		QLineEdit *authorName;
		QSpinBox *fps;
		
		DXYSpinBox *size;
		
		bool useNetwork;
		
		QGroupBox *netOptions;
		
		QLineEdit *server;
		QSpinBox *port;
		QLineEdit *login;
		QLineEdit *password;
};

KTNewProject::KTNewProject(QWidget *parent)
	: DTabDialog(parent), d( new Private)
{
	d->useNetwork = false;
	setWindowTitle(tr("Create a new project"));
	setModal(true);
	
	QFrame *container = new QFrame();
	QGridLayout *layout = new QGridLayout(container);
	
	QLabel *labelProjectName = new QLabel(tr("Project Name"), container);
	layout->addWidget(labelProjectName, 0, 0);
	
	d->projectName = new QLineEdit( container );
	layout->addWidget(d->projectName, 0, 1);
	
	QLabel *labelAuthorName = new QLabel(tr("Author"), container);
	layout->addWidget(labelAuthorName, 1, 0);
	
	d->authorName = new QLineEdit( container );
	layout->addWidget(d->authorName, 1, 1);
	
	d->size = new DXYSpinBox(tr("Dimension"), container);
	d->size->setMaximum( 1000);
	d->size->setModifyTogether(true);;
	
	d->size->setX( 520 );
	d->size->setY( 340);
	
	layout->addWidget(d->size, 2, 0);
	
	QGroupBox *renderAndFps= new QGroupBox(tr("Options"));
	
	QBoxLayout *subLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	renderAndFps->setLayout(subLayout);
	
	QBoxLayout *fpsLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	
	QLabel *label = new QLabel(tr("FPS"));
	d->fps = new QSpinBox();
	d->fps->setValue(24);
	
	fpsLayout->addWidget(label);
	fpsLayout->addWidget(d->fps);
	subLayout->addLayout(fpsLayout);
	
	QWidget *mcont = new QWidget;
	QVBoxLayout *mcontly = new QVBoxLayout(mcont);
	
	mcontly->addWidget(renderAndFps);
	addTab( container, tr("Project info"));
	
	setupNetOptions();
	mcontly->addWidget(d->netOptions);
	
	layout->addWidget( mcont, 2, 1);
	
	d->netOptions->setVisible(false);
	
	QCheckBox *activeNetOptions = new QCheckBox( tr("Create a network project") );
	layout->addWidget( activeNetOptions, 3, 1);
	
	connect(activeNetOptions, SIGNAL(toggled( bool )), this, SLOT(activateNetOptions(bool)));
}

KTNewProject::~KTNewProject()
{
	DConfig *config = dApp->config("Network");
	
	config->setValue("server", d->server->text());
	config->setValue("port", d->port->value());
	config->setValue("login", d->login->text());
	config->setValue("password", d->password->text());
}

void KTNewProject::setupNetOptions()
{
	d->netOptions = new QGroupBox(tr("Network"));
	QVBoxLayout *layout = new QVBoxLayout(d->netOptions);
	
	d->server = new QLineEdit;
	d->port = new QSpinBox;
	d->port->setMinimum(1024);
	d->port->setMaximum(65000);
	
	d->login = new QLineEdit;
	d->password = new QLineEdit;
	
	DConfig *config = dApp->config("Network");
	
	d->server->setText(config->value("server", "localhost").toString());
	d->port->setValue(config->value("port", 6502).toInt());
	
	d->login->setText(config->value("login", "").toString());
	d->password->setText(config->value("password", "").toString());
	
	d->password->setEchoMode( QLineEdit::Password );
	
	layout->addLayout( DFormFactory::makeGrid( QStringList() << tr("Login") << tr("Password") <<tr("Server") << tr("Port"), QWidgetList() << d->login << d->password << d->server << d->port ) );
}

KTProjectManagerParams *KTNewProject::params()
{
	if ( d->useNetwork )
	{
		KTNetProjectManagerParams *params = new KTNetProjectManagerParams;
		params->setProjectName( d->projectName->text() );
		params->setAuthor( d->authorName->text());
		params->setServer(d->server->text());
		params->setPort(d->port->value());
		
		params->setLogin(d->login->text());
		params->setPassword(d->password->text());
		
		
		return params;
	}
	
	KTProjectManagerParams *params = new KTProjectManagerParams;
	
	params->setProjectName(d->projectName->text());
	params->setAuthor( d->authorName->text());
	return params;
}

bool KTNewProject::useNetwork() const
{
	return d->useNetwork;
}

void KTNewProject::ok()
{
	if(d->projectName->text().isEmpty())
	{
		DOsd::self()->display(tr("Please fill the project name field"), DOsd::Error);
		return;
	}
	DTabDialog::ok();
}

void KTNewProject::activateNetOptions(bool no)
{
	d->netOptions->setVisible(no);
	d->useNetwork = no;
}

