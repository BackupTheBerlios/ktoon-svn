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

#include <qstringlist.h>
#include <qdir.h>

#include "document.h"

//-------------- CONSTRUCTOR ---------------

Document::Document()
{
    animation = new Animation();
    custom_palette = new Palette();
    library = new LibraryData();
    name = "";
}

//------------- DESTRUCTOR ------------------

Document::~Document()
{
    delete animation;
    delete custom_palette;
    delete library;
    brushes.setAutoDelete( true );
    brushes.clear();
    brushes.setAutoDelete( false );
}

//------------ PUBLIC MEMBERS ---------------

void Document::setNameDocument( const QString & _name )
{
    name = _name;
}

void Document::setAnimation( Animation *_animation )
{
   Q_CHECK_PTR( _animation );
   animation = _animation;
}

void Document::setPalette( Palette *palette )
{
  Q_CHECK_PTR( palette );
  custom_palette = palette;
}

void Document::setBrushes( QPtrList<Brush> _brushes )
{
    brushes = _brushes;
}

void Document::setLibrary( LibraryData *_library )
{
   Q_CHECK_PTR( _library );
   library = _library;
}

QString Document::nameDocument() const
{
    return name;
}

Animation *Document::getAnimation() const
{
   Q_CHECK_PTR( animation );
   return animation;
}

Palette *Document::getPalette() const
{
    Q_CHECK_PTR( custom_palette );
    return custom_palette;
}

QPtrList<Brush> Document::getBrushes() const
{
    return brushes;
}

LibraryData *Document::getLibrary() const
{
    Q_CHECK_PTR( library );
    return library;
}

void Document::save( QFile *f )
{
    Q_CHECK_PTR( f );
    xml_doc = QDomDocument( turnSlashesIntoUnderscores( name ) );

    QDomElement root = xml_doc.createElement( "Document" );
    xml_doc.appendChild( root );

    QStringList abs_path_str_list = QStringList::split( "/", name );
    QString local_name = abs_path_str_list.last();

    //Save Palette
    QFile palette_file( "projects/cpl/" + local_name + ".cpl" );
    if ( !palette_file.open( IO_WriteOnly ) )
    {
	qDebug( "Could not save .cpl" );
	return;
    }
    QDomDocument palette_document( turnSlashesIntoUnderscores( QDir::current().absPath() ) + "_projects_cpl_" + local_name + ".cpl" );
    QDomElement palette_tag = custom_palette -> createXML( palette_document );
    palette_document.appendChild( palette_tag );
    QDomElement palette_path_tag = xml_doc.createElement( "Palette" );
    palette_path_tag.setAttribute( "PATH", turnSlashesIntoUnderscores( QDir::current().absPath() ) + "_projects_cpl_" + local_name + ".cpl" );
    root.appendChild( palette_path_tag );
    QTextStream palette_stream( &palette_file );
    QString palette_to_write = palette_document.toString( 1 );
    palette_stream << palette_to_write;
    palette_file.close();

    //Save Brushes
    QFile brushes_file( "projects/bru/" + local_name + ".bru" );
    if ( !brushes_file.open( IO_WriteOnly ) )
    {
	qDebug( "Could not save .bru" );
	return;
    }
    QDomDocument brushes_document( turnSlashesIntoUnderscores( QDir::current().absPath() ) + "_projects_bru_" + local_name + ".bru" );
    QDomElement b_tag = brushes_document.createElement( "Brushes" );
    brushes_document.appendChild( b_tag );
    Brush *b_it;
    for ( b_it = brushes.first(); b_it; b_it = brushes.next() )
    {
	QDomElement brush_tag = b_it -> createXML( brushes_document );
	b_tag.appendChild( brush_tag );
    }
    QDomElement brushes_path_tag = xml_doc.createElement( "Brushes" );
    brushes_path_tag.setAttribute( "PATH", turnSlashesIntoUnderscores( QDir::current().absPath() ) + "_projects_bru_" + local_name + ".bru" );
    root.appendChild( brushes_path_tag );
    QTextStream brushes_stream( &brushes_file );
    QString brushes_to_write = brushes_document.toString( 1 );
    brushes_stream << brushes_to_write;
    brushes_file.close();

    //Save Library
    QFile library_file( "projects/lbr/" + local_name + ".lbr" );
    if ( !library_file.open( IO_WriteOnly ) )
    {
	qDebug( "Could not save .lbr" );
	return;
    }
    QDomDocument library_document( turnSlashesIntoUnderscores( QDir::current().absPath() ) + "_projects_lbr_" + local_name + ".lbr" );
    QDomElement library_tag = library -> createXML( library_document );
    library_document.appendChild( library_tag );
    QDomElement library_path_tag = xml_doc.createElement( "Library" );
    library_path_tag.setAttribute( "PATH", turnSlashesIntoUnderscores( QDir::current().absPath() ) + "_projects_lbr_" + local_name + ".lbr" );
    root.appendChild( library_path_tag );
    QTextStream library_stream( &library_file );
    QString library_to_write = library_document.toString( 1 );
    library_stream << library_to_write;
    library_file.close();

    //Save Animation
    QDomElement animation_tag = animation -> createXML( xml_doc );
    root.appendChild( animation_tag );

    QTextStream stream( f );
    QString to_write = xml_doc.toString( 1 );
    stream << to_write;
}

QString Document::turnSlashesIntoUnderscores( const QString &str )
{
    QString ans;

    for ( int i = 0; i < ( int )str.length(); i++ )
    {
	QChar cur_c = str.at( i );
	if ( cur_c == QChar( '/' ) )
	    ans.append( QChar( '_' ) );
	else
	    ans.append( cur_c );
    }

    return ans;
}

QString Document::turnUnderscoresIntoSlashes( const QString &str )
{
    QString ans;

    for ( int i = 0; i < ( int )str.length(); i++ )
    {
	QChar cur_c = str.at( i );
	if ( cur_c == QChar( '_' ) )
	    ans.append( QChar( '/' ) );
	else
	    ans.append( cur_c );
    }

    return ans;
}
