/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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
#include "ktapplication.h"
#include "configwizard.h"
#include <iostream>
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>

KTApplication::KTApplication(int & argc, char ** argv)
	: QApplication(argc, argv, true), m_VERSION("0.8alpha")
{
	m_KTOON_HOME = getenv("HOME")+QString("/")+name()+QString("_")+m_VERSION ;
	m_KTOON_REPOSITORY = m_KTOON_HOME+QString("/repository");
	parseArgs(argc, argv);
	setStyle("plastik");
}


KTApplication::~KTApplication()
{
}

void KTApplication::parseArgs(int &argc, char **argv)
{
	for(uint i = 0; i < argc; i++)
	{
		QString opt = QString(argv[i]).simplifyWhiteSpace();
		if ( opt.startsWith("--") )
		{
			QString arg = "";
			if ( argv[i+1] && ! QString(argv[i+1]).startsWith("-") )
				arg = QString(argv[i+1]).simplifyWhiteSpace();
			m_parseArgs.insert(opt.remove(0,2), arg);
		}
		else if (opt.startsWith("-") )
		{
			QString arg = "";
			if ( argv[i+1] && ! QString(argv[i+1]).startsWith("-") )
				arg = QString(argv[i+1]).simplifyWhiteSpace();
			
			m_parseArgs.insert(opt.remove(0,1), arg );
		}
	}
}

bool KTApplication::isArg(const QString &arg)
{
	return m_parseArgs.keys().contains(arg);
}

QString KTApplication::getParam(const QString &arg)
{
	if ( ! m_parseArgs.contains(arg) )
		return "";
	return m_parseArgs[arg];
}

QString KTApplication::getHome()
{
	return m_KTOON_HOME;
}

void KTApplication::setHome(const QString &home)
{
	m_KTOON_HOME = home;
}

QString KTApplication::getRepository()
{
	return m_KTOON_REPOSITORY;
}

void KTApplication::setRepository(const QString &repos)
{
	m_KTOON_REPOSITORY = repos;
}


QString KTApplication::getVersion()
{
	return m_VERSION;
}

bool KTApplication::firstRun()
{
	ConfigWizard firstDialog;
#ifdef KTHOME
	firstDialog.setInitialData(KTHOME, QString(KTHOME)+"/projects");
#endif
	if ( firstDialog.exec() != QDialog::Rejected )
	{
		m_KTOON_HOME = firstDialog.getHome();
		m_KTOON_REPOSITORY = firstDialog.getRepos();
		return true;
	}
	
	return false;
}

void KTApplication::initRepository()
{
	QDir repos (m_KTOON_REPOSITORY);
	if ( ! repos.exists() )
	{
		if ( ! repos.mkdir(m_KTOON_REPOSITORY) )
			std::cerr << "Error: I cannot create the repository" << std::endl;
		
		QStringList files = QStringList() << "bru" << "cpl" << "output" << "lbr" << "components";
		
		for (uint i = 0; i < files.count(); i++)
		{
			QDir tmp(m_KTOON_REPOSITORY+"/"+files[i]+"/");
			
			if ( ! tmp.exists() )
				tmp.mkdir(m_KTOON_REPOSITORY+"/"+files[i]+"/");
		}
	}
}




