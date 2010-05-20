#include <QtGui>
#include <QtNetwork>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDomDocument>
#include <QEventLoop>

#include "ktwitter.h"

QString KTwitter::TWITTER_HOST = QString("http://twitter.com");
QString KTwitter::IS_TWITTER_UP_URL = QString("/help/test.xml");
QString KTwitter::USER_TIMELINE_URL = QString("/statuses/user_timeline/ktoon_net.xml");

struct KTwitter::Private
{
    bool linkOn;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;
};

KTwitter::KTwitter(QWidget *parent) : QWidget(parent), k(new Private)
{
    QString url = TWITTER_HOST + IS_TWITTER_UP_URL;

    k->manager = new QNetworkAccessManager(this);
    connect(k->manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(closeRequest(QNetworkReply*)));

    k->request.setUrl(QUrl(url));
    k->request.setRawHeader("User-Agent", "KTooN_Browser 1.0");

    k->reply = k->manager->get(k->request);
    connect(k->reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(k->reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
}

KTwitter::~KTwitter(){
    delete k;
}

void KTwitter::downloadNews(){

    k->request.setUrl(QUrl(TWITTER_HOST +  USER_TIMELINE_URL));
    k->request.setRawHeader("User-Agent", "KTooN_Browser 1.0");
    k->reply = k->manager->get(k->request);
}

void KTwitter::closeRequest(QNetworkReply *reply)
{
    QString answer(reply->readAll());

    if (answer.length() > 0) {
        if (answer.compare("<ok>true</ok>") == 0) {
            k->linkOn = true;
            downloadNews();
        } else {
            formatStatus(answer);
            QCoreApplication::quit();
        }
    } else {
        k->linkOn = false;
    }
}

bool KTwitter::isNetAvailable()
{
    return k->linkOn;
}

void KTwitter::slotReadyRead()
{
}

void KTwitter::slotError(QNetworkReply::NetworkError error)
{
    switch (error) {
    case QNetworkReply::HostNotFoundError :
         qWarning("Host not found");
         break;
    case QNetworkReply::TimeoutError :
         qWarning("Time out!");
         break;
    case QNetworkReply::ConnectionRefusedError :
         qWarning("Connection Refused!");
         break;
    case QNetworkReply::ContentNotFoundError :
         qWarning("Content not found!");
         break;
    case QNetworkReply::UnknownNetworkError :
         qWarning("Unknown Network error!");
         break;
    default:
         break;
    }
}

void KTwitter::formatStatus(const QString &input)
{
    QString output = "";
    QString name = "";
    QString description = "";
    QString website = "";
    QString followers = "";
    QString image = "";

    QDomDocument doc;

    if (doc.setContent(input)) {
        QDomElement root = doc.documentElement();
        QDomNode n = root.firstChild();
        int counter = 0;

        while (!n.isNull()) {
               QDomElement e = n.toElement();

               if (!e.isNull()) {
                   if (e.tagName() == "status") {
                       QDomNode n1 = e.firstChild();
                       QString date = "";
                       QString text = "";
                       while (!n1.isNull()) {
                              QDomElement e1 = n1.toElement();
                              if (!e1.isNull()) {
                                  if (e1.tagName() == "created_at") {
                                      QString data = e1.text(); 
                                      QStringList list1 = data.split(" ");
                                      for (int i=0; i<3; i++)
                                           date += list1.at(i) + " ";
                                      date += list1.at(3);
                                  } else {
                                      if (e1.tagName() == "text") {
                                          text = e1.text();
                                      } else {
                                          if (counter == 0) {
                                              if (e1.tagName() == "user") {
                                                  QDomNode n2 = e1.firstChild();
                                                  while (!n2.isNull()) {
                                                         QDomElement e2 = n2.toElement();
                                                         if (!e2.isNull()) {
                                                             if (e2.tagName() == "name") {
                                                                 name = e2.text(); 
                                                             } else if (e2.tagName() == "description") {
                                                                        description = e2.text();
                                                             } else if (e2.tagName() == "profile_image_url") {
                                                                        image = e2.text();
                                                             } else if (e2.tagName() == "url") {
                                                                        website = e2.text(); 
                                                             } else if (e2.tagName() == "followers_count") {
                                                                        followers = e2.text();
                                                             }
                                                         }
                                                         n2 = n2.nextSibling();
                                                  }
                                                  counter = 1;
                                              }
                                          }
                                      }
                                  }
                              }

                              n1 = n1.nextSibling();
                       }

                       output += "<p class=\"status\">\n";
                       output += "[ " + date + " ]";
                       output += " <b>KTooN:</b> ";
                       output += text;
                       output += "<p/>\n";
                   }
               }

               n = n.nextSibling();
        }
     }

     output += "</body>\n</html>\n";

     QString html = "";

     html += "<html><body>\n";

     html += "NAME: " + name + "<br/>\n";
     html += "DESC: " + description + "<br/>\n";
     html += "URL: " + website + "<br/>\n";
     html += "FOLLOWERS: " + followers + "<br/>\n";
     html += "IMAGE: " + image + "<br/>\n";

     html += output;
  
     qDebug() << html;
}
