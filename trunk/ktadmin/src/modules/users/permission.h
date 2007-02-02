//
// C++ Interface: permission
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef USERSPERMISSION_H
#define USERSPERMISSION_H

#include <QString>
namespace Users {

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class Permission
{
	public:
		Permission();
		Permission(const QString &module, bool read, bool write);
		Permission(const Permission & copy);
		~Permission();
		
		void setModule(const QString &module);
		void setRead(bool read);
		void setWrite(bool write);
		
		QString module() const;
		bool read();
		bool write();
		
		Permission *operator=(const Permission &copy);
		
	private:
		struct Private;
		Private * const d;
};

}

#endif
