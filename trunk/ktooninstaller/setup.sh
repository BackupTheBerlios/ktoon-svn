#!/bin/bash

# Author: David Cuadrado
# Email: krawek [at] toonka [dot] com

WORKINGDIR=$(dirname $0)
SETUP_BINARY=$WORKINGDIR/bin/setup.bin
KTOON_TMP_HOME=/tmp/ktoon.home-$RANDOM
KTOON_DIST_FILE=$WORKINGDIR/dist/ktoon_home.tar.bz2
DIRTMP=/tmp/ktoon.$RANDOM/

function verify()
{
	if [ ! -x $SETUP_BINARY ]
	then
		echo "Can't find setup.bin"
		exit -1
	fi
}

function end()
{
	if [ -d $KTOON_TMP_HOME ]
	then
		rm -rf $KTOON_TMP_HOME
	fi
	if [ -d $DIRTMP ]
	then
		rm -rf $DIRTMP
	fi
}

function unpack_ktoon_home()
{
	echo "Unpacking $KTOON_DIST_FILE"

	if [ ! -d $DIRTMP ]
	then
		mkdir -p $DIRTMP
	fi
	
	if [ -f $KTOON_DIST_FILE ]
	then
		tar xjf $KTOON_DIST_FILE -C $DIRTMP
		
		DIR_EXTRACTED=$(ls $DIRTMP)
		mv $DIRTMP/$DIR_EXTRACTED $KTOON_TMP_HOME
		
		export LD_LIBRARY_PATH=$KTOON_TMP_HOME/lib
	fi

	if [ ! -d $KTOON_TMP_HOME ]
	then
		echo "Fatal error"
		exit -1
	fi
	
}

# Clean environment
if [ -f /etc/profile ]
then
	. /etc/profile
fi

unpack_ktoon_home
verify

$SETUP_BINARY $KTOON_TMP_HOME

end

