# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/components/timeline
# Destiono es una biblioteca:  

HEADERS += tlframe.h \
           kttimeline.h \
           ktlayermanager.h \
           ktlayersequence.h \
           kttimelinelayer.h \
           kttlruler.h \
           ktframesequencemanager.h \
           ktframesequence.h \
           ktframesequencecontainer.h 
SOURCES += tlframe.cpp \
           kttimeline.cpp \
           ktlayermanager.cpp \
           ktlayersequence.cpp \
           kttimelinelayer.cpp \
           kttlruler.cpp \
           ktframesequencemanager.cpp \
           ktframesequence.cpp \
           ktframesequencecontainer.cpp 
INCLUDEPATH += ../../../src/lib \
../../../src/images/images
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
qt \
staticlib
TEMPLATE = lib
QT += qt3support xml opengl
