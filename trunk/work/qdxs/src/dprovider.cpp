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

#include <qmessagebox.h>
#include <QtDebug>
#include <QSettings>

#include "dprovider.h"

DProvider::DProvider() : mNoUpload( false )
{
}

DProvider::DProvider( const QDomElement &e ) : mNoUpload( false )
{
	parseDomElement( e );
}

DProvider::~DProvider()
{
}


void DProvider::setName( const QString &name )
{
	mName = name;
}

QString DProvider::name() const
{
	return mName;
}


void DProvider::setIcon( const QUrl &url )
{
	mIcon = url;
}

QUrl DProvider::icon() const
{
	return mIcon;
}


void DProvider::setDownloadUrl( const QUrl &url )
{
	mDownloadUrl = url;
}

QUrl DProvider::downloadUrl() const
{
	return mDownloadUrl;
}


void DProvider::setUploadUrl( const QUrl &url )
{
	mUploadUrl = url;
}

QUrl DProvider::uploadUrl() const
{
	return mUploadUrl;
}


void DProvider::setNoUploadUrl( const QUrl &url )
{
	mNoUploadUrl = url;
}

QUrl DProvider::noUploadUrl() const
{
	return mNoUploadUrl;
}


void DProvider::setNoUpload( bool enabled )
{
	mNoUpload = enabled;
}

bool DProvider::noUpload() const
{
	return mNoUpload;
}


void DProvider::parseDomElement( const QDomElement &element )
{
	if ( element.tagName() != "provider" ) return;

	setDownloadUrl( QUrl( element.attribute("downloadurl") ) );
	setUploadUrl( QUrl( element.attribute("uploadurl") ) );
	setNoUploadUrl( QUrl( element.attribute("nouploadurl") ) );

	QUrl iconurl( element.attribute("icon") );
	if(!iconurl.isValid()) iconurl.setPath( element.attribute("icon") );
	setIcon( iconurl );

	QDomNode n;
	for ( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
		QDomElement p = n.toElement();
    
		if ( p.tagName() == "noupload" ) setNoUpload( true );
		if ( p.tagName() == "title" ) setName( p.text().trimmed() );
	}
}

QDomElement DProvider::createDomElement( QDomDocument &doc, QDomElement &parent )
{
	QDomElement entry = doc.createElement( "stuff" );
	parent.appendChild( entry );

	QDomElement n = doc.createElement( "name" );
	n.appendChild( doc.createTextNode( name() ) );
	entry.appendChild( n );
  
	return entry;
}


DProviderLoader::DProviderLoader( QWidget *parentWidget ) : mParentWidget( parentWidget )
{
	m_transferManager = new DTransferManager;
	connect( m_transferManager, SIGNAL(error(const QString &)), this, SLOT(haveError(const QString &)));
	connect(m_transferManager, SIGNAL(readed( const QByteArray& )), this, SLOT(slotRead(const QByteArray &)));
}

DProviderLoader::~DProviderLoader()
{
	qDeleteAll(mDProviders);
	mDProviders.clear();
	
	if ( m_transferManager )
	{
		delete m_transferManager;
	}
}

void DProviderLoader::load( const QString &type, const QString &providersList )
{
	qDebug() << "DProviderLoader::load()";

	mDProviders.clear();
// 	mJobData = "";

	QSettings cfg;
	cfg.beginGroup("QNewStuff");

	QString providersUrl = providersList;
	if( providersUrl.isEmpty() )
		providersUrl = cfg.value( "ProvidersUrl").toString();

	if ( providersUrl.isEmpty() ) {
    		// TODO: Replace the default by the real one.
		QString server = cfg.value( "MasterServer",
					    "http://korganizer.kde.org" ).toString();
  
		providersUrl = server + "/knewstuff/" + type + "/providers.xml";
	}

	qDebug() << "DProviderLoader::load(): providersUrl: " << providersUrl << endl;
	
	
	m_transferManager->get(providersUrl);
	
// 	KIO::TransferJob *job = KIO::get( QUrl( providersUrl ) );
// 	connect( job, SIGNAL( result( KIO::Job * ) ),
// 		 SLOT( slotJobResult( KIO::Job * ) ) );
// 	connect( job, SIGNAL( data( KIO::Job *, const QByteArray & ) ),
// 		 SLOT( slotJobData( KIO::Job *, const QByteArray & ) ) );

}


void DProviderLoader::slotRead(const QByteArray &data)
{
	qDebug("slotRead!!!");
	if ( data.size() == 0 ) return;
	
	QDomDocument doc;
	if ( !doc.setContent( QString(data) ) )
	{
		QMessageBox::critical( mParentWidget, tr("An error"), tr("Error parsing providers list.") );
		return;
	}
	
	QDomElement providers = doc.documentElement();
	
	if ( providers.isNull() ) 
	{
		qDebug() << "No document in DProviders.xml." << endl;
	}
	
	QDomNode n;
	for ( n = providers.firstChild(); !n.isNull(); n = n.nextSibling() ) 
	{
		QDomElement p = n.toElement();
	 
		if ( p.tagName() == "provider" ) 
		{
			mDProviders.append( new DProvider( p ) );
		}
	}
	  
	emit providersLoaded( &mDProviders );
}

void DProviderLoader::haveError(const QString &msg)
{
	qDebug() << "Error while try to get data " << msg;
}

// FIXME FIXME FIXME
// void DProviderLoader::slotJobData( KIO::Job *, const QByteArray &data )
// {
// 	qDebug() << "DProviderLoader::slotJobData()";
// 
// 	if ( data.size() == 0 ) return;
// 
// 	mJobData.append( QString::fromUtf8( data ) ); // ####### The fromUtf8 conversion should be done at the end, not chunk by chunk
// }


// void DProviderLoader::slotJobResult( KIO::Job *job )
// {
// 	if ( job->error() ) {
// 		job->showErrorDialog( mParentWidget );
// 	}
// 
// 	kdDebug(5850) << "--PROVIDERS-START--" << endl << mJobData << "--PROV_END--"
// 			<< endl;
// 
// 	QDomDocument doc;
// 	if ( !doc.setContent( mJobData ) ) {
// 		KMessageBox::error( mParentWidget, tr("Error parsing providers list.") );
// 		return;
// 	}
// 
// 	QDomElement providers = doc.documentElement();
// 
// 	if ( providers.isNull() ) {
// 		kdDebug(5850) << "No document in DProviders.xml." << endl;
// 	}
// 
// 	QDomNode n;
// 	for ( n = providers.firstChild(); !n.isNull(); n = n.nextSibling() ) {
// 		QDomElement p = n.toElement();
//  
// 		if ( p.tagName() == "provider" ) {
// 			mDProviders.append( new DProvider( p ) );
// 		}
// 	}
//   
// 	emit providersLoaded( &mDProviders );
// }
