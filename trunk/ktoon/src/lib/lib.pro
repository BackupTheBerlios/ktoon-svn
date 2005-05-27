# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/lib
# Destiono es una biblioteca:  

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
               ../../src/lib \
               ../../src/images/images 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
DEFINES = KTHOME=\"$(KTOON_HOME)\" 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += glgraphiccomponent.h \
           ktapplication.h \
           configwizard.h \
           ktxmlparser.h \
           ktxmlreader.h 
SOURCES += glgraphiccomponent.cpp \
           ktapplication.cpp \
           configwizard.cpp \
           ktxmlparser.cpp \
           ktxmlreader.cpp 
