/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   				   *
 *   krawek@gmail.com   						   *
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

#ifndef DANIMWIDGET_H
#define DANIMWIDGET_H
/**
 * @file danimwidget.h
 * @brief Include this file if you need the class DAnimWidget
 */

#include <QPixmap>
#include <QHideEvent>
#include <QShowEvent>
#include <QList>
#include <QWidget>
#include "dcore/dglobal.h"

typedef QList<QPixmap> ListOfPixmaps;

/**
 * @if english
 * @short Translate
 * @elseif spanish
 * Si el es de texto lo va desplazando de arriba abajo hacia arriba y si es de imgenes las va mostrando una por una.
 * @short La clase DAnimWidget provee de un widget que hace una simple animacion, de un texto o una secuencia de imagenes.
 * 
 * @Author David Cuadrado
 * @endif
 */
class D_GUI_EXPORT DAnimWidget : public QWidget
{
	public:
		enum Type { AnimText = 0, AnimPixmap };
		/**
		 * Construye un DAnimWidget con una imagen de fondo, un texto para animar y un padre.
		 * @param px imagen de fondo
		 * @param text texto para animar
		 * @param parent 
		 */
		DAnimWidget(const QPixmap &px, const QString &text, QWidget *parent = 0);
		/**
		 * Construye un DAnimWidget con una lista de imagenes para animar y un padre.
		 * @param lop imagenes para animar
		 * @param parent 
		 */
		DAnimWidget(ListOfPixmaps lop, QWidget *parent = 0);
		
		/**
		 * Destructor
		 */
		~DAnimWidget();
		
		/**
		 * pone una imagen de fondo a la animacion
		 * @param px imagen de fondo
		 */
		void setBackgroundPixmap(const QPixmap &px);

		
	protected:
		/**
		 * Inicia la animacion
		 */
		void showEvent ( QShowEvent * e);
		/**
		 * Detiene la animacion
		 */
		void hideEvent ( QHideEvent * e);
		
	protected:
		/**
		 * Avanza la animacion.
		 */
		void timerEvent(QTimerEvent *e);
		/**
		 * Dibuja la animacion.
		 */
		void paintEvent(QPaintEvent *e);
		
	private:
		Type m_type;
		class Controller;
		Controller *m_controller;
		QPixmap m_background;
		QString m_text;
		QRectF m_textRect;
		
		ListOfPixmaps m_pixmaps;
		int m_pixmapIndex;
};

#endif
