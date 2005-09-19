# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/images/images
# Destiono es una biblioteca:  

INSTALLS += klimages 
klimages.files += *.png *.jpg *.xpm 
klimages.path = $(KTOON_HOME)/images 
IMAGES += arrow_rotate.xpm \
          arrow_selection.xpm \
          background.xpm \
          ogllogo.png \
          spin.xpm \
          splash.xpm \
          laboratoon.jpg \
          toonka.jpg \
          credits-image.png 
HEADERS += images.h 
SOURCES += images.cpp 
INCLUDEPATH += ../../../src/images/sequences \
../../../src/images/cursors \
../../../src/images/icons
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
#The following line was inserted by qt3to4
QT += qt3support 
