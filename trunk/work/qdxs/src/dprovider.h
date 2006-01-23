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
#ifndef QNEWSTUFF_PROVIDER_H
#define QNEWSTUFF_PROVIDER_H

#include <qdom.h>
#include <qobject.h>
#include <qstring.h>

#include <qurl.h>

#include "dtransfermanager.h"

class DProvider
{
	public:
		typedef QList<DProvider *> List;

		DProvider();
		DProvider( const QDomElement & );

		~DProvider();

		void setName( const QString & );
		QString name() const;
		void setDownloadUrl( const QUrl & );
		QUrl downloadUrl() const;
		void setUploadUrl( const QUrl & );
		QUrl uploadUrl() const;
		void setNoUploadUrl( const QUrl & );
		QUrl noUploadUrl() const;
		void setNoUpload( bool );
		bool noUpload() const;
		void setIcon( const QUrl & );
		QUrl icon() const;

	protected:
		void parseDomElement( const QDomElement & );

		QDomElement createDomElement( QDomDocument &, QDomElement &parent );

	private:
		QString mName;
		QUrl mDownloadUrl;
		QUrl mUploadUrl;
		QUrl mNoUploadUrl;
		QUrl mIcon;
		bool mNoUpload;
};

class  DProviderLoader : public QObject
{
	Q_OBJECT
	public:
		DProviderLoader( QWidget *parentWidget );
		~DProviderLoader();
		void load( const QString &type, const QString &providerList = QString::null );

	signals:
		void providersLoaded( DProvider::List * );

	protected slots:
		void slotRead(const QByteArray &);
		void haveError(const QString &);
// 		void slotJobData( KIO::Job *, const QByteArray & );
// 		void slotJobResult( KIO::Job * );

	private:
		QWidget *mParentWidget;
		DProvider::List mDProviders;
		
		DTransferManager *m_transferManager;
};


#endif
