#ifndef KTWITTER_H
#define KTWITTER_H

#include <QWidget>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QFile>
#include <QByteArray>

class KTwitter : public QWidget
{
    Q_OBJECT

    public:
        KTwitter(QWidget *parent=0);
        ~KTwitter();
        void start();

    private slots:
        void closeRequest(QNetworkReply *reply);
        void slotError(QNetworkReply::NetworkError error);

    signals:
        void pageReady();

    private:
        void requestFile(QString target);
        void checkSoftwareUpdates(QByteArray array);
        void formatStatus(QByteArray array);

        static QString TWITTER_HOST;
        static QString IS_TWITTER_UP_URL;
        static QString USER_TIMELINE_URL;
        static QString KTOON_VERSION_URL;
        static QString BROWSER_FINGERPRINT;

        struct Private;
        Private *const k;
};

#endif
