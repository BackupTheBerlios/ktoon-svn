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
#ifndef KNEWSTUFF_ENTRY_H
#define KNEWSTUFF_ENTRY_H

#include <qdatetime.h>
#include <qdom.h>
#include <qmap.h>
#include <qstring.h>
#include <qstringlist.h>

#include <qurl.h>

/**
 * @short QNewStuff data entry container.
 *
 * This class provides accessor methods to the data objects
 * as used by QNewStuff.
 * It should probably not be used directly by the application.
 *
 * @author Cornelius Schumacher (schumacher@kde.org)
 * \par Maintainer:
 * Josef Spillner (spillner@kde.org)
 */
class  DEntry
{
  public:
    DEntry();
    /**
     * Constructor.
     */
    DEntry( const QDomElement & );

    /**
     * Destructor.
     */
    ~DEntry();

    /**
     * Sets the (unique) name for this data object.
     */
    void setName( const QString & );

    /**
     * Retrieve the name of the data object.
     *
     * @return object name
     */
    QString name() const;

    /**
     * Sets the application type, e.g. 'kdesktop/wallpaper'.
     */
    void setType( const QString & );

    /**
     * Retrieve the type of the data object.
     *
     * @return object type
     */
    QString type() const;

    /**
     * Sets the full name of the object's author.
     */
    void setAuthor( const QString & );

    /**
     * Retrieve the author's name of the object.
     *
     * @return object author
     */
    QString author() const;

    /**
     * Sets the license (abbreviation) applicable to the object.
     */
    void setLicence( const QString & );

    /**
     * Retrieve the license name of the object.
     *
     * @return object license
     */
    QString license() const;

    /**
     * Sets a short description on what the object is all about.
     */
    void setSummary( const QString &, const QString &lang = QString::null );

    /**
     * Retrieve a short description about the object.
     *
     * @param lang preferred language, or QString::null for KDE default
     * @return object description
     */
    QString summary( const QString &lang = QString::null ) const;

    /**
     * Sets the version number.
     */
    void setVersion( const QString & );

    /**
     * Retrieve the version string of the object.
     *
     * @return object version
     */
    QString version() const;

    /**
     * Sets the release number, which is increased for feature-equal objects
     * with the same version number, but slightly updated contents.
     */
    void setRelease( int );

    /**
     * Retrieve the release number of the object
     *
     * @return object release
     */
    int release() const;

    /**
     * Sets the release date.
     */
    void setReleaseDate( const QDate & );

    /**
     * Retrieve the date of the object's publication.
     *
     * @return object release date
     */
    QDate releaseDate() const;

    /**
     * Sets the object's file.
     */
    void setPayload( const QUrl &, const QString &lang = QString::null );

    /**
     * Retrieve the file name of the object.
     *
     * @param lang preferred language, or QString::null for KDE default
     * @return object filename
     */
    QUrl payload( const QString &lang = QString::null ) const;

    /**
     * Sets the object's preview file, if available. This should be a
     * picture file.
     */
    void setPreview( const QUrl &, const QString &lang = QString::null );

    /**
     * Retrieve the file name of an image containing a preview of the object.
     *
     * @param lang preferred language, or QString::null for KDE default
     * @return object preview filename
     */
    QUrl preview( const QString &lang = QString::null ) const;

    /**
     * Sets the rating between 0 (worst) and 10 (best).
     *
     * @internal
     */
    void setRating( int );

    /**
     * Retrieve the rating for the object, which has been determined by its
     * users and thus might change over time.
     *
     * @return object rating
     */
    int rating();

    /**
     * Sets the number of downloads.
     * 
     * @internal
     */
    void setDownloads( int );

    /**
     * Retrieve the download count for the object, which has been determined
     * by its hosting sites and thus might change over time.
     *
     * @return object download count
     */
    int downloads();

    /**
     * Return the full name for the meta information. It is constructed as
     * name-version-release.
     */
    QString fullName();

    /**
     * Return the list of languages this object supports.
     */
    QStringList langs();

    /**
     * @internal
     */
    void parseDomElement( const QDomElement & );

    /**
     * @internal
     */
    QDomElement createDomElement( QDomDocument &, QDomElement &parent );

  protected:
    QDomElement addElement( QDomDocument &doc, QDomElement &parent,
                            const QString &tag, const QString &value );

  private:
    QString mName;
    QString mType;
    QString mAuthor;
    QString mLicence;
    QMap<QString,QString> mSummaryMap;
    QString mVersion;
    int mRelease;
    QDate mReleaseDate;
    QMap<QString,QUrl> mPayloadMap;
    QMap<QString,QUrl> mPreviewMap;
    int mRating;
    int mDownloads;

    QStringList mLangs;
};


#endif
