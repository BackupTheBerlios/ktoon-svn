# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/modules/arthury/plugins/tools/apolygontool
# Target is a library:  

QT += xml 
INSTALLS += target 
target.path = /plugins/ 
KDEV_QTVER = 4 
TARGETDEPS += ../../../../../../src/store/libstore.so \
              ../../../../../../src/dlib/dgui/libdgui.so \
              ../../../../../../src/dlib/dcore/libdcore.so \
              ../../../../../../src/ktoonlib/libktoonlib.so 
LIBS += -lstore \
        -ldgui \
        -ldcore \
        -lktoonlib 
INCLUDEPATH += ../../../../../../src/modules/arthury/interfaces \
               ../../../../../../src/store \
               ../../../../../../src/ktoonlib \
               ../../../../../../src/dlib/dgui \
               ../../../../../../src/dlib/dcore 
QMAKE_LIBDIR = ../../../../../../src/store \
               ../../../../../../src/dlib/dgui \
               ../../../../../../src/dlib/dcore \
               ../../../../../../src/ktoonlib 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
HEADERS += apolygontool.h 
SOURCES += apolygontool.cpp 
