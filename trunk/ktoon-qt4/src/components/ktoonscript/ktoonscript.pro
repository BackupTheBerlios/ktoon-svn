# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/components/ktoonscript
# Destiono es una biblioteca:  

INCLUDEPATH += ../../../src/lib \
               ../../src/lib 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += ktoonscript.h \
           ktseditor.h \
           ktsfunctionview.h 
SOURCES += ktoonscript.cpp \
           ktseditor.cpp \
           ktsfunctionview.cpp 
QT += qt3support xml opengl
