# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/modules/arthury/interfaces
# Target is a library:  

INSTALLS += interfaces 
interfaces.files += *.h 
interfaces.path = /include/arthury/ 
HEADERS += afilterinterface.h \
           atoolinterface.h 
QT += xml
KDEV_QTVER = 4
INCLUDEPATH += ../../../../src/store
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib

# a little hack
macx {
TEMPLATE = subdirs
}
win32 {
TEMPLATE = subdirs
#LIBS += -lQtOpenGL4 \
#-ldgui \
#-lktoon \
#-lstore \
#QMAKE_LIBDIR = ../../../../src/dlib/dcore/release \
#               ../../../../src/dlib/dgui/release \
#               ../../../../src/ktoonlib/release \
#               ../../../../src/store/release
}