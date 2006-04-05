#!/bin/bash

# Script that update the .pro files && build the app
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
	
	echo "Send the file $LOG_FILE to $EMAIL"

	return -1
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
}

function openglCC
{
	PARAMETERS="$PARAMETERS DEFINES+=KT_OPENGL"
}

function qtCC
{
	PARAMETERS="$PARAMETERS "
}

function updateMakefiles()
{
#	qpinfo "Updating Makefiles with parameters ${PARAMETERS}"
	echo -n "Wait a moment."
	DIRS=`find . -type d`
	for i in $DIRS
	do
		cd $i
		if [ -e *.pro ]
		then
# 			qpinfo "Updating $i..."
			echo -n "."

			$QMAKE ${PARAMETERS} 2> /dev/null >/dev/null
		fi
		cd - 2> /dev/null >/dev/null
	done
	echo
}

function addMenuEntry()
{
	if [ `whoami` == "root" ]
	then
		mkdir -p /usr/share/applications/Graphics
		if [ -d /usr/share/applications/Graphics ]
		then
			cp ktoon.desktop /usr/share/applications/Graphics &&
			qpinfo "Added menu entry to /usr/share/applications"
		fi
	else
		mkdir -p $HOME/.local/share/applications/Graphics/
		cp ktoon.desktop $HOME/.local/share/applications/Graphics/ktoon.desktop && 
		qpinfo "Added menu entry to $HOME/.local/share/applications/Graphics/ktoon.desktop"
	fi
}

function createLauncher() {
	LAUNCHER=${KTOON_HOME}/ktoon
	echo "#!/bin/bash" > $LAUNCHER
	echo "export KTOON_HOME=${KTOON_HOME}" >> $LAUNCHER
	echo "export LD_LIBRARY_PATH=$KTOON_HOME/lib" >> $LAUNCHER
	echo "exec $KTOON_HOME/bin/ktoon \$*" >> $LAUNCHER
	chmod 755 $LAUNCHER
}

function ktinstall()
{
	$MAKE install 2>> $LOG_FILE >/dev/null || fails "Error while install!. Please send the file $LOG_FILE to $EMAIL"

	createLauncher
 	addMenuEntry
}




function main()
{
	echo
	echo `date` >> $LOG_FILE
	qpinfo "###################################"
	if [ $OPTION_GL -eq 1 ]
	then
		qpinfo "-> Using OpenGL version"
		openglCC
	else
		qpinfo "-> Using NO-OpenGL version"
		qtCC
	fi
	
	if [ $OPTION_NODEBUG -eq 1 ]
	then
		qpinfo "-> Using NO-DEBUG support"
		PARAMETERS="$PARAMETERS DEFINES+=KT_NODEBUG"
	else
		qpinfo "-> Using DEBUG support"
	fi
	
	qpinfo "###################################"
	echo
	
	if [ $ASSUME_YES -ne 1 ]
	then
		qpelec "This is right (y/n)? "
		read UOG
	
		case $UOG in
			n|N|no|NO) exit -1 ;;
		esac
	fi
	
	updateMakefiles
	$QMAKE
	
	if [ $ASSUME_YES -ne 1 ]
	then
		qpelec "Do you wants clean the project (y/n)? "
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
	
# 	$MAKE 2> /dev/null > /dev/null || ( fails "Error while compiling, please try to run \"make\" manually"; exit 1 )
	
	echo > $STAT_FILE
	END=0
	INIT_TIME=`date +%s`
	( ( $MAKE  >/dev/null 2>> $LOG_FILE || fails "Error while compile!" ) && echo END=1 > $STAT_FILE ) & 
	
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
	
	qpinfo "Compiling successful! Time: `expr $END_TIME - $INIT_TIME` seconds"
	
	if [ $ASSUME_YES -ne 1 ]
	then
		qpelec "Do you wants install ktoon in \033[0;41m\"$KTOON_HOME\"\033[0;0m (y/n)? "
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
	echo "	-p,--prefix [PREFIX]"
	echo "	   Set the prefix"
	echo "	-d,--debug"
	echo "	   Activate debug"
	echo "	-o,--use-opengl"
	echo "	   Compile the opengl version"
	echo "	-Y,--assume-yes"
	echo "	   Assume yes"
	echo "	-h,--help"
	echo "	   This message"
	echo
	exit 0
}

if [ $# -eq 0 ]
then
	if [ ! $KTOON_HOME ]
	then
		usage
	fi
fi


TEMP=`getopt -o p:doYh:: --long prefix:,help,debug,use-opengl::,assume-yes -n "$0" -- "$@"`

eval set -- "$TEMP"
while [ true ]
do
        case "$1" in
		-h|--help) usage; shift;;
		-p|--prefix) KTOON_HOME=$2; shift 2 ;;
		-d|--debug) OPTION_NODEBUG=0; shift ;;
		-o|--use-opengl) OPTION_GL=1; shift ;;
		-Y|--assume-yes) ASSUME_YES=1; OPTION_NODEBUG=1; OPTION_GL=1; shift ;; 
		--) shift ; break ;;
		*) break;
	esac
done

if [ ! -f config.h ]
then
	qperror "Please run ./configure first"
	exit -1
fi


verifyEnv
detectQtVersion

if [ $OPTION_GL -eq -1 ]
then
	qpelec "Do you wants compile opengl version (y/n)? "
	read UOG

	case $UOG in
		y|Y|yes|YES|si|SI|s|S) OPTION_GL=1 ;;
		*) OPTION_GL=0 ;;
	esac
fi

if [ $OPTION_NODEBUG -eq -1 ]
then
	qpelec "Do you wants debug support (y/n)? "
	read UOG
	
	case $UOG in
		n|N|no|NO) OPTION_NODEBUG=1 ;;
	esac
fi

export KTOON_HOME
export INSTALL_ROOT=$KTOON_HOME

main

