/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#ifndef DAPPLICATION_H
#define DAPPLICATION_H

#include <QApplication>
#include <QMap>
#include <QColorGroup>

#include "dgui/dthememanager.h"
#include "dcore/dconfig.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QDialog>

#include "dgui/dvhbox.h"
#include "dcore/dglobal.h"

#include "dgui/dwizard.h"



class QApplication;
class QString;
class DThemeDocument;
class QPalette;
class DActionManager;
class DAction;

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
class D_GUI_EXPORT DApplication : public QApplication
{
	Q_OBJECT
	public:
		enum ColorSchema { DarkBlue };
		/**
		 * @if english
		 * Translate
		 * @endif
		 * @if spanish
		 * Construye DApplication con los argumentos y el numero de argumentos que recibe la aplicacion por la linea de comandos
		 * @endif
		 * @param argc 
		 * @param argv 
		 * @return 
		 */
		DApplication(int & argc, char ** argv);
		
		
		/**
		 * Destructor
		 * @return 
		 */
		~DApplication();
		
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
		 * @param dd 
		 */
		void applyTheme(const DThemeDocument &dd);
		
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
		
		
		DConfig *config(const QString &group = "General");
		
		
		bool insertGlobalAction(QAction *action, const QString& id);
		void removeGlobalAction(QAction *action);
		QAction *findGlobalAction(const QString &id);
		
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
		
	private:
		ParseArgs m_parseArgs;
		DThemeManager m_themeManager;
		DActionManager *m_actionManager;
};

#include <dcore/dglobal.h>

#define dApp static_cast<DApplication *>(qApp)

#endif
