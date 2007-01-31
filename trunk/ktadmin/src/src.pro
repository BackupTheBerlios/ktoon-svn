CONFIG += warn_on \
	  thread \
          qt
TARGET = ../bin/ktadmin

QT += gui
TEMPLATE = subdirs

SUBDIRS += lib modules shell
