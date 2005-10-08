# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/modules/opengl/tools
# Destiono es una biblioteca:  

HEADERS += brushproperties.h \
           ellipseproperties.h \
           imageproperties.h \
           lineproperties.h \
           pencilproperties.h \
           penproperties.h \
           rectangleproperties.h \
           glbitmap.h \
           glbrush.h \
           glellipse.h \
           glimage.h \
           glline.h \
           glpencil.h \
           glpen.h \
           glrectangle.h \
           brush.h \
           gltext.h \
           glgraphiccomponent.h 
SOURCES += brushproperties.cpp \
           ellipseproperties.cpp \
           imageproperties.cpp \
           lineproperties.cpp \
           pencilproperties.cpp \
           penproperties.cpp \
           rectangleproperties.cpp \
           glbitmap.cpp \
           glbrush.cpp \
           glellipse.cpp \
           glimage.cpp \
           glline.cpp \
           glpencil.cpp \
           glpen.cpp \
           glrectangle.cpp \
           brush.cpp \
           gltext.cpp \
           glgraphiccomponent.cpp 
INCLUDEPATH += ../../../../src/modules/opengl/draw \
               ../../../../src/modules/opengl/cameras \
               ../../../../src/store \
               ../../../../src/lib 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
QT += qt3support xml opengl