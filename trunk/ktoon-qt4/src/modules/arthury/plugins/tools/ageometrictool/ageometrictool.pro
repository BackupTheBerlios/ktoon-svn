# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/modules/arthury/plugins/tools/ageometrictool
# Destiono es una biblioteca:  

INSTALLS += ageometrictoolplugin 
ageometrictoolplugin.files += *.so 
ageometrictoolplugin.path = /plugins/ 
INCLUDEPATH += ../../../../../../src/modules/arthury/interfaces 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
HEADERS += ageometrictoolplugin.h 
SOURCES += ageometrictoolplugin.cpp 
QT += xml