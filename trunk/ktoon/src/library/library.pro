# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/library
# Destiono es una biblioteca:  

INCLUDEPATH += ../../src/core \
               ../../src/dialogs \
               ../../src/qt/draw \
               ../../src/qt/tools \
               ../../src/qt/cameras \
               ../../src/opengl/tools \
               ../../src/opengl/cameras \
               ../../src/opengl/draw \
               ../../src/store \
               ../../src/library \
               ../../src/exposure \
               ../../src/timeline \
               ../../src/lib \
               ../../src/images/sequences \
               ../../src/images/images \
               ../../src/images/cursors \
               ../../src/images/icons 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += library.h \
           symbolitem.h \
           symboltable.h \
           symbolview.h 
SOURCES += library.cpp \
           symbolitem.cpp \
           symboltable.cpp \
           symbolview.cpp 
