/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *                                                                         *
 *   License:                                                              *
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


#ifndef KDOUBLECOMBOBOX_H
#define KDOUBLECOMBOBOX_H

#include <QComboBox>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KDoubleComboBox : public QComboBox
{
	Q_OBJECT;
	
	public:
		KDoubleComboBox(double min, double max, QWidget *parent = 0);
		~KDoubleComboBox();
		
		bool showAsPercent() const;
		
		void setDecimals(int d);
		int decimals() const;
		
		void setMaximum(double max);
		void setMinimum(double min);
		
		double maximum() const;
		double minimum() const;
		
		void addValue(double v);
		void addPercent(double p);
		
		double value();
		void setValue(int index, double v);
		
		double percent();
		void setPercent(int index, double p);
		
		
	public slots:
		void setShowAsPercent(bool p);
		
	private slots:
		void emitHighlighted(int index);
		void emitActivated(int index);
		void emitCurrentIndexChanged(int index);
		
	signals:
		void highlighted(double v);
		void activated(double v);
		void currentIndexChanged(double v);
		void editingFinished();
	
	private:
		struct Private;
		Private *const k;
};

#endif
