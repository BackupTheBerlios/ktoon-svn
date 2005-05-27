/***************************************************************************
 *   Copyright (C) 2004 by Simena Dinas                                    *
 *   dsimena@toonka.com                                                    *
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

#ifndef DOCUMENT_H
#define DOCUMENT_H

/**
 * @file document.h
 * @brief Include this file if you need the class Document
 */

#include <qstring.h>
#include <qptrlist.h>
#include <qdom.h>
#include <qfile.h>

#include "palette.h"
#include "brush.h"
#include "librarydata.h"
#include "animation.h"

/**
 * @brief Class that encapsulates the concept of document
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the document storing scheme. It provides a document name, an animation,
 * a custom color palette, and a list of brushes.
 */
class Document
{

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a Document object.
     */
    Document();
    /**
     * @brief Default Destructor
     *
     * Destroys the Document object.
     */
    ~Document();

    //Set methods
    /**
     * @brief Sets a new name for this document
     *
     * @param name The new name
     * @sa nameDocument()
     */
    void setNameDocument( const QString & name );
    /**
     * @brief Sets a new Animation for this document
     *
     * @param animation The new Animation
     * @sa getAnimation()
     */
    void setAnimation( Animation *animation );
    /**
     * @brief Sets a new Palette for this document
     *
     * @param palette The new custom palette
     * @sa getPalette()
     */
    void setPalette( Palette *palette );
    /**
     * @brief Sets a new list of brushes for this document
     *
     * @param brushes The new list of brushes
     * @sa getBrushes()
     */
    void setBrushes( QPtrList<Brush> brushes );
    /**
     * @brief Sets a new library for this document
     *
     * @param library The new library
     * @sa getLibrary()
     */
    void setLibrary( LibraryData *library );

    //Get methods
    /**
     * @brief Gets the current document name
     *
     * @return The current name
     * @sa setNameDocument()
     */
    QString nameDocument() const;
    /**
     * @brief Gets the current document Animation
     *
     * @return The current Animation
     * @sa setAnimation()
     */
    Animation *getAnimation() const;
    /**
     * @brief Gets the current document Palette
     *
     * @return The current custom palette
     * @sa setPalette()
     */
    Palette *getPalette() const;
    /**
     * @brief Gets the current document list of brushes
     *
     * @return The current list of brushes
     * @sa setBrushes()
     */
    QPtrList<Brush> getBrushes() const;
    /**
     * @brief Gets the current document library
     *
     * @return The current library
     * @sa setLibrary()
     */
    LibraryData *getLibrary() const;

    /**
     * @brief Saves this document into the file \a f
     *
     * @param f The file where the document are going to be saved
     */
    void save( QFile *f );
    /**
     * @brief Parses the argument, converting the '/'s into '_'
     *
     * Since the XML specification does not allow the character '/' into attribute names or values because it is
     * used for a tag end mark, this function is used for transforming that characters into '_'.
     * @e Example: "/usr/local/bin" turns into "_usr_local_bin"
     * @param str The string to be converted
     * @return The converted string
     * @sa turnUnderscoresIntoSlashes()
     */
    static QString turnSlashesIntoUnderscores( const QString &str );
    /**
     * @brief Parses the argument, converting the '_'s into '/'
     *
     * Function that does the inverse operation than the above function.
     * @e Example: "_usr_local_bin" turns into "/usr/local/bin"
     * @param str The string to be converted
     * @return The converted string
     * @sa turnSlashesIntoUnderscores()
     */
    static QString turnUnderscoresIntoSlashes( const QString &str );
	      
	      
	      static QString getLocalName( const QString &str);

private:
    QString name;
    Animation *animation;
    Palette *custom_palette;
    QPtrList<Brush> brushes;
    LibraryData *library;
    QDomDocument xml_doc;
    

};

#endif
