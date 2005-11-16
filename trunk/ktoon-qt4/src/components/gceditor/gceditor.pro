# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/components/gceditor
# Destiono es una biblioteca:  

HEADERS += ktgceditor.h 
SOURCES += ktgceditor.cpp 
TARGETDEPS += ../../../src/lib/libktoon.so 
LIBS += -lktoon 
INCLUDEPATH += ../../../src/lib 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
QMAKE_LIBDIR = ../../../src/lib 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
