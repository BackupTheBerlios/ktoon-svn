//
// C++ Implementation: kttabdialog
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "kttabdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>

KTTabDialog::KTTabDialog(QWidget *parent)
 : QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;

	m_tabWidget = new QTabWidget(this);
	
	
	QPushButton *okButton = new QPushButton(tr("OK"));
	QPushButton *cancelButton = new QPushButton(tr("Cancel"));

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);

	mainLayout->addWidget(m_tabWidget);
	mainLayout->addLayout(buttonLayout);
	setLayout(mainLayout);
}


KTTabDialog::~KTTabDialog()
{
}

void KTTabDialog::addTab ( QWidget * child, const QString & label )
{
	m_tabWidget->addTab(child, label);
}

void KTTabDialog::addTab ( QWidget * child, const QIcon & iconset, const QString & label )
{
	m_tabWidget->addTab(child, iconset, label);
}

