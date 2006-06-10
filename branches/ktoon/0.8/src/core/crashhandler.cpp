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

#include <QString>
#include <QApplication>
#include <QDomDocument>
#include <QFile>
#include <QTemporaryFile>

#include <csignal>
#include <cstdio>

#ifdef Q_OS_UNIX
extern "C"
{
#include <sys/types.h> //pid_t
#include <sys/wait.h>  //waitpid
#include <unistd.h>    //write, getpid
}
#endif

#include "crashhandler.h"
#include "crashwidget.h"

#include "ddebug.h"
#include "dglobal.h"

CrashHandler *CrashHandler::m_instance = 0;

void crashTrapper (int sig);

CrashHandler::CrashHandler() : m_verbose(false)
{
	m_program = QCoreApplication::applicationName();
	setTrapper(crashTrapper);
	
	m_config.title = QObject::tr("Fatal error");
	m_config.message = QObject::tr("%1 is crashing...").arg( m_program );
	m_config.buttonText = QObject::tr("Close");
	m_config.defaultText = QObject::tr("This is a general failure");
}


CrashHandler::~CrashHandler ()
{
	if ( m_instance ) delete m_instance;
}

CrashHandler *CrashHandler::instance()
{
	init();
	return m_instance;
}
			
void CrashHandler::init()
{
	if ( m_instance == 0 )
	{
		m_instance = new CrashHandler();	
	}
}

void CrashHandler::setTrapper (void (*trapper)(int))
{
#ifdef Q_OS_UNIX


	if (!trapper)
	{
		trapper = SIG_DFL;
	}
	sigset_t mask;
	sigemptyset(&mask);
	
	
	signal(SIGSEGV,trapper);
	sigaddset(&mask, SIGSEGV);
	
	signal(SIGFPE,trapper);
	
	signal(SIGILL,trapper);
	sigaddset(&mask, SIGILL);
	
	signal (SIGABRT, trapper);
	sigaddset(&mask, SIGABRT);
	
	signal(SIGBUS,trapper);
	signal(SIGIOT,trapper);
	
	sigprocmask(SIG_UNBLOCK, &mask, 0);
#endif
}


QString CrashHandler::program () const
{
	return m_program;
}


void CrashHandler::setProgram (const QString &prog)
{
	m_program = prog;
}

void CrashHandler::setImagePath(const QString &imagePath)
{
	m_imagePath = imagePath;
}

QString CrashHandler::imagePath() const
{
	return m_imagePath;
}

QString CrashHandler::title() const
{
	return m_config.title;
}

QString CrashHandler::message() const
{
	return m_config.message;
}

QColor CrashHandler::messageColor() const
{
	if( m_config.messageColor.isValid() )
	{
		return m_config.messageColor;
	}
	
	return QApplication::palette().color(QPalette::Text);
}

QString CrashHandler::buttonText() const
{
	return m_config.buttonText;
}

QString CrashHandler::defaultText() const
{
	return m_config.defaultText;
}

QString CrashHandler::defaultImage() const
{
	return m_imagePath +"/"+m_config.defaultImage;
}


QString CrashHandler::signalText(int signal)
{
	return m_config.signalEntry[signal].first;
}

QString CrashHandler::signalImage(int signal)
{
	return m_imagePath +"/"+m_config.signalEntry[signal].second;
}

bool CrashHandler::containsSignalEntry(int signal)
{
	return m_config.signalEntry.contains(signal);
}

void CrashHandler::setConfig(const QString &filePath)
{
	D_FUNCINFO;
	SHOW_VAR(filePath);
	QDomDocument doc;
	QFile file(filePath);
	
	if (!file.open(QIODevice::ReadOnly))
		return;
	if (!doc.setContent(&file)) 
	{
		file.close();
		return;
	}
	file.close();
	
	QDomElement docElem = doc.documentElement();
	
	if ( docElem.tagName() == "CrashHandler" )
	{
		QDomNode n = docElem.firstChild();
		while( !n.isNull() ) 
		{
			QDomElement e = n.toElement();
			if(!e.isNull()) 
			{
				if ( e.tagName() == "Title" )
				{
					m_config.title = e.attribute("text");
				}
				else if ( e.tagName() == "Message" )
				{
// 					<Message text="str" color="red" />
					m_config.message = e.attribute("text");
					m_config.messageColor = QColor(e.attribute("color"));
				}
				else if ( e.tagName() == "Button" )
				{
// 					<Button text="Ok" />
					m_config.buttonText = e.attribute("text");
				}
				else if ( e.tagName() == "Default" )
				{
					m_config.defaultText = e.attribute("text");
					m_config.defaultImage = e.attribute("image");
				}
				else if ( e.tagName() == "Signal" )
				{
					int signalId = e.attribute("id").toInt();
					m_config.signalEntry.insert(signalId, qMakePair(e.attribute("text"), e.attribute("image")));
				}
			}
			n = n.nextSibling();
		}
	}
}

static QString runCommand( const QString &command )
{
	static const uint SIZE = 40960; //40 KiB
	static char buf[ SIZE ];
	
// 	dDebug() << "Running: " << command;
	
	FILE *process = ::popen( command.toLocal8Bit().data(), "r" );
	buf[ std::fread( (void*)stdout, sizeof(char), SIZE-1, process ) ] = '\0';
	::pclose( process );

	return QString::fromLocal8Bit( buf );
}


void crashTrapper (int sig)
{
	qDebug("%s is crashing with signal %d :(", CHANDLER->program().toLatin1().data(), sig);
	
	CHANDLER->setTrapper(0); // Unactive crash handler
	
	bool isActive = true;
	
	QApplication* application = qApp;
// 	if (!qApp)
	{
		isActive = false;
		int argc = 1;
		char *argv[] = { CHANDLER->program().toUtf8().data(), 0  };
		application = new QApplication(argc, argv);
	}
	
	const pid_t pid = ::fork();
	
	if( pid <= 0 )
	{
		QString bt;
		QString execInfo;
		
		{
			QTemporaryFile temp("qt_temp_file");
			temp.setAutoRemove( true );
			
			temp.open();
			
			const int handle = temp.handle();
	
			const QString gdb_batch = "bt\n";
	
			::write( handle, gdb_batch.toLatin1().data(), gdb_batch.length() );
			::fsync( handle );
	
			// so we can read stderr too
			::dup2( fileno( stdout ), fileno( stderr ) );
		
			QString gdb;
			gdb  = "gdb --nw -n --batch -x ";
			gdb += temp.fileName();
			gdb += HOME+"/bin/ktoon";
			gdb += QString::number( ::getppid() );
	
			bt = runCommand( gdb );
		}
		
		/// clean up
		bt.remove( QRegExp("\\(no debugging symbols found\\)") );
		bt = bt.simplified();

		execInfo = runCommand( "file "+HOME+"/bin/ktoon");
		
		/////// Widget
		CrashWidget widget(sig);
	
		widget.addBacktracePage( execInfo, bt );
	
		widget.exec();
	
		if ( !isActive )
		{
			application->exec();
		}
		
		::exit(255);
	}
	else
	{
		// Process crashed!
		::alarm( 0 );
		// wait for child to exit
		::waitpid( pid, NULL, 0 );
	}
	
	exit(128);
}

