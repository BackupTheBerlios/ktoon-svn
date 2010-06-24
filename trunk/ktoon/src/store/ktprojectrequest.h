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
             * Intercambia del valor entre dos posiciones 
             */
            Exchange = 2,
            /**
             * Mueve una parte, el tipo de data es un entero que significa la nueva posicion
             */
            Move = 3,
            /**
             * Bloquea una parte, el tipo de data es booleano.
             */
            Lock = 4,
            /**
             * Renombra una parte, el tipo de data es una cadena de texto.
             */
            Rename = 5,
            /**
             * Selecciona una parte, el tipo de data es nulo
             */
            Select = 6,
            /**
             * Cambia la visibilidad
             */
            View = 7,
            
            Group = 8,
            Ungroup = -Group,
            
            // Items
            Transform = 9,
            Convert = 10,
            EditNodes = 11,
            AddSymbolToProject = 12,
            Tweening = 13,
            
            // frames
            Update = 14,
            Expand = 15,
            Copy = 16,
            Paste = 17,
            
            // scenes
            GetInfo = 18
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
