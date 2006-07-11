/***************************************************************************
 *   Copyright (C) 2005 by Alexander Dymo                                  *
 *   adymo@kdevelop.org                                                    *
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef DDOCKWINDOW_H
#define DDOCKWINDOW_H

#include <QDockWidget>
#include <QList>
#include <QBoxLayout>
#include <QMap>
#include <QVBoxLayout>

#include <QDockWidget>
#include <QPushButton>

class QBoxLayout;
class QToolButton;
class QStackedWidget;

class QComboBox;
class QSettings;

class DiDockWidget;
class DiDockInternalWidget;
class DiDockWidgetLayout;

namespace Ideal {
	class DiButton;
	class DiButtonBar;
}

class DiDockWidget : public QDockWidget
{
	Q_OBJECT
	public:
		enum Position { Bottom, Left, Right };
		
		DiDockWidget(QWidget *parent, Position position);
		virtual ~DiDockWidget();
		void addWidget(const QString &title, QWidget *widget);
		
// 		QSize sizeHint() const;
// 		QSize minimumSizeHint() const;
		DiDockInternalWidget *centralWidget();
		
	private slots:
		void setFloatingOption(bool);
		
	protected:
		void closeEvent(QCloseEvent *e);
		
	private:
		DiDockInternalWidget *m_centralWidget;
};

class DiDockInternalWidget : public QWidget 
{
	Q_OBJECT
	public:
    
		DiDockInternalWidget(QWidget *parent, DiDockWidget::Position position);
		virtual ~DiDockInternalWidget();
    
		virtual void setExpanded(bool v);
		bool visible() const { return m_visible; }
    
		virtual void addWidget(const QString &title, QWidget *widget);
		virtual void raiseWidget(QWidget *widget);
		/**Removes the widget from dock. Does not delete it.*/
		virtual void removeWidget(QWidget *widget);
    
		virtual void hideWidget(QWidget *widget);
		virtual void showWidget(QWidget *widget);
    
		virtual QWidget *currentWidget() const;
    
		DiDockWidget::Position position() const { return m_position; }
		
	signals:
		void fixedExtentHeight(int);
		void fixedExtentWidth(int);

	private slots:
		void selectWidget();
		void selectWidget(Ideal::DiButton *button);
		void dialoged(int );

	private:
		void shrink();
		
	protected:
		virtual void loadSettings();
		virtual void saveSettings();
		
	protected:
    
		Ideal::DiButtonBar *m_bar;
		QStackedWidget *m_widgetStack;
    
		QMap<Ideal::DiButton*, QWidget*> m_widgets;
		QMap<QWidget*, Ideal::DiButton*> m_buttons;

	private:
		DiDockWidget::Position m_position;
		bool m_visible;
		QString m_name;
	
		Ideal::DiButton *m_toggledButton;
		QBoxLayout *m_internalLayout;
		QWidget *m_container;
};

#endif
