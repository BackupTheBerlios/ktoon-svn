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


#ifndef KTPROJECTCOMMAND_H
#define KTPROJECTCOMMAND_H

#include <QUndoCommand>
#include "ktglobal_store.h"

class KTProject;
class KTProjectEvent;
class KTFrameEvent;
class KTLayerEvent;
class KTSceneEvent;
class KTItemEvent;

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class STORE_EXPORT KTProjectCommand : public QUndoCommand
{
	public:
		KTProjectCommand(KTProject *project, const KTProjectEvent *event);
		~KTProjectCommand();
		
		virtual void redo();
		virtual void undo();
		
		void frameCommand(const KTFrameEvent *event, bool redo);
		void layerCommand(const KTLayerEvent *event, bool redo);
		void sceneCommand(const KTSceneEvent *event, bool redo);
		void itemCommand(const KTItemEvent *event, bool redo);
		
	private:
		QString actionString(int action);
		
	private:
		KTProject *m_project;
		KTProjectEvent *m_event;
		
		QString m_xml;
};

#endif
