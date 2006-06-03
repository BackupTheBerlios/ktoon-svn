/***************************************************************************
 *   Copyright (C) 2005 by Alexander Dymo                                  *
 *   adymo@kdevelop.org                                                    *
 *   David Cuadrado 2005                                                   *
 *   krawek@toonka.com                                                     *
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

class DDockWindow;
class DDockInternalWidget;
class DDockWidgetLayout;

namespace Ideal {
	class Button;
	class ButtonBar;
}

class DDockWindow : public QDockWidget
{
	Q_OBJECT
	public:
		enum Position { Bottom, Left, Right };
		
		DDockWindow(QWidget *parent, Position position);
		virtual ~DDockWindow();
		void addWidget(const QString &title, QWidget *widget);
		
// 		QSize sizeHint() const;
// 		QSize minimumSizeHint() const;
		DDockInternalWidget *centralWidget();
		
	private slots:
		void setFloatingOption(bool);
		
	protected:
		void closeEvent(QCloseEvent *e);
		
	private:
		DDockInternalWidget *m_centralWidget;
};

class DDockInternalWidget : public QWidget {
	Q_OBJECT
	public:
    
		DDockInternalWidget(QWidget *parent, DDockWindow::Position position);
		virtual ~DDockInternalWidget();
    
		virtual void setExpanded(bool v);
		bool visible() const { return m_visible; }
    
		virtual void addWidget(const QString &title, QWidget *widget);
		virtual void raiseWidget(QWidget *widget);
		/**Removes the widget from dock. Does not delete it.*/
		virtual void removeWidget(QWidget *widget);
    
		virtual void hideWidget(QWidget *widget);
		virtual void showWidget(QWidget *widget);
    
		virtual QWidget *currentWidget() const;
    
		DDockWindow::Position position() const { return m_position; }
		
		void setSeparator(QWidget *separator);
    
	signals:
		void fixedExtentHeight(int);
		void fixedExtentWidth(int);

	private slots:
		void selectWidget();
		void selectWidget(Ideal::Button *button);
		void dialoged(int );

	private:
		void shrink();
		
	protected:
		virtual void loadSettings();
		virtual void saveSettings();
		
	protected:
    
		Ideal::ButtonBar *m_bar;
		QStackedWidget *m_widgetStack;
    
		QMap<Ideal::Button*, QWidget*> m_widgets;
		QMap<QWidget*, Ideal::Button*> m_buttons;
		
		QWidget * m_separator;

	private:
		DDockWindow::Position m_position;
		bool m_visible;
		QString m_name;
	
		Ideal::Button *m_toggledButton;
		QBoxLayout *m_internalLayout;
		QWidget *m_container;
};

#endif
