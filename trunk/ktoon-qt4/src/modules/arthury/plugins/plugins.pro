# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/modules/arthury/plugins
# Destiono es una biblioteca:  

QT += xml 
INSTALLS += plugins 
plugins.files += *.so 
plugins.path = /plugins/ 
INCLUDEPATH += ../../../../src/modules/arthury/interfaces \
               ../../../../src/lib 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
HEADERS += agenericbrush.h 
SOURCES += agenericbrush.cpp 
