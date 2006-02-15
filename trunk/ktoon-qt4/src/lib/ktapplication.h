/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#ifndef KTAPPLICATION_H
#define KTAPPLICATION_H

#include <QApplication>
#include <QMap>
#include <QColorGroup>

#include "ktthememanager.h"
#include "ktconfig.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QDialog>

#include "ktvhbox.h"

#include "ktwizard.h"

class QApplication;
class QString;
class KTThemeDocument;
class QPalette;

typedef QMap<QString, QString> ParseArgs;

/**
 * @if english
 * Translate
 * @endif
 * @if spanish
 * Esta clase sirve para controlar y proveer informacion a las demas clases y modulos de la aplicacion
 * @endif
 * 
 * @author David Cuadrado
 * @TODO
 * @li We need have a cache directory (like /tmp in un*x)
*/
class Q_GUI_EXPORT KTApplication : public QApplication
{
	Q_OBJECT
	public:
		enum ColorSchema { Default = 0, DarkBlue };
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Construye KTApplication con los argumentos y el numero de argumentos que recibe la aplicacion por la linea de comandos
		 * @endif
		 * @param argc 
		 * @param argv 
		 * @return 
		 */
		KTApplication(int & argc, char ** argv);
		
		
		/**
		 * Destructor
		 * @return 
		 */
		~KTApplication();
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Retorna la ruta a el directorio donde esta instalada la aplicacion
		 * @endif
		 * @return 
		 */
		QString home();
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Pone el directorio donde esta instalada la aplicacion
		 * @endif
		 * @param home 
		 */
		void setHome(const QString &home);
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Retorna la ruta al directorio de themes
		 * @endif
		 * @return 
		 */
		QString themeDir() const;
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Pone el directorio de themes
		 * @endif
		 * @param themeDir 
		 */
		void setThemeDir(const QString &themeDir);
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Retorna la ruta al directorio de datos
		 * @endif
		 * @return 
		 */
		QString dataDir() const;
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Retorna la ruta al repositorio de proyectos
		 * @endif
		 * @return 
		 */
		QString repository();
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Crea un repositorio en la ruta especificada por repository
		 * @endif
		 * @param repository 
		 */
		void createRepository(const QString &repository);
		
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Retorna la version de la aplicacion
		 * @endif
		 * @return 
		 */
		QString version();
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Analiza los argumentos de entrada
		 * @endif
		 * @param argc 
		 * @param argv 
		 */
		void parseArgs( int &argc, char **argv);
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Aplica colores predefinidos
		 * @endif
		 * @param cs 
		 */
		void applyColors(ColorSchema cs);
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Aplica una paleta a toda la aplicacion
		 * @endif 
		 * @param p 
		 */
		void applyPalette( const QPalette &p );
		
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Aplica un theme a toda la aplicacion desde una ruta
		 * @endif 
		 * @param file 
		 */
		void applyTheme(const QString &file);
		
		/**
		 * * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Aplica un theme a toda la aplicacion desde un documento XML
		 * @endif
		 * @param ktd 
		 */
		void applyTheme(const KTThemeDocument &ktd);
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Esta funcion retorna verdadero si el argumento fue suministrado por la linea de comandos
		 * @endif 
		 * @param arg 
		 * @return 
		 */
		bool isArg(const QString &arg);
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Obtiene el parametro suministrado a un argumento
		 * @endif
		 * @param arg 
		 * @return 
		 */
		QString getParam(const QString &arg);
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Cambia la letra de toda la aplicacion
		 * @endif
		 * @param font 
		 */
		void changeFont(const QFont &font); // static?
		
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Retorna la ruta al directorio de configuracion
		 * @endif
		 * @return 
		 */
		QString configDir();
		
	public slots:
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Lanza un wizard de configuracion la primera vez que se inicia la aplicacion
		 * @endif
		 * @return 
		 */
		virtual bool firstRun();
		
	protected:
// 		virtual void detectOpengl();
		
	private:
		QString m_KTOON_HOME;
		QString m_THEME_DIR;
		QString m_KTOON_REPOSITORY;
		QString m_configDir;
		const QString m_VERSION;
		ParseArgs m_parseArgs;
		KTThemeManager m_themeManager;
		bool m_haveDri;
};

class CWFirstPage;
class CWSecondPage;

/**
 * @if english
 * This class helps to config KToon!
 * @endif
 * @if spanish
 * @brief la clase ConfigWizard provee el dialogo para configurar los directorios principales de ktoon.
 * El home que es el directorio donde esta instalado ktoon y el repositorio que es el directorio en donde se guardaran todos los proyectos del usuario
 * 
 * @endif
 * @author David Cuadrado
 */
class ConfigWizard : public KTWizard
{
	Q_OBJECT
	public:
    		/**
	         * Crea el dialogo configuracion.
		 */
		ConfigWizard();
    		/**
		 * destuctor
		 */
		~ConfigWizard();
		
		/**
		 * Pone los directorios por defecto en el dialogo.
		 * @param home 
		 * @param repos 
		 */
		void setInitialData(const QString &home, const QString &repos);
		
		/**
		 * @return la ruta del directorio home
		 */
		QString home();
		
		/**
		 * @return la ruta del directorio repository
		 */
		QString repository();

	private:
		CWFirstPage *m_firstPage;
		CWSecondPage *m_secondPage;
		
		QString kthome;
		QString ktrepos;
};

class CWFirstPage : public KTWizardPage
{
	Q_OBJECT
	public:
		CWFirstPage(QWidget *parent = 0);
		~CWFirstPage();

		bool isComplete() { return true; };
		void reset() { };
};

class CWSecondPage : public KTWizardPage
{
	Q_OBJECT
	public:
		CWSecondPage(QWidget *parent = 0);
		~CWSecondPage();
		void setData(const QString &home, const QString &repos);
		QString home();
		QString repository();
		
		bool isComplete();
		void reset();
		
	private slots:
		void verify(const QString &);
		
	private:
		QLineEdit *m_kthome;
		QLineEdit *m_ktrepos;
};

#include "ktglobal.h"

#endif
