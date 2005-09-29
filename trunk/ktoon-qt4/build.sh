#!/bin/bash

# Script for update the .pro files && build the app
# Author: Krawek
# Version: 0.0.4

APPNAME="KToon"
APPVER="0.8alpha-svn"

MAKE=`which make` 
QMAKE=`which qmake`

KTOON_GLOBAL_ENV=/etc/ktoon.env
KTOON_LOCAL_ENV=~/.ktoon.env

export PATH=/bin:/usr/bin:/usr/local/bin:/sbin:/usr/sbin:/usr/local/sbin

if [ $# -eq 0 ]
then
	if [ ! $KTOON_HOME ]
	then
		echo "Usage: `basename $0` -p [prefix]"
		exit 0
	fi
fi

set - `getopt ":p" "$@"`

if [ "$1" == "-p" ]
then
	KTOON_HOME=$3 
fi

shift
export KTOON_HOME
export INSTALL_ROOT=$KTOON_HOME

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
	echo -en "$RED $1 $NULLC $2 "
}

function verifyEnv()
{
	if [ ! -x $QMAKE -a $QMAKE ]
	then
		qperror "You need \"qmake\" in your path!"
		exit -1
	fi
	
	if [ ! -x $MAKE -a $MAKE ]
	then
		qperror "You doesn't have installed \"make\" tool"
		exit -1
	fi
}

function detectQtVersion()
{
	LINE=`$QMAKE -v | grep "Using Qt"`
	if [ "$LINE" != "" ]
	then
		echo "Using Qt4"
	else
		echo "Using Qt3"
	fi
}

detectQtVersion

function openglCC
{
	touch ktoon.config
}

function qtCC
{
	cat << _EOF_ > ktoon.config
DEFINES += NO_OPENGL
_EOF_
}

function buildenv()
{
	if [ `whoami` == "root" ]
	then
		echo export KTOON_HOME=$KTOON_HOME > $KTOON_GLOBAL_ENV
		echo export PATH=\$PATH:$KTOON_HOME/bin >> $KTOON_GLOBAL_ENV
	else
		echo export KTOON_HOME=$KTOON_HOME > $KTOON_LOCAL_ENV
		echo export PATH=\$PATH:$KTOON_HOME/bin >> $KTOON_LOCAL_ENV
	fi
}

function addMenuEntry()
{
	if [ `whoami` == "root" ]
	then
		if [ -d /usr/share/applications ]
		then
			echo "Add menu entry to /usr/share/applications"
			cp ktoon.desktop /usr/share/applications
		fi
	else
		mkdir -p $HOME/.kde/share/applnk/Graphics
		echo "Add menu entry to $HOME/.kde/share/applnk/Graphics/ktoon.desktop"
		cp ktoon.desktop $HOME/.kde/share/applnk/Graphics/ktoon.desktop
	fi
}

function ktinstall()
{
	make install
	
	if [ $(basename `echo $SHELL`) == "bash" ]
	then
		buildenv
		if [ `whoami` == "root" ]
		then
			if [ `grep -c "source $KTOON_GLOBAL_ENV" /etc/profile` -eq 0 ]
			then
				echo "source $KTOON_GLOBAL_ENV" >> /etc/profile
			fi
#install menu entry
		else
			if [ -f ~/.bashrc ]
			then
				if [ `grep -c "source $KTOON_LOCAL_ENV" ~/.bashrc` -eq 0 ]
				then
					echo "source $KTOON_LOCAL_ENV" >> ~/.bashrc
				fi
				
			else
				if [ -f ~/.bash_profile ]
				then
					if [ `grep -c "source $KTOON_LOCAL_ENV" ~/.bash_profile` -eq 0 ]
					then
						echo "source $KTOON_LOCAL_ENV" >> ~/.bash_profile
					fi
				else
					echo "source $KTOON_LOCAL_ENV" >> ~/.bashrc
				fi
			fi
 		fi
 		
 		addMenuEntry
	fi
}

verifyEnv

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

# qpinfo "Compiling ming"

#if [ ! -s $KTHOME/ming/Makefile ]
#then
#	echo "Removing " $KTHOME/ming/Makefile
#	rm $KTHOME/ming/Makefile 2> /dev/null >/dev/null
#	ln -s $KTHOME/ming/Makefile-real $KTHOME/ming/Makefile
#fi

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

qpelec '-' "Do you wants use opengl (y/n)? "
read UOG

case $UOG in
	y) openglCC ;;
	yes) openglCC ;;
	si) openglCC ;;
	s) openglCC ;;
	Y) openglCC ;;
	S) openglCC ;;
	*) qtCC ;;
esac

$QMAKE

qpinfo "Compiling $APPNAME $APPVER"

#$MAKE clean

$MAKE || exit 1

qpinfo "Compiling successful!"

qpelec '-' "Do you wants install ktoon in \"$KTOON_HOME\" (y/n)? "
read SN

case $SN in
	y) ktinstall;;
	yes) ktinstall;;
	si) ktinstall;;
	s) ktinstall;;
esac


