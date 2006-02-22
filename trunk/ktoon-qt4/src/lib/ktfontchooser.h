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

#ifndef KTFONTCHOOSER_H
#define KTFONTCHOOSER_H

#include <QFrame>

class QComboBox;

/**
 * @if english
 * @short translate me
 * @elseif spanish
 * @short Esta clase provee de una interfaz grafica para seleccionar una "fuente" apartir de: la familia, el estilo y su tamaño.
 * @endif
 * @author David Cuadrado <krawek@toonka.com>
 * @see QFont
 */
class KTFontChooser : public QFrame
{
	Q_OBJECT
	public:
		/**
		 * @if english
		 * translate me
		 * @elseif spanish
		 * Constructor por defecto.
		 * @endif
		 */
		KTFontChooser(QWidget *parent = 0);
		/**
		 * Destructor
		 */
		~KTFontChooser();
		/**
		 * @if english
		 * translate me
		 * @elseif spanish
		 * Pone el una "fuente" para ser escogida.
		 * @endif
		 */
		void setCurrentFont(const QFont &font);
		/**
		 * @if english
		 * translate me
		 * @elseif spanish
		 * Retorna la "fuente" actualmente seleccionada.
		 * @endif
		 */
		QFont font() const;
		
	signals:
		/**
		 * @if english
		 * translate me
		 * @elseif spanish
		 * Es emitido cuando la fuente es cambiada.
		 * @endif
		 */
		void fontChanged();
		
	private slots:
		void emitFontChanged(int =0);
		
	public slots:
		/**
		 * @if english
		 * translate me
		 * @elseif spanish
		 * Carga la informacion de una familia.
		 * @endif
		 */
		void loadFontInfo(const QString &family);
		
	private:
		QComboBox *m_families;
		QComboBox *m_fontStyle;
		QComboBox *m_fontSize;
};

#endif
