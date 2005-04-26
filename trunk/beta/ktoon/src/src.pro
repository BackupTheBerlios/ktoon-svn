# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src
# Destiono es un subdirectorio de proyecto 

TRANSLATIONS += trans/ktoon_es.ts \
                trans/ktoon_fr.ts \
                trans/ktoon_tr.ts 
SUBDIRS += brushes \
           cameras \
           color \
           drawqt \
           exposure \
           gl \
           library \
           modal \
           properties \
           scenes \
           store \
           timeline \
           tools \
           images \
           core 
LIBS += ../lib/libming.a
INCLUDEPATH += ../src
MOC_DIR = ./moc
OBJECTS_DIR = ./obj
QMAKE_LIBDIR = ../lib/
TARGET = ../bin/ktoon
CONFIG += release \
warn_on \
qt \
opengl \
x11
TEMPLATE = subdirs
