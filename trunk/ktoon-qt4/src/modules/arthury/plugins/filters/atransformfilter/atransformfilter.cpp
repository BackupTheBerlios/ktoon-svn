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
 
#include "atransformfilter.h"


QStringList ATransformFilter::keys() const
{
	return QStringList() << tr("Flip Horizontally") << tr("Flip Vertically");
}

void ATransformFilter::filter(const QString &filter, const QList<AGraphicComponent *> &frame)
{
	foreach(AGraphicComponent *gc, frame)
	{
		QMatrix matrix(1,0,0,1,0,0);
		
		if (filter == tr("Flip Horizontally"))
		{
			gc->flip(Qt::Horizontal);
		}
		else if (filter == tr("Flip Vertically"))
		{
			gc->flip(Qt::Vertical);
		}
	}
}

QHash<QString, DAction *> ATransformFilter::actions()
{
	QHash<QString, DAction *> hash;
	
	DAction *flipH = new DAction( QIcon(), tr("Flip Horizontally"), QKeySequence(tr("Alt+Ctrl+H" )), this);
	hash.insert( tr("Flip Horizontally"), flipH );
	
	DAction *flipV = new DAction(QIcon(), tr("Flip Vertically"), QKeySequence(tr("Alt+Ctrl+V" )), this);
	hash.insert(tr("Flip Vertically"), flipV);
	
	return hash;
}



Q_EXPORT_PLUGIN( ATransformFilter );


