# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/components/brushes
# Destiono es una biblioteca:  

HEADERS += brushes.h \
           ktdisplaybrush.h \
           ktbrusheslist.h \
           ktbrushwidget.h 
SOURCES += brushes.cpp \
           ktdisplaybrush.cpp \
           ktbrusheslist.cpp \
           ktbrushwidget.cpp 
INCLUDEPATH += ../../../src/dialogs \
../../../src/store \
../../../src/lib \
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
QT += qt3support xml opengl
