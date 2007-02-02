//
// C++ Implementation: infousers
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "usersinfo.h"
#include "../users/user.h"

namespace Packages{

UsersInfo::UsersInfo(): QDomDocument()
{
	QDomElement root = createElement("usersinfo");
	root.setAttribute("version", 0);
	appendChild(root);
}


UsersInfo::~UsersInfo()
{
}


void UsersInfo::addUser(const Users::User *user)
{
	QDomElement userE = createElement("user");
	userE.setAttribute("login",  user->login());
	userE.setAttribute("name",  user->name());
	firstChild().appendChild(userE);
}

}
