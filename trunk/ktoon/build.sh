#!/bin/bash

# Script for updating the .pro files and to build KToon 
# Author: David Cuarado krawek [at] gmail [dot] com
# Version: 0.5.0

PARAMETERS=""

OPTION_NODEBUG=-1
OPTION_GL=-1
ASSUME_YES=0

STAT_FILE=/tmp/ktoon_stat_file-$RANDOM

. scripts/global

function fails ()
{
	# Stop thread
	echo 'END=-1' > $STAT_FILE

	echo 
	qperror $*
	echo "---- dmesg ------------" >> $LOG_FILE
	echo `dmesg | tail -n 10` >> $LOG_FILE
	echo "---- uname ------------" >> $LOG_FILE
	echo `uname -a` >> $LOG_FILE
	
	echo "---- config.h ---------" >> $LOG_FILE
	cat config.h >> $LOG_FILE
	
	echo "---- ktconfig.pri -----" >> $LOG_FILE
	cat ${KT_CONFIG} >> $LOG_FILE

	echo "---- gcc --------------" >> $LOG_FILE
	echo `g++ --version` >> $LOG_FILE
	echo "-----------------------" >> $LOG_FILE
	
	echo "There was an error in the compilation process."
        echo "To fix it, please, send the file $LOG_FILE to $EMAIL"

	return -1
}


function verifyEnv()
{
	if [ ! -x $QMAKE -a $QMAKE ]
	then
		qperror "Program \"qmake\" is missing in your path variable!"
                qperror "Have you installed qmake?"
		exit -1
        else
                VERSION=`$QMAKE -query QT_VERSION` 
                if [ ! "$VERSION" = "4.2.0" ]
                then
			qperror "Qt version required to compile KToon is 4.2.0"
                        qperror "Qt version detected: $VERSION"
			qperror "If you have installed Qt 4.2.0, please define the QTDIR variable"
			qperror "pointing to the path where it is installed."
			exit -1
                fi
	fi
	
	if [ ! -x $MAKE -a $MAKE ]
	then
		qperror "Program \"make\" is missing in your path variable!"
                qperror "Have you installed make?"
		exit -1
	fi

        if [ ! "$QTDIR" = "" ]
        then
		if [ -x "$QTDIR/bin/qmake" ]
		then
			QMAKE="$QTDIR/bin/qmake"
		else
			qperror "Ooops... it looks like the \"Qtlibs\" package is missing in your system"
               		qperror "Please install it and try to compile KToon again"
			exit 1
		fi
	fi
}

function openglCC
{
	PARAMETERS="$PARAMETERS DEFINES+=KT_OPENGL"
}

function qtCC
{
	PARAMETERS="$PARAMETERS "
}

function addMenuEntry()
{
	if [ `whoami` == "root" ]
	then
		mkdir -p /usr/share/applications/Graphics
		if [ -d /usr/share/applications/Graphics ]
		then
			cp ktoon.desktop /usr/share/applications/Graphics &&
			qpinfo "Adding Menu entry for KToon in /usr/share/applications"
		fi
	else
		mkdir -p $HOME/.local/share/applications/Graphics/
		cp ktoon.desktop $HOME/.local/share/applications/Graphics/ktoon.desktop && 
		qpinfo "Adding Menu entry for KToon in $HOME/.local/share/applications/Graphics/ktoon.desktop"
	fi
}

function createLauncher() {
	LAUNCHER=${KTOON_HOME}/ktoon
	echo "#!/bin/bash" > $LAUNCHER
	echo "export KTOON_HOME=${KTOON_HOME}" >> $LAUNCHER
	echo "export LD_LIBRARY_PATH=$KTOON_HOME/lib" >> $LAUNCHER
	echo "exec $KTOON_HOME/bin/ktoon.bin \$*" >> $LAUNCHER
	chmod 755 $LAUNCHER
}

function ktinstall()
{
	$MAKE install 2>> $LOG_FILE >/dev/null || fails "Installation process failed. Please, send the file $LOG_FILE to $EMAIL to report errors"

	createLauncher
 	addMenuEntry
}


function main()
{
	echo
	echo `date` >> $LOG_FILE
	
	if [ $OPTION_NODEBUG -eq 1 ]
	then
		qpinfo "-> Using NO-DEBUG support"
		PARAMETERS="$PARAMETERS DEFINES+=D_NODEBUG"
	else
		qpinfo "-> Using DEBUG support"
	fi
	
	qpinfo "###################################"
	echo
	
	if [ $ASSUME_YES -ne 1 ]
	then
		qpelec "Is this correct for you (y/n)? "
		read UOG
	
		case $UOG in
			n|N|no|NO) exit -1 ;;
		esac
	fi
	
	$QMAKE
	
	if [ $ASSUME_YES -ne 1 ]
	then
		qpelec "Do you want to clean the project (y/n)? "
		read UOG
	
		case $UOG in
			y|yes|si|s|Y|S)
				qpinfo "Cleaning..."
				$MAKE clean > /dev/null
			;;
		esac
	else
		$MAKE clean > /dev/null
	fi
	
########################
	qpinfo "Compiling $APPNAME $APPVER..."
	qpinfo "Go for a coffee cup ;)"
	
	echo > $STAT_FILE
	END=0
	INIT_TIME=`date +%s`
	( ( $MAKE  >/dev/null 2>> $LOG_FILE || fails "Compilation Error!" ) && echo END=1 > $STAT_FILE ) & 
	
	while [ $END -eq 0 ]
	do
		if [ -f $STAT_FILE ]
		then
 	       		. $STAT_FILE
 	       		echo -n .
 	       		sleep 2
 	       	else
 	       		wait
 	       		break;
 	       	fi
	done
	END_TIME=`date +%s`
	echo
	rm $STAT_FILE
	
	if [ $END -eq -1 ]
	then
		exit -1
	fi
	
	qpinfo "Compilation process successful! Time: `expr $END_TIME - $INIT_TIME` seconds"
	
	if [ $ASSUME_YES -ne 1 ]
	then
		qpelec "Do you want to install KToon in \033[0;41m\"$KTOON_HOME\"\033[0;0m (y/n)? "
		read SN
	
		case $SN in
			y|yes|si|s) ktinstall ;;
		esac
	else
		ktinstall
	fi
}

function usage()
{
	echo "Usage: `basename $0` [option]"
	echo
	echo "Options: "
	echo "	-q,--quiet"
	echo "	   Enable debug support"
	echo "	-Y,--assume-yes"
	echo "	   Assume yes"
	echo "	-h,--help"
	echo "	   Show help menu"
	echo
	exit 0
}


if [ -f ${CONFIG_STATUS} ]
then
	. ${CONFIG_STATUS}
else
	qperror "Please run ./configure first"
	exit 0
fi

if [ ! $KTOON_HOME ]
then
	qperror "Please run ./configure first"
	exit 0
fi


TEMP=`getopt -o :qoYh:: --long help,quiet,use-opengl::,assume-yes -n "$0" -- "$@"`

eval set -- "$TEMP"
while [ true ]
do
        case "$1" in
		-h|--help) usage; shift;;
		-q|--quiet) OPTION_NODEBUG=1; shift ;;
		-Y|--assume-yes) ASSUME_YES=1; OPTION_NODEBUG=1; shift ;; 
		--) shift ; break ;;
		*) break;
	esac
done

if [ ! -f config.h ]
then
	qperror "Error: you need to run the command \"./configure\" first"
	exit -1
fi


if [ $OPTION_NODEBUG -eq -1 ]
then
	qpelec "Do you want to enable Debug Support (y/n)? "
	read UOG
	
	case $UOG in
		n|N|no|NO) OPTION_NODEBUG=1 ;;
	esac
fi

export KTOON_HOME
# export INSTALL_ROOT=$KTOON_HOME

main


