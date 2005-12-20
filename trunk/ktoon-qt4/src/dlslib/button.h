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

#ifndef IDEALBUTTON_H
#define IDEALBUTTON_H

#include <QPushButton>
#include <QIcon>
#include <QStyleOptionButton>
#include <QTimer>

#include "comdefs.h"

namespace Ideal {

class ButtonBar;
class Button;

class Animation
{
	public:
		Animation(QObject *parent) : count(0), MAXCOUNT(20), INTERVAL(30), isEnter(false)
		{
			timer = new QTimer(parent);
		}
		~Animation() {}
		void start()
		{
			timer->start(INTERVAL);
		}
		
		QColor blendColors( const QColor& color1, const QColor& color2, int percent )
		{
			const float factor1 = ( 100 - ( float ) percent ) / 100;
			const float factor2 = ( float ) percent / 100;

			const int r = static_cast<int>( color1.red() * factor1 + color2.red() * factor2 );
			const int g = static_cast<int>( color1.green() * factor1 + color2.green() * factor2 );
			const int b = static_cast<int>( color1.blue() * factor1 + color2.blue() * factor2 );

			QColor result;
			result.setRgb( r % 256, g% 256, b% 256 );

			return result;
		}
			
		QTimer *timer;
		int count;
		const int MAXCOUNT;
		const int INTERVAL;
			
		bool isEnter;
};

/**
 * @short A button to place onto the ButtonBar
 * A QPushButton derivative with a size of a QToolBar. Button can be rotated 
(placed onto different places in ideal mode).
 * @TODO: remember mouse sensibility
*/
class Button : public QPushButton {
    Q_OBJECT
public:
    Button(ButtonBar *parent, const QString text, const QIcon &icon = QIcon(),
        const QString &description = QString::null);
    virtual ~Button();
    
    /**Sets the description used as a tooltip.*/
    void setDescription(const QString &description);
    /**Returns the description.*/
    QString description() const;
    
    QStyleOptionButton Button::styleOption() const;
    
    /**Sets the place of a button.*/
    void setPlace(Ideal::Place place);
    /**Sets the mode of a button.*/
    void setMode(Ideal::ButtonMode mode);

    QSize sizeHint() const;
    QSize sizeHint(const QString &text) const;
    
    /**Updates size of a widget. Used after squeezing button's text.*/
    void updateSize();
    
    /**Returns the real (i.e. not squeezed) text of a button.*/
    QString realText() const;
    bool isSensible() const
    {
	    return m_isSensible;
    }
    
	protected slots:
		virtual void animate();
		void toggleSensibility();
		
	signals:
		void selected();
		void deselected();
    
protected:
    ButtonMode mode();
    
    virtual void paintEvent(QPaintEvent *e);
    virtual void enterEvent( QEvent* );
    virtual void leaveEvent( QEvent* );

private:
    
    void fixDimensions(Place oldPlace);
    
    void enableIconSet();
    void disableIconSet();
    void enableText();
    void disableText();
    
    ButtonBar *m_buttonBar;
    
    QString m_description;
    Place m_place;
    
    QString m_realText;
    QIcon m_realIconSet;
    
    bool m_isSensible, m_haveIcon;
    
    Animation *m_animation;
    
friend class ButtonBar;
};

}

#endif
