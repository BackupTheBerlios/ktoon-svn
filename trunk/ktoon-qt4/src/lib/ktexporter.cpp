/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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
#include "ktexporter.h"

KTExporter::KTExporter(QObject *parent) : QObject(parent), m_filename(QString::null)
{
	qDebug("[Initializing KTExporter]");
}


KTExporter::~KTExporter()
{
	qDebug("[Destroying KTExporter]");
}

bool KTExporter::exportAnimation(const QString &filename, Format format)
{
	m_filename = filename;
	switch(format)
	{
		case PNG:
		{
			if ( ! m_filename.contains(".png", Qt::CaseInsensitive) )
			{
				m_filename += ".png";
			}
			
		}
		break;
		case SWF:
		{
			if ( ! m_filename.contains(".swf", Qt::CaseInsensitive) )
			{
				m_filename += ".swf";
			}
		}
		break;
		case SVG:
		{
			if ( ! m_filename.contains(".svg", Qt::CaseInsensitive) )
			{
				m_filename += ".svg";
			}
		}
		break;
		case JPEG:
		{
			if ( ! m_filename.contains(".jpg", Qt::CaseInsensitive) )
			{
				m_filename += ".jpg";
			}
		}
		break;
		default:
		{
			return false;
		}
		break;
	}
	return true;
}

#ifdef USE_MING
SWFDisplayItem *KTExporter::doAnimation( SWFMovie *movie, const QStringList &images )
{
    int i, image_count;
    QStringList copy = QStringList( images );
    QStringList::Iterator it;
    SWFDisplayItem *frame = NULL;
    image_count = ( int )copy.count();
    SWFShape *shape[image_count];

    for( it = copy.begin(), i = 0; i < image_count; ++i, ++it )
    {
        SWFBitmap *bitmap = new SWFBitmap( ( char * )( *it ).latin1() );
        float width  = bitmap -> getWidth();
        float height = bitmap -> getHeight();

	  SWFShape* p_shape = new(std::nothrow) SWFShape();
	  if(!p_shape)
	      {
		delete bitmap;
		throw std::bad_alloc();
		}
	  shape[i] = p_shape;
	  
        SWFFill *fill = shape[i] -> addBitmapFill( bitmap );
        shape[i] -> setRightFill( fill );
        shape[i] -> drawLine( width, 0 );
        shape[i] -> drawLine( 0, height );
        shape[i] -> drawLine( -1 * width, 0 );
        shape[i] -> drawLine( 0, -1 * height );

	frame = movie -> add( shape[i] );
        movie -> nextFrame();
    }

    return frame;
}

SWFMovie *KTExporter::doMovie( int width, int height )
{
    SWFMovie *movie = new SWFMovie();
    movie -> setDimension( width, height );
    movie -> setBackground( 0xff, 0xff, 0xff );

    return movie;
}

#endif


