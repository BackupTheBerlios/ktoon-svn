/*
    This file is part of KOrganizer.
    Copyright (c) 2002 Cornelius Schumacher <schumacher@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "dentry.h"
#include <QStringList>

DEntry::DEntry() :
		mRelease( 0 ), mReleaseDate( QDate::currentDate() ), mRating( 0 ),
  mDownloads( 0 )
{
}

DEntry::DEntry( const QDomElement &e ) :
		mRelease( 0 ), mRating( 0 ), mDownloads( 0 )
{
	parseDomElement( e );
}

DEntry::~DEntry()
{
}


void DEntry::setName( const QString &name )
{
	mName = name;
}

QString DEntry::name() const
{
	return mName;
}


void DEntry::setType( const QString &type )
{
	mType = type;
}

QString DEntry::type() const
{
	return mType;
}


void DEntry::setAuthor( const QString &author )
{
	mAuthor = author;
}

QString DEntry::author() const
{
	return mAuthor;
}


void DEntry::setLicence( const QString &license )
{
	mLicence = license;
}

QString DEntry::license() const
{
	return mLicence;
}


void DEntry::setSummary( const QString &text, const QString &lang )
{
	mSummaryMap.insert( lang, text );

	if ( ! mLangs.contains( lang )  ) mLangs.append( lang );
}

QString DEntry::summary( const QString &lang ) const
{
	if ( mSummaryMap.isEmpty() ) return QString::null;

	if ( !mSummaryMap[ lang ].isEmpty() ) return mSummaryMap[ lang ];
	else 
	{
		QStringList langs = QStringList() << "es"; // FIXME: KGlobal::locale()->languageList();
		for(QStringList::Iterator it = langs.begin(); it != langs.end(); ++it)
			if( !mSummaryMap[ *it ].isEmpty() ) return mSummaryMap[ *it ];
	}
	if ( !mSummaryMap[ QString::null ].isEmpty() ) return mSummaryMap[ QString::null ];
	else return *(mSummaryMap.begin());
}


void DEntry::setVersion( const QString &version )
{
	mVersion = version;
}

QString DEntry::version() const
{
	return mVersion;
}


void DEntry::setRelease( int release )
{
	mRelease = release;
}

int DEntry::release() const
{
	return mRelease;
}


void DEntry::setReleaseDate( const QDate &d )
{
	mReleaseDate = d;
}

QDate DEntry::releaseDate() const
{
	return mReleaseDate;
}


void DEntry::setPayload( const QUrl &url, const QString &lang )
{
	mPayloadMap.insert( lang, url );

	if ( ! mLangs.contains( lang )) mLangs.append( lang );
}

QUrl DEntry::payload( const QString &lang ) const
{
	QUrl payload = mPayloadMap[ lang ];
	if ( payload.isEmpty() ) 
	{
		QStringList langs = QStringList() << "es"; //FIXME: KGlobal::locale()->languageList();
		for(QStringList::Iterator it = langs.begin(); it != langs.end(); ++it)
			if( !mPayloadMap[ *it ].isEmpty() ) return mPayloadMap[ *it ];
	}
	if ( payload.isEmpty() ) payload = mPayloadMap [ QString::null ];
	if ( payload.isEmpty() && !mPayloadMap.isEmpty() ) {
		payload = *(mPayloadMap.begin());
	}
	return payload;
}


void DEntry::setPreview( const QUrl &url, const QString &lang )
{
	mPreviewMap.insert( lang, url );
  
	if (! mLangs.contains( lang )) mLangs.append( lang );
}

QUrl DEntry::preview( const QString &lang ) const
{
	QUrl preview = mPreviewMap[ lang ];
	if ( preview.isEmpty() ) {
		QStringList langs = QStringList() << "es"; //FIXME: KGlobal::locale()->languageList();
		for(QStringList::Iterator it = langs.begin(); it != langs.end(); ++it)
			if( !mPreviewMap[ *it ].isEmpty() ) return mPreviewMap[ *it ];
	}
	if ( preview.isEmpty() ) preview = mPreviewMap [ QString::null ];
	if ( preview.isEmpty() && !mPreviewMap.isEmpty() ) {
		preview = *(mPreviewMap.begin());
	}
	return preview;
}


void DEntry::setRating( int rating )
{
	mRating = rating;
}

int DEntry::rating()
{
	return mRating;
}


void DEntry::setDownloads( int downloads )
{
	mDownloads = downloads;
}

int DEntry::downloads()
{
	return mDownloads;
}

QString DEntry::fullName()
{
	return name() + "-" + version() + "-" + QString::number( release() );
}

QStringList DEntry::langs()
{
	return mLangs;
}

void DEntry::parseDomElement( const QDomElement &element )
{
	if ( element.tagName() != "stuff" ) return;
	mType = element.attribute("type");

	QDomNode n;
	for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
		QDomElement e = n.toElement();
		if ( e.tagName() == "name" ) setName( e.text().trimmed() );
		if ( e.tagName() == "author" ) setAuthor( e.text().trimmed() );
		if ( e.tagName() == "licence" ) setLicence( e.text().trimmed() );
		if ( e.tagName() == "summary" ) {
			QString lang = e.attribute( "lang" );
			setSummary( e.text().trimmed(), lang );
		}
		if ( e.tagName() == "version" ) setVersion( e.text().trimmed() );
		if ( e.tagName() == "release" ) setRelease( e.text().toInt() );
		if ( e.tagName() == "releasedate" ) {
			QDate date = QDate::fromString( e.text().trimmed(), Qt::ISODate );
			setReleaseDate( date );
		}
		if ( e.tagName() == "preview" ) {
			QString lang = e.attribute( "lang" );
			setPreview( QUrl( e.text().trimmed() ), lang );
		}
		if ( e.tagName() == "payload" ) {
			QString lang = e.attribute( "lang" );
			setPayload( QUrl( e.text().trimmed() ), lang );
		}
		if ( e.tagName() == "rating" ) setRating( e.text().toInt() );
		if ( e.tagName() == "downloads" ) setDownloads( e.text().toInt() );
	}
}

QDomElement DEntry::createDomElement( QDomDocument &doc,
				     QDomElement &parent )
{
	QDomElement entry = doc.createElement( "stuff" );
	entry.setAttribute("type", mType);
	parent.appendChild( entry );

	addElement( doc, entry, "name", name() );
	addElement( doc, entry, "author", author() );
	addElement( doc, entry, "licence", license() );
	addElement( doc, entry, "version", version() );
	addElement( doc, entry, "release", QString::number( release() ) );
	addElement( doc, entry, "rating", QString::number( rating() ) );
	addElement( doc, entry, "downloads", QString::number( downloads() ) );

	addElement( doc, entry, "releasedate",
		    releaseDate().toString( Qt::ISODate ) );

	QStringList ls = langs();
	QStringList::ConstIterator it;
	for( it = ls.begin(); it != ls.end(); ++it ) {
		QDomElement e = addElement( doc, entry, "summary", summary( *it ) );
		e.setAttribute( "lang", *it );
		e = addElement( doc, entry, "preview", preview( *it ).toString() );
		e.setAttribute( "lang", *it );
		e = addElement( doc, entry, "payload", payload( *it ).toString() );
		e.setAttribute( "lang", *it );
	}

	return entry;
}

QDomElement DEntry::addElement( QDomDocument &doc, QDomElement &parent,
			       const QString &tag, const QString &value )
{
	QDomElement n = doc.createElement( tag );
	n.appendChild( doc.createTextNode( value ) );
	parent.appendChild( n );

	return n;
}
