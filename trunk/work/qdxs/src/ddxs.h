#ifndef DDXS_H
#define DDXS_H

#include <QObject>
#include <QStringList>

class QDomNode;
class DSoap;

class DDxs : public QObject
{
	Q_OBJECT;
	public:
		DDxs();
		~DDxs();
		void setEndpoint(QString endpoint);

		void call_info();
		void call_categories();
		void call_comments(int id);
		void call_history(int id);
		void call_removal(int id);
		void call_subscription(int id, bool subscribe);
		void call_comment(int id, QString comment);
		void call_rating(int id, int rating);

	signals:
		void signalInfo(const QString &provider, const QString &server, const QString &version);
		void signalCategories(QStringList categories);
		void signalComments(QStringList comments);
		void signalHistory(QStringList entries);
		void signalRemoval(bool success);
		void signalSubscription(bool success);
		void signalComment(bool success);
		void signalRating(bool success);
		void signalFault();

	private slots:
		void slotResult(const QDomNode &node);

	private:
		DSoap *m_soap;
		QString m_endpoint;
};


#endif
