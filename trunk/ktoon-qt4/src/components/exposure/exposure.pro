# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/components/exposure
# Destiono es una biblioteca:  

INCLUDEPATH += ../../../src/modules/opengl/draw \
               ../../../src/modules/opengl/tools \
               ../../../src/modules/opengl/cameras \
               ../../../src/store \
               ../../../src/lib 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += esframe.h \
           eslayer.h \
           ktexposuresheet.h \
           kttableexposure.h \
           ktlayerexposure.h 
SOURCES += esframe.cpp \
           eslayer.cpp \
           ktexposuresheet.cpp \
           kttableexposure.cpp \
           ktlayerexposure.cpp 
QT += qt3support xml opengl
