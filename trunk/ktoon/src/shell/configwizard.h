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

#ifndef CONFIGWIZARD_H
#define CONFIGWIZARD_H

#include <QString>

#include <kgui/kwizard.h>

class CWFirstPage;
class CWSecondPage;

class QLineEdit;

/**
 * @if english
 * This class helps to config KToon!
 * @endif
 * @if spanish
 * @brief la clase ConfigWizard provee el dialogo para configurar los directorios principales de KToon.
 * El home que es el directorio donde esta instalado ktoon y el repositorio que es el directorio en donde se guardaran todos los proyectos del usuario
 * 
 * @endif
 * @author David Cuadrado
 */
class ConfigWizard : public KWizard
{
    Q_OBJECT
    public:
        /**
         * Crea el dialogo configuracion.
         */
        ConfigWizard();
        /**
         * destuctor
         */
        ~ConfigWizard();

        /**
         * Pone los directorios por defecto en el dialogo.
         * @param home 
         * @param repos 
         */
        void setInitialData(const QString &home, const QString &repos);

        /**
         * @return la ruta del directorio home
         */
        QString home() const;

        /**
         * @return la ruta del directorio repository
         */
        QString cache() const;

     private:
        CWFirstPage *m_firstPage;
        CWSecondPage *m_secondPage;

        QString kthome;
        QString ktrepos;
};

class CWFirstPage : public KWizardPage
{
    Q_OBJECT
    public:
        CWFirstPage(QWidget *parent = 0);
        ~CWFirstPage();

        bool isComplete() const { return true; };
        void reset() { };
};

class CWSecondPage : public KWizardPage
{
    Q_OBJECT
    public:
        CWSecondPage(QWidget *parent = 0);
        ~CWSecondPage();
        void setData(const QString &home, const QString &repos);
        QString home() const;
        QString repository() const;

        bool isComplete() const;
        void reset();

    private slots:
        void verify(const QString &);

    private:
        QLineEdit *m_kthome;
        QLineEdit *m_ktrepos;
};

#endif

