#include <QtGui>
#include <QtNetwork>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QEventLoop>

#include "ktwitter.h"
#include <kcore/kdebug.h>

QString KTwitter::TWITTER_HOST = QString("http://twitter.com");
QString KTwitter::IS_TWITTER_UP_URL = QString("/help/test.xml");
QString KTwitter::USER_TIMELINE_URL = QString("/statuses/user_timeline/ktoon_net.xml");
QString KTwitter::KTOON_VERSION_URL = QString("http://www.ktoon.net/updates/current_version.xml");
QString KTwitter::BROWSER_FINGERPRINT = QString("KTooN_Browser 1.0");

struct KTwitter::Private
{
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;
    QString version;
    QString revision;
    QString codeName;
    bool update;
};

KTwitter::KTwitter(QWidget *parent) : QWidget(parent), k(new Private)
{
    k->update = false;
}

void KTwitter::start()
{
    kFatal() << "VERSION: " << kAppProp->version();
    kFatal() << "CODE NAME: " << kAppProp->codeName();
    kFatal() << "REVISION: " << kAppProp->revision();

    QString url = TWITTER_HOST + IS_TWITTER_UP_URL;

    k->manager = new QNetworkAccessManager(this);
    connect(k->manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(closeRequest(QNetworkReply*)));

    k->request.setUrl(QUrl(url));
    k->request.setRawHeader("User-Agent", BROWSER_FINGERPRINT.toAscii());

    k->reply = k->manager->get(k->request);
    connect(k->reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
}

KTwitter::~KTwitter(){
    delete k;
}

void KTwitter::requestFile(QString target){
    k->request.setUrl(QUrl(target));
    k->request.setRawHeader("User-Agent", BROWSER_FINGERPRINT.toAscii());
    k->reply = k->manager->get(k->request);
}

void KTwitter::closeRequest(QNetworkReply *reply)
{
    QByteArray array = reply->readAll();
    QString answer(array);

    if (answer.length() > 0) {
        if (answer.compare("<ok>true</ok>") == 0) {
            kFatal() << "Calling out: " << KTOON_VERSION_URL;
            requestFile(KTOON_VERSION_URL);
        } else {
            if (answer.contains("branch", Qt::CaseSensitive)) {
                checkSoftwareUpdates(array);
                requestFile(TWITTER_HOST +  USER_TIMELINE_URL);
            } else {
                formatStatus(array);
            }
        }
    } 
}

void KTwitter::slotError(QNetworkReply::NetworkError error)
{
    switch (error) {
            case QNetworkReply::HostNotFoundError :
                 qWarning("Network Error: Host not found");
            break;
            case QNetworkReply::TimeoutError :
                 qWarning("Network Error: Time out!");
            break;
            case QNetworkReply::ConnectionRefusedError :
                 qWarning("Network Error: Connection Refused!");
            break;
            case QNetworkReply::ContentNotFoundError :
                 qWarning("Network Error: Content not found!");
            break;
            case QNetworkReply::UnknownNetworkError :
                 qWarning("Network Error: Unknown Network error!");
            break;
            default:
            break;
    }
}

void KTwitter::checkSoftwareUpdates(QByteArray array)
{
    QDomDocument doc;

    if (doc.setContent(array)) {

        QDomElement root = doc.documentElement();
        QDomNode n = root.firstChild();

        while (!n.isNull()) {
               QDomElement e = n.toElement();
               if (!e.isNull()) {
                   if (e.tagName() == "branch") {
                       k->version = e.text();
                       if (k->version.compare(kAppProp->version())!=0)
                           k->update = true;
                   } else if (e.tagName() == "rev") {
                       k->revision = e.text();
                       if (k->revision.compare(kAppProp->revision())!=0)
                           k->update = true;
                   } else if (e.tagName() == "codeName") {
                       k->codeName = e.text();
                   }
               }
               n = n.nextSibling();
        }
    }
}

void KTwitter::formatStatus(QByteArray array)
{
    QString output = "";
    QString name = "";
    QString description = "";
    QString website = "";
    QString followers = "";
    QString image = "";

    QDomDocument doc;

    if (doc.setContent(array)) {

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
                                          int init = text.indexOf("http://");
                                          if (init >= 0) {
                                              int end = text.indexOf(" ", init);
                                              if (end < 0)
                                                  end = text.length();
                                              QString www = text.mid(init, end-init);
                                              QString link = "<a href=\"" + www + "\">";
                                              text.insert(end, "</a>");
                                              text.insert(init, link);
                                          }
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
                                                                        website = "<a href=\"" + e2.text() + "\">";
                                                                        website += e2.text(); 
                                                                        website += "</a>";
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
                       output += text + "\n";
                       output += "       <p/>\n";
                   }
               }

               n = n.nextSibling();
        }
     }

     QString html = "";

     html += "<html>\n";
     html += "<head>\n";
     html += "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html;charset=utf-8\">\n";
     html += "<link rel=\"stylesheet\" type=\"text/css\" href=\"" + QString::fromLocal8Bit(::getenv("KTOON_SHARE")) \
             + "/data/help/css/ktoon.css\" />\n";
     html += "</head>\n";
     html += "<body class=\"ktoon_background1\">\n";
     html += " <table class=\"twitter_base\">\n";
     html += "  <tr>\n";
     html += "  <td class=\"twitter_status\">\n";
     html += "    <table class=\"status_table\">\n";
     html += "     <tr>\n";
     html += "     <td>\n";
     html += "      &nbsp;&nbsp;\n";
     html += "      <img class=\"twitter_logo\" src=\"" + QString::fromLocal8Bit(::getenv("KTOON_SHARE")) \
             + "/data/help/images/twitter01.png\" alt=\"ktoon_net\"/>\n";
     html += "      <font class=\"twitter_headline\" >&nbsp;&nbsp;ktoon_net</font>\n";
     html += "      <br/>\n";
     html += "     </td>\n";
     html += "     </tr>\n";
     html += "     <tr>\n";

     QString css = "ktoon_version";  
     if (k->update)
         css = "ktoon_update"; 

     html += "     <td class=\"" + css + "\">\n";
     html += "       <center>" + tr("Latest version") + ": <b>" + k->version + "</b> &nbsp;&nbsp;&nbsp;" + tr("Revision") + ": <b>" + k->revision + "</b> &nbsp;&nbsp;&nbsp;" + tr("Code Name") + ": <b>" + k->codeName + "</b>";

     if (k->update)
         html += "&nbsp;&nbsp;&nbsp;<b>[</b> <a href=\"http://www.ktoon.net\">" + tr("Update here!") + "</a>  <b>]</b>"; 

     html += "</center>\n";

     html += "     </td>\n";
     html += "     </tr>\n";
     html += "     <tr>\n";
     html += "     <td>\n";
     html += "     <b>" + tr("This is what's happening") + ":</b>\n";
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
     html += "          <b>" + tr("Name") + ":</b> " + name + "<br/>\n";
     html += "          <b>" + tr("Description") + ":</b> " + description + "<br/>\n";
     html += "          <b>" + tr("Website") + ":</b> " +  website + "<br/>\n";
     html += "          <b>" + tr("Followers") + ":</b> " + followers + "<br/>\n";
     html += "     </td></tr>\n";
     html += "     </table>\n";
     html += "  </td>\n";
     html += "  </tr>\n";
     html += "</table>\n";
     html += "</body>\n";
     html += "</html>";

     QString twitterPath = QDir::homePath() + "/." + QCoreApplication::applicationName() + "/twitter.html";
     QFile file(twitterPath);
     file.open(QIODevice::WriteOnly);

     QByteArray data = html.toUtf8();
     file.write(data, qstrlen(data));
     file.close();

     // FIXME: This class doesn't close its network request correctly / refactoring required  
     // delete k->reply;
     // delete k->manager;

     emit pageReady();
}
