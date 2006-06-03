# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/modules/arthury/plugins
# Destiono es un subdirectorio de proyecto 

SUBDIRS += tools \
           filters 
QT += xml
TARGETDEPS += ../../../../src/lib/liblib.a
LIBS += ../../../../src/lib/liblib.a
INCLUDEPATH += ../../../../src/modules/arthury/interfaces \
../../../../src/lib
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
TARGET = arthuryplugins
CONFIG += release \
warn_on
TEMPLATE = subdirs
