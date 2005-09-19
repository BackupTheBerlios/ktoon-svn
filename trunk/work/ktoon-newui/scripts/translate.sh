#!/bin/sh
# Author: krawek [at] toonka.com
# Version: 0.1
# This help to generate the ts file

if [ ! "$0" == "translate.sh" ]
then
	echo "Use: sh translate.sh"
	exit 0
fi

KTDIR=$(pwd)/..
KTTS=/tmp/ktoon_untranslated.ts
TRPRO=trans.pro

echo "Your translate file will be generated in " $KTTS

echo -e "PREFIX =\n" > $TRPRO
echo -e "ROOT =\n" >> $TRPRO
echo -e "\n" >> $TRPRO

HEADERS=$(find $KTDIR -name "*.h" -printf "%p ")
SOURCES=$(find $KTDIR -name "*.cpp" -printf "%p ")

echo "HEADERS = $HEADERS" >> $TRPRO
echo "SOURCES = $SOURCES" >> $TRPRO

echo -e "TRANSLATIONS = $KTTS" >> $TRPRO

lupdate -noobsolete $TRPRO
rm -fR $TRPRO

LING=`which linguist`

if [ -x $LING -a $LING ]
then
	linguist $KTTS &
else
	echo "* Please install linguist"
fi

cd $KTDIR

echo "File generated!"

