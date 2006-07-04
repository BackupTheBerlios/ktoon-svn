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

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTProjectCommand : public QUndoCommand
{
	public:
		enum Action
		{
			Add = 0,
			Remove
		};
		enum Part
		{
			Project = 1000,
			Frame,
			Layer,
			Scene
		};
		
		KTProjectCommand(Action action, const QString &name);
		~KTProjectCommand();
		
		
		Action action() const;
		
		QString partName() const;
		
		virtual void redo();
		virtual void undo();
		
		virtual int id() const;
		
	private:
		Action m_action;
		QString m_partName;
};

#endif
