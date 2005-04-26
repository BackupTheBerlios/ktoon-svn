#!/bin/bash
# Autor krawek@toonka.com

# Description: This file add the links flags to the project for use -lpng, -lgif -lungif, -lz or not.

CORE=../src/core/

cd $CORE
qmake $CORE/core.pro

if [ `cat $CORE/core.pro | grep -c ^LIBS` == 1 ]
then
	grep ^PNGLIB Makefile.config | awk '{ print "LIBS += " $3 }'  >> $CORE/core.pro
	
	grep ^GIFLIB Makefile.config | awk '{ print "LIBS += " $3 }' >> $CORE/core.pro

	grep ^ZLIB Makefile.config | awk '{ print "LIBS += " $3 }' >> $CORE/core.pro
fi

cd -
