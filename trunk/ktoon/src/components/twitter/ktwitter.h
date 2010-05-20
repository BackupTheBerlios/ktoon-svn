#ifndef KTWITTER_H
#define KTWITTER_H

#include <QWidget>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

class KTwitter : public QWidget
{
    Q_OBJECT

    public:
        KTwitter(QWidget *parent=0);
        ~KTwitter();
        bool isNetAvailable();
        void downloadNews();

    private slots:
        void closeRequest(QNetworkReply *reply);
        void slotReadyRead();
        void slotError(QNetworkReply::NetworkError error);

    private:
        void formatStatus(const QString &input);

        static QString TWITTER_HOST;
        static QString IS_TWITTER_UP_URL;
        static QString USER_TIMELINE_URL;

        struct Private;
        Private *const k;
};

#endif
