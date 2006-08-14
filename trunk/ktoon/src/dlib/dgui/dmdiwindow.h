/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#ifndef DMDIWINDOW_H
#define DMDIWINDOW_H

#include <QMainWindow>
#include <QWorkspace>
#include <dcore/dglobal.h>

/**
 * @if english
 * This class is used like internal window in the workspace
 * @elseif spanish
 * Esta clase es usada como ventana interna de los espacios de trabajo
 * @endif
 * @author Jorge Cuadrado
*/
class D_GUI_EXPORT DMdiWindow : public QMainWindow
{
	Q_OBJECT
	public:
		/**
		 * @if english
		 * Default Constructor 
		 * @elseif spanish
		 * Constructor por defecto
		 * @endif
		 * @param parent 
		 * @param name 
		 * @return 
		 */
		DMdiWindow(QWorkspace* parent = 0, const char* name = 0);
		/**
		 * Destructor
		 * @return 
		 */
		~DMdiWindow();
		
		
		/**
		 * @if english
		 * Returns the workspace associated to window
		 * @elseif spanish
		 * Retorna el espacio de trabajo asociado a la ventana
		 * @endif
		 * @return 
		 */
		QWorkspace* workspace();
		
	protected:
		virtual bool event( QEvent * e );
	
	signals:
		/**
		 * @if english
		 * 
		 * @elseif spanish
		 * 
		 * @endif
		 * @param  
		 * @param ms 
		 */
		void sendMessage(const QString &, int ms = 0);
		
		
		/**
		 * 
		 * @param step 
		 * @param totalSteps 
		 */
		void sendProgress(int step, int totalSteps);
		/**
		 * @if english
		 * This signal is emitted when the window is activated
		 * @elseif spanish
		 * Esta señal es emitida cuando la ventana es activada
		 * @endif
		 * @param  
		 */
		void activate(bool isVisible);
		
	private:
		QWorkspace *m_workspace;
};

#endif
