
STORE_DIR = ../../../src/store 
LIBKTOON_DIR = ../../../src/libktoon 
LIBBASE_DIR = ../../../src/libbase
KLIB_DIR = ../../../src/klib

include($$STORE_DIR/store.pri)
include($$LIBKTOON_DIR/libktoon.pri)
include($$LIBBASE_DIR/libbase.pri)
include($$KLIB_DIR/klib.pri)


include(../../../ktconfig.pri)

QT += xml

