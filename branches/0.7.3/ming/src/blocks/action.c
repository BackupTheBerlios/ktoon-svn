/*
    Ming, an SWF output library
    Copyright (C) 2002  Opaque Industries - http://www.opaque.net/

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* $Id: action.c,v 1.10 2004/03/30 15:07:28 strk Exp $ */

#include "action.h"
#include "outputblock.h"
#include "libming.h"


SWFAction newSWFAction()
{
	return newSWFOutputBlock(newSWFOutput(), SWF_DOACTION);
}


void destroySWFAction(SWFAction action)
{
	destroySWFOutputBlock(action);
}


SWFAction newSWFAction_fromOutput(SWFOutput out)
{
	return newSWFOutputBlock(out, SWF_DOACTION);
}


/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 */
