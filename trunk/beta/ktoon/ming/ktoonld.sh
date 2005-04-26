#!/bin/bash
# Autor krawek@toonka.com

# Description: This file add the links flags to the project for use -lpng, -lgif -lungif, -lz or not.

MING=$(pwd)
CORE=$(pwd)/../src/core/

cd $CORE
qmake $CORE/core.pro

if [ `cat $CORE/core.pro | grep -c ^LIBS` == 1 ]
then
	grep ^PNGLIB $MING/Makefile.config | awk '{ print "LIBS += " $3 }'  >> $CORE/core.pro
	
	grep ^GIFLIB $MING/Makefile.config | awk '{ print "LIBS += " $3 }' >> $CORE/core.pro

	grep ^ZLIB $MING/Makefile.config | awk '{ print "LIBS += " $3 }' >> $CORE/core.pro
fi

cd -
