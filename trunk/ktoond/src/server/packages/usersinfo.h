//
// C++ Interface: infousers
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PACKAGESINFOUSERS_H
#define PACKAGESINFOUSERS_H

#include <QDomDocument>

namespace Users
{
	class User;
}

namespace Packages{

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 */
class UsersInfo : public QDomDocument
{
	public:
		UsersInfo();
		~UsersInfo();
		
		void addUser(const Users::User *user);

};
}
#endif
