# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/store
# Destiono es una biblioteca:  

HEADERS += ktkeyframe.h \
           agraphiccomponent.h \
           ktlayer.h \
           ktprojectmanager.h \
           ktscene.h \
           ktdocument.h \
           ktserializableobject.h 
SOURCES += ktkeyframe.cpp \
           agraphiccomponent.cpp \
           ktlayer.cpp \
           ktprojectmanager.cpp \
           ktscene.cpp \
           ktdocument.cpp \
           ktserializableobject.cpp 
INCLUDEPATH += ../../src/modules/opengl/draw \
../../src/modules/opengl/tools \
../../src/modules/opengl/cameras \
../../src/core \
../../src/dialogs \
../../src/store \
../../src/lib \
../../src/images/sequences \
../../src/images/icons \
../../src/images/cursors \
../../src/images/images
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
QT += qt3support xml opengl
