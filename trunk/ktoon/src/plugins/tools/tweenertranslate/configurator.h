//
// C++ Interface: configurator
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QFrame>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class Configurator : public QFrame
{
	Q_OBJECT
	public:
		Configurator(QWidget *parent = 0);
		~Configurator();
		
	private slots:
		void emitOptionChanged(int option);
		
	signals:
		void clikedCreatePath();
		void clikedSelect();
		void clikedApplyTweener();
		
	private:
		struct Private;
		Private *const d;
};

#endif
