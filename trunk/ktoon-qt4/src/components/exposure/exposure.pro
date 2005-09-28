# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/components/exposure
# Destiono es una biblioteca:  

QT += xml opengl qt3support 
INCLUDEPATH += ../../../src/qt/cameras \
               ../../../src/qt/tools \
               ../../../src/qt/draw \
               ../../../src/opengl/draw \
               ../../../src/opengl/tools \
               ../../../src/opengl/cameras \
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
