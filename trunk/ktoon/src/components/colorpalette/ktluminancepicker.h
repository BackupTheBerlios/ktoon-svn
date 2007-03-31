#ifndef KTLUMINACEPICKER_H
#define KTLUMINACEPICKER_H


#include <QPaintEvent>
#include <QPixmap>
#include <QMouseEvent>
#include <QWidget>

class KTLuminancePicker : public QWidget
{
	Q_OBJECT
	public:
		KTLuminancePicker(QWidget* parent=0);
		~KTLuminancePicker();
		int value();
	public slots:
		void setCol(int h, int s, int v);
		void setCol(int h, int s);
		void setVal(int v);

	signals:
		void newHsv(int h, int s, int v);

	protected:
		void paintEvent(QPaintEvent*);
		void mouseMoveEvent(QMouseEvent *);
		void mousePressEvent(QMouseEvent *);

	private:
		enum { foff = 3, coff = 4 }; //frame and contents offset
		
		struct Private;
		Private *const d;
		
		int y2val(int y);
		int val2y(int val);

};

#endif

