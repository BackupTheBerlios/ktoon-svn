/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "ktnewproject.h"
#include "ktnetprojectmanagerparams.h"

#include <kgui/kformfactory.h>
#include <kcore/kconfig.h>
#include <kgui/kapplication.h>
#include <kgui/kosd.h>

#include <QLineEdit>
#include <QCheckBox>
//TODO anadir un campo para ingresar la descripcion del proyecto 

struct KTNewProject::Private
{
    QLineEdit *projectName;
    QLineEdit *authorName;
    QSpinBox *fps;

    KXYSpinBox *size;
    bool useNetwork;

    QGroupBox *netOptions;

    QLineEdit *server;
    QSpinBox *port;
    QLineEdit *login;
    QLineEdit *password;
};

KTNewProject::KTNewProject(QWidget *parent) : KTabDialog(parent), k( new Private)
{
    setWindowIcon(QPixmap(THEME_DIR + "icons/new.png"));

    k->useNetwork = false;
    setWindowTitle(tr("Create a new project"));
    setModal(true);

    QFrame *container = new QFrame();
    QGridLayout *layout = new QGridLayout(container);

    QLabel *labelProjectName = new QLabel(tr("Project Name"), container);
    layout->addWidget(labelProjectName, 0, 0);

    k->projectName = new QLineEdit( container );
    layout->addWidget(k->projectName, 0, 1);

    QLabel *labelAuthorName = new QLabel(tr("Author"), container);
    layout->addWidget(labelAuthorName, 1, 0);

    k->authorName = new QLineEdit( container );
    layout->addWidget(k->authorName, 1, 1);

    k->size = new KXYSpinBox(tr("Dimension"), container);
    k->size->setMaximum(5000);
    //k->size->setModifyTogether(true);;

    k->size->setX(520);
    k->size->setY(380);
	
    layout->addWidget(k->size, 2, 0);

    QGroupBox *renderAndFps= new QGroupBox(tr("Options"));
	
    QBoxLayout *subLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    renderAndFps->setLayout(subLayout);
	
    QBoxLayout *fpsLayout = new QBoxLayout(QBoxLayout::LeftToRight);

    QLabel *label = new QLabel(tr("FPS"));
    k->fps = new QSpinBox();
    k->fps->setValue(24);

    fpsLayout->addWidget(label);
    fpsLayout->addWidget(k->fps);
    subLayout->addLayout(fpsLayout);

    QWidget *mcont = new QWidget;
    QVBoxLayout *mcontly = new QVBoxLayout(mcont);
    mcontly->addWidget(renderAndFps);

    addTab( container, tr("Project info"));

    setupNetOptions();
    mcontly->addWidget(k->netOptions);

    layout->addWidget(mcont, 2, 1);

    k->netOptions->setVisible(false);

    /*
    QCheckBox *activeNetOptions = new QCheckBox( tr("Create a network project") );
    layout->addWidget(activeNetOptions, 3, 1);
    activeNetOptions->setEnabled(false);

    connect(activeNetOptions, SIGNAL(toggled( bool )), this, SLOT(activateNetOptions(bool)));
    */
}

KTNewProject::~KTNewProject()
{
  /*
    KConfig *config = kApp->config("Network");
    config->setValue("server", k->server->text());
    config->setValue("port", k->port->value());
    config->setValue("login", k->login->text());
    config->setValue("password", k->password->text());
   */
   delete k;
}

void KTNewProject::setupNetOptions()
{
    k->netOptions = new QGroupBox(tr("Network"));
    QVBoxLayout *layout = new QVBoxLayout(k->netOptions);

    k->server = new QLineEdit;
    k->port = new QSpinBox;
    k->port->setMinimum(1024);
    k->port->setMaximum(65000);

    k->login = new QLineEdit;
    k->password = new QLineEdit;

    KConfig *config = kApp->config("Network");

    k->server->setText(config->value("server", "localhost").toString());
    k->port->setValue(config->value("port", 6502).toInt());

    k->login->setText(config->value("login", "").toString());
    k->password->setText(config->value("password", "").toString());

    k->password->setEchoMode( QLineEdit::Password );

    layout->addLayout( KFormFactory::makeGrid( QStringList() << tr("Login") << tr("Password") <<tr("Server") << tr("Port"), QWidgetList() << k->login << k->password << k->server << k->port ) );
}

KTProjectManagerParams *KTNewProject::parameters()
{
    if (k->useNetwork) {
        KTNetProjectManagerParams *params = new KTNetProjectManagerParams;
        params->setProjectName(k->projectName->text());
        params->setAuthor(k->authorName->text());
        const QSize size(k->size->x(),k->size->y());
        params->setDimension(size);
        params->setFPS(k->fps->value());
        // Network settings
        params->setServer(k->server->text());
        params->setPort(k->port->value());
        params->setLogin(k->login->text());
        params->setPassword(k->password->text());

        return params;
    }

    KTProjectManagerParams *params = new KTProjectManagerParams;
    params->setProjectName(k->projectName->text());
    params->setAuthor(k->authorName->text());
    const QSize size(k->size->x(),k->size->y());
    params->setDimension(size);
    params->setFPS(k->fps->value());

    return params;
}

bool KTNewProject::useNetwork() const
{
    return k->useNetwork;
}

void KTNewProject::ok()
{
    if (k->projectName->text().isEmpty()) {
        KOsd::self()->display(tr("Error"), tr("Please fill the project name field"), KOsd::Error);
        return;
    }
    KTabDialog::ok();
}

void KTNewProject::activateNetOptions(bool isVisible)
{
    k->netOptions->setVisible(isVisible);
    k->useNetwork = isVisible;
}

