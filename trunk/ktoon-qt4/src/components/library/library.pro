# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/components/library
# Destiono es una biblioteca:  

INCLUDEPATH += ../../../src/store \
               ../../../src/lib \
               ../../../src/images/images \
               ../../../src/qt/cameras \
               ../../../src/qt/tools \
               ../../../src/qt/draw \
               ../../../src/opengl/draw \
               ../../../src/opengl/tools \
               ../../../src/opengl/cameras 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += library.h \
           symbolitem.h \
           symboltable.h \
           symbolview.h 
SOURCES += library.cpp \
           symbolitem.cpp \
           symboltable.cpp \
           symbolview.cpp 
QT += qt3support xml opengl
