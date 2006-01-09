#!/bin/bash

. /etc/profile

if [ ! $KTOON_HOME ]
then
	echo -n "KTOON_HOME="
	read KTOON
else
	KTOON=$KTOON_HOME
fi

if [ ! -d $KTOON_HOME ]
then
	echo "Error, $KTOON_HOME is not directory"
	exit -1
fi

#if [ ! -d $QTDIR ]
#then
	echo -n "QTDIR="
	read QTDIR
#fi

if [ ! -d $QTDIR ]
then
	echo "Error, $QTDIR is not a directory"
	exit -1
fi

echo -n "Output dir="
read OUTPUT

if [ ! -d $OUTPUT ]
then
	mkdir -p $OUTPUT || exit -1
fi

cp -r $KTOON $OUTPUT/ktoon_home/ || exit 1
cp -r $QTDIR/lib/libQt*.so* $OUTPUT/ktoon_home/lib/ || exit 1

rm -f $OUTPUT/ktoon_home/lib/libQtAssistant* $OUTPUT/ktoon_home/lib/libQtDesigner* || exit 1

cp -r bin/ setup.sh $OUTPUT

cd $OUTPUT
mkdir dist

tar cvjf ktoon_home.tar.bz2 ktoon_home 
mv ktoon_home.tar.bz2 dist
rm -rf ktoon_home

cd -
echo "Distribution generated in $OUTPUT"

