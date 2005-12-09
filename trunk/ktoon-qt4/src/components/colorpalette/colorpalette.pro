# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/components/colorpalette
# Destiono es una biblioteca:  

INSTALLS += palettes 
palettes.files += palettes/*.ktpl 
palettes.path = /data/palettes/ 
DISTFILES += dcolorarrow.xpm \
             dcolorreset.xpm 
IMAGES += dcolorarrow.xpm \
          dcolorreset.xpm 
HEADERS += ktcolorpicker.h \
           ktcolorpalette.h \
           ktvaluecolor.h \
           ktviewcolorcells.h \
           ktluminancepicker.h \
           ktdualcolorbutton.h \
           ktgradientselector.h \
           ktgradientviewer.h \
           ktgradientmanager.h \
           ktcellscolor.h \
           ktpaletteparser.h 
SOURCES += ktcolorpicker.cpp \
           ktcolorpalette.cpp \
           ktvaluecolor.cpp \
           ktviewcolorcells.cpp \
           ktluminancepicker.cpp \
           ktdualcolorbutton.cpp \
           ktgradientselector.cpp \
           ktgradientviewer.cpp \
           ktgradientmanager.cpp \
           ktcellscolor.cpp \
           ktpaletteparser.cpp 
QT += xml qt3support
INCLUDEPATH += ../../../src/store \
../../../src/lib
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
