#include <QFrame>

#ifndef KTCOLORPICKER_H
#define KTCOLORPICKER_H

class KTColorPicker : public QFrame
{
	Q_OBJECT
	public:
		KTColorPicker(QWidget* parent);
		~KTColorPicker();

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
		int m_hue;
		int m_sat;

		QPoint colPt();
		int huePt(const QPoint &pt);
		int satPt(const QPoint &pt);
		void setCol(const QPoint &pt);

		QPixmap *m_pix;
};

#endif

