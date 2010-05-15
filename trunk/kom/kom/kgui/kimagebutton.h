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


#ifndef KIMAGEBUTTON_H
#define KIMAGEBUTTON_H

#include <QPushButton>
#include <QImage>
#include <QPixmap>
#include <QIcon>
#include <QTimer>
#include <kcore/kglobal.h>

/**
 * A image pressable
 * @author David Cuadrado <krawek@gmail.com>
*/

class K_GUI_EXPORT KImageButton : public QPushButton
{
    Q_OBJECT

    public:
        KImageButton(const QIcon &icon, int size, QWidget *parent = 0, bool animate = false);
        KImageButton(const QIcon &icon, int size, QObject *reciever, const  char *slot, QWidget *parent = 0, bool animate = false);
        ~KImageButton();
        virtual void setImage(const QIcon &icon);
        
        void setAnimated(bool anim);
        
    protected:
        void enterEvent(QEvent *e);
        void leaveEvent(QEvent *e);
        
    private slots:
        void animate();
        
    private:
        void setup();
        
    private:
        int m_imageSize;
        class Animation;
        Animation *m_animator;
        
        bool m_isAnimated;
/*      
    protected:
        void resizeEvent(QResizeEvent *e);
        void paintEvent(QPaintEvent *e);
        QPixmap getPixmap();
*/
};

#endif
