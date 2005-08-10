# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/exposure
# Destiono es una biblioteca:  

HEADERS += exposuresheet.h \
           esframe.h \
           eslayer.h \
           ilayer.h \
           ktexposuresheet.h \
           kttableexposure.h \
           ktlayerexposure.h 
SOURCES += esframe.cpp \
           eslayer.cpp \
           exposuresheet.cpp \
           ilayer.cpp \
           ktexposuresheet.cpp \
           kttableexposure.cpp \
           ktlayerexposure.cpp 
INCLUDEPATH += ../../src/core \
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
../../src/images/images \
../../src/lib
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
