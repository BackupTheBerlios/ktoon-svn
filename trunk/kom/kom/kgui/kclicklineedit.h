
#ifndef DCLICKLINEEDIT_H
#define DCLICKLINEEDIT_H

#include <QLineEdit>
#include "kcore/kglobal.h"

class D_GUI_EXPORT DClickLineEdit : public QLineEdit
{
	public:
		DClickLineEdit( const QString &msg, QWidget *parent = 0 );
		~DClickLineEdit();
		
		void setClickMessage( const QString &msg );
		QString clickMessage() const;
		
		virtual void setText( const QString& txt );
	
	protected:
		virtual void paintEvent( QPaintEvent *ev );
		virtual void focusInEvent( QFocusEvent *ev );
		virtual void focusOutEvent( QFocusEvent *ev );
	
	private:
		struct Private;
		Private *const d;

};

#endif


