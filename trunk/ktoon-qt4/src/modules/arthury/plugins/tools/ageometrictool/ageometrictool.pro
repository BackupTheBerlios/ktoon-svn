# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/modules/arthury/plugins/tools/ageometrictool
# Destiono es una biblioteca:  

QT += xml 
INSTALLS += ageometrictoolplugin 
ageometrictoolplugin.files += *.so 
ageometrictoolplugin.path = /plugins/ 
TARGETDEPS += ../../../../../../src/lib/libktoon.so 
LIBS += -lktoon 
INCLUDEPATH += ../../../../../../src/modules/arthury/interfaces \
               ../../../../../../src/lib 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
QMAKE_LIBDIR = ../../../../../../src/lib 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
HEADERS += ageometrictoolplugin.h 
SOURCES += ageometrictoolplugin.cpp 
