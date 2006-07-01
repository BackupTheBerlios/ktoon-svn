/***************************************************************************
 *   Copyright (C) 2004 by Alexander Dymo                                  *
 *   adymo@kdevelop.org                                                    *
 *   David Cuadrado (C) 2005 						   *
 *   krawek@gmail.com							   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef IDEALBUTTONBAR_H
#define IDEALBUTTONBAR_H

#include <QWidget>
#include <QList>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QBoxLayout>

#include "dgui/comdefs.h"

#include <QLayout>

namespace Ideal {

class DiButton;
class DiButtonBar;

/**
 * @short A layout for a DiButtonBar class.
 * Overrides minimumSize method to allow shrinking DiButton bar buttons.
*/
class DiButtonLayout: public QBoxLayout
{
	public:
		DiButtonLayout(Direction d, DiButtonBar *parent);
        
		virtual QSize minimumSize() const;
    
	private:
		DiButtonBar *m_buttonBar;
};

/**
@short A bar with tool buttons.

Looks like a toolbar but has another behaviour. It is suitable for
placing on the left(right, bottom, top) corners of a window as a bar with slider.
*/
class DiButtonBar : public QWidget 
{
    Q_OBJECT
public:    
    DiButtonBar(Place place, ButtonMode mode = IconsAndText,
        QWidget *parent = 0);
    virtual ~DiButtonBar();

    /**Adds a DiButton to the bar.*/
    virtual void addButton(DiButton *DiButton);
    /**Removes a DiButton from the bar and deletes the DiButton.*/
    virtual void removeButton(DiButton *DiButton);

    /**Sets the mode.*/    
    void setMode(ButtonMode mode);
    /**@returns the mode.*/
    ButtonMode mode() const;
    
    /**@returns the place.*/
    Place place() const;   
    
    bool autoResize() const;
    void setAutoResize(bool b);
    
    /**Shrinks the DiButton bar so all buttons are visible. Shrinking is done by 
    reducing the amount of text shown on buttons. DiButton icon size is a minimum size
    of a DiButton. If a DiButton does not have an icon, it displays "...".*/
    virtual void shrink(int preferredDimension, int actualDimension);
    virtual void deshrink(int preferredDimension, int actualDimension);
    /**Restores the size of DiButton bar buttons.*/
    virtual void unshrink();
    
	public slots:
		void onlyIcons();
		void onlyText();
		void textAndIcons();
    
protected:
    virtual void resizeEvent ( QResizeEvent *ev );
    virtual void mousePressEvent(QMouseEvent *e);
    
    int originalDimension();
   
private:
    void fixDimensions();
    void setButtonsPlace(Ideal::Place place);
    
    typedef QList<DiButton*> ButtonList;
    ButtonList m_buttons;

    ButtonMode m_mode;
    Place m_place;
    
    DiButtonLayout *l;
    
    bool m_shrinked;
    bool m_autoResize;
};

}

#endif
