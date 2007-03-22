
#include <QCoreApplication>
#include <QDesktopServices>
#include <QGraphicsView>
#include <QUrl>

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	QGraphicsView *view = new QGraphicsView;
	QDesktopServices::openUrl(QUrl("file:///") );

	return 0;
}


