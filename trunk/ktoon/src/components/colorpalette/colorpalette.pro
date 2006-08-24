# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/colorpalette
# Target is a library:  

INSTALLS += palettes 

palettes.files += palettes/*.ktpl 
palettes.path = /data/palettes/ 
HEADERS += ktcolorpicker.h \
           ktcolorpalette.h \
           ktvaluecolor.h \
           ktviewcolorcells.h \
           ktluminancepicker.h \
           ktcellscolor.h \
           ktpaletteparser.h 
SOURCES += ktcolorpicker.cpp \
           ktcolorpalette.cpp \
           ktvaluecolor.cpp \
           ktviewcolorcells.cpp \
           ktluminancepicker.cpp \
           ktcellscolor.cpp \
           ktpaletteparser.cpp 
KDEV_QTVER = 4



CONFIG += static
TEMPLATE = lib

TARGET = colorpalette

include(../components_config.pri)

