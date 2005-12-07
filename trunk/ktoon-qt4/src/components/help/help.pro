# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/components/help
# Destiono es una biblioteca:  

INSTALLS += help 
help.files += help 
help.path = /data/ 
HEADERS += kthelpwidget.h \
           kthelpbrowser.h 
SOURCES += kthelpwidget.cpp \
           kthelpbrowser.cpp 
INCLUDEPATH += ../../../src/dlslib \
               ../../../src/lib 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 

QT += qt3support xml
