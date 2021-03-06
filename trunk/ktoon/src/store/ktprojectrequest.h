/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
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
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
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

#ifndef KTPROJECTREQUEST_H
#define KTPROJECTREQUEST_H

#include <QObject>
#include <QString>
#include <QVariant>
#include "ktglobal_store.h"

class KTProjectRequest;
class KTProjectRequest;
class KTProjectRequest;
class KTProjectRequest;
class KTProjectResponse;

class STORE_EXPORT KTProjectRequestArgument
{
    public:
        KTProjectRequestArgument();
        KTProjectRequestArgument(const QString &v);
        ~KTProjectRequestArgument();
        
        void operator = (const QString &value);
        void setValue(const QString &value);
        
        bool toBool();
        int toInt();
        double toReal();
        QString toString();
        
    private:
		QString m_value;
};

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTProjectRequest
{
    public:
        enum Action
        {
            None = 0,
            /**
             * Anade una parte, el tipo de data es nulo
             */
            Add = 1,
            /**
             * Remueve una parte, el tipo de data es nulo
             */
            Remove = -Add,
            /**
             * Reinicia los valores de un objeto
             */
            Reset = 2,
            /**
             * Intercambia del valor entre dos posiciones 
             */
            Exchange = 3,
            /**
             * Mueve una parte, el tipo de data es un entero que significa la nueva posicion
             */
            Move = 4,
            /**
             * Bloquea una parte, el tipo de data es booleano.
             */
            Lock = 5,
            /**
             * Renombra una parte, el tipo de data es una cadena de texto.
             */
            Rename = 6,
            /**
             * Selecciona una parte, el tipo de data es nulo
             */
            Select = 7,
            /**
             * Cambia la visibilidad
             */
            View = 8,
            
            Group = 9,
            Ungroup = -Group,
            
            // Items
            Transform = 10,
            Convert = 11,
            EditNodes = 12,
            AddSymbolToProject = 13,
            Tweening = 14,
            
            // frames
            Update = 15,
            Expand = 16,
            Copy = 17,
            Paste = 18,

            // scenes
            GetInfo = 19 
        };
        
        enum Part
        {
            Project = 1000,
            Scene,
            Layer,
            Frame,
            Item,
            Library
        };
        
        KTProjectRequest(const QString &xml = 0);
        virtual ~KTProjectRequest();
        
        void setId(int id);
        virtual int id() const;
        virtual bool isValid() const;
        
        QString xml() const;
        
        void setExternal(bool b);
        bool isExternal() const;
        
        KTProjectRequest &operator = (const KTProjectRequest &other);

    private:
        struct Private;
        Private *const k;
};

#endif
