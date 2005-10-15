# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/store
# Destiono es una biblioteca:  

HEADERS += animation.h \
           camera.h \
           document.h \
           folder.h \
           gldrawing.h \
           item.h \
           keyframe.h \
           layer.h \
           librarydata.h \
           palette.h \
           point3d.h \
           scene.h \
           soundclip.h \
           soundlayer.h \
           symbol.h \
           ktcolor.h \
           status.h \
           ktbrush.h \
           ktkeyframe.h \
           agraphiccomponent.h 
SOURCES += animation.cpp \
           camera.cpp \
           folder.cpp \
           gldrawing.cpp \
           item.cpp \
           keyframe.cpp \
           layer.cpp \
           librarydata.cpp \
           palette.cpp \
           point3d.cpp \
           scene.cpp \
           soundclip.cpp \
           soundlayer.cpp \
           symbol.cpp \
           document.cpp \
           ktcolor.cpp \
           status.cpp \
           ktbrush.cpp \
           ktkeyframe.cpp \
           agraphiccomponent.cpp 
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
