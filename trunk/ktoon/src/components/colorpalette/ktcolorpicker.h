#include <QFrame>

#ifndef KTCOLORPICKER_H
#define KTCOLORPICKER_H

class KTColorPicker : public QFrame
{
	Q_OBJECT
	public:
		KTColorPicker(QWidget* parent);
		~KTColorPicker();
		int hue();
		int sat();
		

	public slots:
		void setCol(int h, int s);
		void setH(int h);
		void setS(int s);

	signals:
		void newCol(int h, int s);

	protected:
		QSize sizeHint() const;
		void paintEvent(QPaintEvent*);
		void mouseMoveEvent(QMouseEvent *);
		void mousePressEvent(QMouseEvent *);

	private:
		struct Private;
		Private *const d;
		
		QPoint colPt();
		int huePt(const QPoint &pt);
		int satPt(const QPoint &pt);
		void setCol(const QPoint &pt);


};

#endif

