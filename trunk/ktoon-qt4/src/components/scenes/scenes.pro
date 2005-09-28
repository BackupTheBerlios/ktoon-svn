# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/components/scenes
# Destiono es una biblioteca:  

HEADERS += scenes.h \
           ktsceneslist.h 
SOURCES += scenes.cpp \
           ktsceneslist.cpp 
INCLUDEPATH += ../../../src/dialogs \
../../../src/qt/cameras \
../../../src/qt/tools \
../../../src/qt/draw \
../../../src/opengl/draw \
../../../src/opengl/tools \
../../../src/opengl/cameras \
../../../src/store \
../../../src/lib
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
QT += xml opengl qt3support
