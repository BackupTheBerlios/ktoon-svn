# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/modules/arthury/plugins
# Destiono es una biblioteca:  

INSTALLS += plugins 
plugins.files += *.so 
plugins.path = /plugins/ 
HEADERS += agenericbrush.h 
SOURCES += agenericbrush.cpp 
INCLUDEPATH += ../../../../src/modules/arthury/interfaces \
               ../../../../src/lib 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
QT += xml
