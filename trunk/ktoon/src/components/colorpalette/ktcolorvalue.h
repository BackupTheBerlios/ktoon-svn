/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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

#ifndef KTVALUECOLOR_H
#define KTVALUECOLOR_H

#include <QFrame>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class KTItemColorValue : public QFrame
{
    Q_OBJECT
    public:
        KTItemColorValue( const QString &text = QString::null, QWidget *parent = 0);
        ~KTItemColorValue();
        int  value();
        void setMax(int);
        void setRange(int minimum, int maximum);
        void setSuffix(const QString &suffix );

    private:
        struct Private;
        Private *const k;

    public slots:
        void setValue ( int val );

    signals:
        void valueChanged ( int i );
        void valueChanged ( const QString & text );
        void editingFinished();
};

class KTColorValue : public QFrame
{
    Q_OBJECT
    public:
        KTColorValue(QWidget *parent = 0);
        ~KTColorValue();
       int hue();
       int saturation();
       int value();
       int alpha();

    private:
       struct Private;
       Private *const k;

    private:
       void setupForm();

    public slots:
       void setColor(const QBrush & brush);

    private slots:
       void syncValuesRgb(int value = 0);

    signals:
       void brushChanged(const QBrush &);
       void hueChanged(int);
       void saturationChanged(int);
       void valueChanged(int);
};

#endif
