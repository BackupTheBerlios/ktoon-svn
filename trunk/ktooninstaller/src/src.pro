# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src
# Destiono es una aplicaci??n: ../bin/setup.bin

HEADERS += ktooninstaller.h \
           welcomepage.h \
           licencepage.h \
           installpage.h \
           application.h 
SOURCES += ktooninstaller.cpp \
           main.cpp \
           welcomepage.cpp \
           licencepage.cpp \
           installpage.cpp \
           application.cpp 
LIBS += -lktoon 
INCLUDEPATH += $(KTOON_HOME)/include 
QMAKE_LIBDIR = $(KTOON_HOME)/lib 
TARGET = ../bin/setup.bin 
CONFIG += release \
          warn_on \
          qt \
          thread 
TEMPLATE = app 
