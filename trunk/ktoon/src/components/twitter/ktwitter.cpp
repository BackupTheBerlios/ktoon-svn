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
            emit internetIsOn();
            downloadNews();
        } else {
            formatStatus(answer);
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

                       output += "       <p class=\"status\">\n";
                       output += "       <div class=\"date\">[ " + date + " ]</div><br/>\n";
                       output += "       <b>KTooN:</b> ";
                       output += text;
                       output += "       <p/>\n";
                   }
               }

               n = n.nextSibling();
        }
     }

     QString html = "";

     html += "<html>\n";
     html += "<head>\n";
     html += "<link rel=\"stylesheet\" type=\"text/css\" href=\"ktoon.css\" />\n";
     html += "</head>\n";
     html += "<body class=\"license\">\n";
     html += " <table class=\"twitter_base\">\n";
     html += "  <tr>\n";
     html += "  <td class=\"twitter_status\">\n";
     html += "    <table class=\"status_table\">\n";
     html += "     <tr>\n";
     html += "     <td>\n";
     html += "      &nbsp;&nbsp;\n";
     html += "      <img class=\"twitter_logo\" src=\"/tmp/ktoon_net.png\" alt=\"ktoon_net\"/>\n";
     html += "      <font class=\"twitter_headline\" >&nbsp;&nbsp;ktoon_net</font>\n";
     html += "      <br/>\n";
     html += "     </td>\n";
     html += "     </tr>\n";
     html += "     <tr>\n";
     html += "     <td>\n";

     html += output;

     html += "     </td>\n";
     html += "     </tr>\n";
     html += "    </table>\n";
     html += "  </td>\n";
     html += "  <td class=\"twitter_desc\">\n";
     html += "    <table class=\"desc_table\">\n";
     html += "     <tr><td>\n";
     html += "          <b>Name:</b> " + name + "<br/>\n";
     html += "          <b>Bio:</b> " + description + "<br/>\n";
     html += "          <b>Web:</b> " +  website + "<br/>\n";
     html += "          <b>Followers:</b> " + followers + "<br/>\n";
     html += "          <b>Image:</b> " + image + "<br/>\n";
     html += "     </td></tr>\n";
     html += "     </table>\n";
     html += "  </td>\n";
     html += "  </tr>\n";
     html += "</table>\n";
     html += "</body>\n";
     html += "</html>";

     QFile file("/tmp/twitter.html");
     file.open(QIODevice::WriteOnly);
     QByteArray array = html.toUtf8();
     file.write(array, qstrlen(array));
     file.close();
}
