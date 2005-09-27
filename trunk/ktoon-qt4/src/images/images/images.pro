# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/images/images
# Destiono es una biblioteca:  

INSTALLS += klimages 
klimages.files += *.png \
                  *.jpg \
                  *.xpm 
klimages.path = /images 
DISTFILES += splash.png 
IMAGES += arrow_rotate.xpm \
          arrow_selection.xpm \
          background.xpm \
          ogllogo.png \
          spin.xpm \
          laboratoon.jpg \
          toonka.jpg \
          credits-image.png \
          splash.png 
HEADERS += images.h 
SOURCES += images.cpp 
INCLUDEPATH += ../../../src/images/sequences \
../../../src/images/cursors \
../../../src/images/icons
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
QT += qt3support
