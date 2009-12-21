
STORE_DIR = ../../../src/store 
LIBKTOON_DIR = ../../../src/libktoon 
LIBBASE_DIR = ../../../src/libbase
#KLIB_DIR = ../../../src/klib
EXPORT_DIR = ../../../src/components/export

include($$STORE_DIR/store.pri)
include($$LIBKTOON_DIR/libktoon.pri)
include($$LIBBASE_DIR/libbase.pri)
#include($$KLIB_DIR/klib.pri)
include($$EXPORT_DIR/export.pri)

include(../../../ktconfig.pri)

INCLUDEPATH += ../../../src/shell

QT += xml

