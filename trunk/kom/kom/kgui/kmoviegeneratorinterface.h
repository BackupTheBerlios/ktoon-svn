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


#ifndef KMOVIEGENERATORINTERFACE_H
#define KMOVIEGENERATORINTERFACE_H

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class KMovieGeneratorInterface
{
    public:
        enum Format
             {
               NONE = 0,
               OGV  = 1 << 0,
               SWF  = 1 << 1,
               MPEG = 1 << 2,
               AVI  = 1 << 3,
               RM   = 1 << 4,
               ASF  = 1 << 5,
               MOV  = 1 << 6,
               GIF  = 1 << 7
             };

        Q_DECLARE_FLAGS(Formats, Format);

        virtual ~KMovieGeneratorInterface() {};
        virtual void nextFrame() = 0;
        virtual void reset() = 0;
        virtual bool movieHeaderOk() = 0;
        virtual const char* getErrorMsg() = 0;

        void saveMovie(const QString &filename) {
                       end();
                       __saveMovie(filename);
        };

    protected:
        virtual void __saveMovie(const QString &filename) = 0;
        virtual bool begin() = 0;
        virtual void end() = 0;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(KMovieGeneratorInterface::Formats);

#endif
