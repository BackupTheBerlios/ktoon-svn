# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/timeline
# Destiono es una biblioteca:  

HEADERS += tlframe.h \
           kttimeline.h \
           ktlayermanager.h \
           ktlayersequence.h \
           kttimelinelayer.h \
           kttlruler.h \
           ktframesequencemanager.h \
           ktframesequence.h 
SOURCES += tlframe.cpp \
           kttimeline.cpp \
           ktlayermanager.cpp \
           ktlayersequence.cpp \
           kttimelinelayer.cpp \
           kttlruler.cpp \
           ktframesequencemanager.cpp \
           ktframesequence.cpp 
INCLUDEPATH += ../../src/core \
../../src/exposure \
../../src/dialogs \
../../src/qt/cameras \
../../src/qt/tools \
../../src/qt/draw \
../../src/opengl/draw \
../../src/opengl/tools \
../../src/opengl/cameras \
../../src/store \
../../src/library \
../../src/timeline \
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
qt \
staticlib
TEMPLATE = lib
#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
