#!/bin/bash

# Script for update the .pro files && build the app
# Author: Krawek
# Version: 0.0.2

APPNAME="KToon"
APPVER="0.7.4svn"

MAKE=`which make`
QMAKE=`which qmake`
KTHOME=$(pwd)

# Look & feel

GREEN='\033[1;32m'
RED='\033[9;31m'
NULLC='\033[0;0m'

function qpinfo ()
{
	echo -e $GREEN '*' $NULLC $1
}

function qperror ()
{
	echo -e $RED '*' $NULLC $1
}

function qpelec ()
{
	echo -e $RED $1 $NULLC $2
}


DIRS=`find . -type d`

if [ ! $QMAKE ]
then
	if [ -x "$QTDIR/bin/qmake" ]
	then
		QMAKE="$QTDIR/bin/qmake"
	else
		qperror "You don't have qmake in your PATH, or doesn't have Qtlibs, please install it and try"
		exit 1
	fi
fi

# compile ming

qpinfo "Compiling ming"

if [ ! -s $KTHOME/ming/Makefile ]
then
	echo "Removing " $KTHOME/ming/Makefile
	rm $KTHOME/ming/Makefile 2> /dev/null >/dev/null
	ln -s $KTHOME/ming/Makefile-real $KTHOME/ming/Makefile
fi

qpinfo "Update dirs"
for i in $DIRS
do
	cd $i
	if [ -e *.pro ]
	then
		qpinfo "Updating $i..."
		$QMAKE 2> /dev/null >/dev/null
	fi
	cd - 2> /dev/null >/dev/null
done

$QMAKE

qpinfo "Compiling $APPNAME $APPVER"

$MAKE || exit 1

qpinfo "Compiling successful!"

