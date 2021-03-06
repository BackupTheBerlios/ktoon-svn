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
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef KSEPARATOR_H
#define KSEPARATOR_H

#include <QFrame>
#include <kcore/kglobal.h>

/**
 * Standard horizontal or vertical separator.
 *
 * @author Michael Roth \<mroth@wirlweb.de\>
 */
class K_GUI_EXPORT KSeparator : public QFrame
{
	Q_OBJECT;
	Q_PROPERTY( Qt::Orientation orientation READ orientation WRITE setOrientation );

 public:
  /**
   * Constructor.
   * @param parent parent object.
   * @param f extra QWidget flags.
   **/
  KSeparator(QWidget* parent=0);

  /**
   * Constructor.
   * @param orientation Set the orientation of the separator.
   * Possible values are Horizontal or Vertical.
   * @param parent parent object.
   * @param f extra QWidget flags.
   **/
  KSeparator(Qt::Orientation orientation, QWidget* parent=0);

  /**
   * Returns the orientation of the separator.
   * @return int Possible values Horizontal or Vertical.
   **/
  Qt::Orientation orientation() const;

  /**
   * Set the orientation of the separator to @p orientation
   *
   * @param orientation Possible values are Vertical and Horizontal.
   */
  void setOrientation(Qt::Orientation orientation);

protected:
  virtual void virtual_hook( int id, void* data );

private:
  class KSeparatorPrivate* k;
};


#endif // KSEPARATOR_H
