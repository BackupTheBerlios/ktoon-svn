#!/bin/sh
# Author: krawek [at] toonka.com
# Version: 0.1
# This help to generate the ts file

KTDIR=../../
KTTS=ktoon_new.ts
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

if [ -x `which linguist` ]
then
	linguist $KTTS &
fi

cd $KTDIR

