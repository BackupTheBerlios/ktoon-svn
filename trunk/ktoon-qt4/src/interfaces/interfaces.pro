# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/interfaces
# Target is a library:  

INSTALLS += ifaceheaders 
ifaceheaders.files += *.h 
ifaceheaders.path = /include/interfaces/ 
HEADERS += exportinterface.h 
KDEV_QTVER = 4 
INCLUDEPATH += ../../src/store \
               ../../src/lib 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
