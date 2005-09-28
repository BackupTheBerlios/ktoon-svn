# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/components/colorpalette
# Destiono es una biblioteca:  

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
HEADERS += colorcells.h \
           colordisplay.h \
           colorgradientselector.h \
           colormixer.h \
           colorpalette.h \
           custompalette.h \
           fillcolor.h \
           gradientswitch.h \
           gradientviewer.h \
           outlinecolor.h \
           valueselector.h 
SOURCES += colorcells.cpp \
           colordisplay.cpp \
           colorgradientselector.cpp \
           colormixer.cpp \
           colorpalette.cpp \
           custompalette.cpp \
           fillcolor.cpp \
           gradientswitch.cpp \
           gradientviewer.cpp \
           outlinecolor.cpp \
           valueselector.cpp 

QT += xml opengl qt3support
