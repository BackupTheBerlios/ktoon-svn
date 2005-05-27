# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/opengl/tools
# Destiono es una biblioteca:  

HEADERS += tools.h \
           brushproperties.h \
           ellipseproperties.h \
           imageproperties.h \
           lineproperties.h \
           pencilproperties.h \
           penproperties.h \
           rectangleproperties.h \
           brushes.h \
           glbitmap.h \
           glbrush.h \
           glellipse.h \
           glimage.h \
           glline.h \
           glpencil.h \
           glpen.h \
           glrectangle.h 
SOURCES += tools.cpp \
           brushproperties.cpp \
           ellipseproperties.cpp \
           imageproperties.cpp \
           lineproperties.cpp \
           pencilproperties.cpp \
           penproperties.cpp \
           rectangleproperties.cpp \
           brushes.cpp \
           glbitmap.cpp \
           glbrush.cpp \
           glellipse.cpp \
           glimage.cpp \
           glline.cpp \
           glpencil.cpp \
           glpen.cpp \
           glrectangle.cpp 
TARGETDEPS += ../../../src/lib/liblib.a 
LIBS += ../../../src/lib/liblib.a 
INCLUDEPATH += ../../../src/dialogs \
               ../../../src/core \
               ../../../src/qt/cameras \
               ../../../src/qt/tools \
               ../../../src/qt/draw \
               ../../../src/opengl/draw \
               ../../../src/opengl/tools \
               ../../../src/opengl/cameras \
               ../../../src/store \
               ../../../src/library \
               ../../../src/exposure \
               ../../../src/timeline \
               ../../../src/lib \
               ../../../src/images/images 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
