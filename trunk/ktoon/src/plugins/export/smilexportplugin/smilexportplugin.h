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

#ifndef SMILEXPORTPLUGIN_H
#define SMILEXPORTPLUGIN_H

#include <ktexportpluginobject.h>

#include <QDomDocument>
#include <QDomElement>

/**
 * @author David Cuadrado \<krawek@toonka.com\>
*/

class SmilExportPlugin : public KTExportPluginObject
{
    Q_OBJECT;

    public:
        SmilExportPlugin();
        virtual ~SmilExportPlugin();
        virtual QString key() const;
        KTExportInterface::Formats availableFormats();

        virtual bool exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, KTExportInterface::Format format, const QSize &size, int fps);
        virtual const char* getExceptionMsg();
        const char *errorMsg;

    private:
        QStringList createImages(const QList<KTScene *> &scenes, const QDir &dir,float sx = 1, float sy = 1, const char *format = "PNG");
        void initSmil();
        void createPar(const QString &filePath, double duration);

    private:
        QString m_baseName;

        QDomDocument m_smil;
        QDomElement m_body;

        int m_fps;
        QSize m_size;
};

#endif
