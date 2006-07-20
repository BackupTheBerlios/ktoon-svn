# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/store
# Target is a library:  

INSTALLS += headers \
            target 
target.path = /lib/ 
headers.files += *.h 
headers.path = /include/ 
HEADERS += ktlayer.h \
           ktpalettedocument.h \
           ktframe.h \
           ktframeevent.h \
           ktlayerevent.h \
           ktprojectevent.h \
           ktsceneevent.h \
           ktscene.h \
           ktproject.h \
           ktprojectmanager.h \
           ktprojectcommand.h \
           ktabstractprojecteventhandler.h \
           ktabstractserializable.h \
           ktframegroup.h \
           ktphotogram.h \
           ktpathitem.h \
           ktitemevent.h \
           ktrectitem.h \
           ktpixmapitem.h \
           kttextitem.h \
           ktbuttonitem.h 
SOURCES += ktlayer.cpp \
           ktpalettedocument.cpp \
           ktframe.cpp \
           ktframeevent.cpp \
           ktlayerevent.cpp \
           ktprojectevent.cpp \
           ktsceneevent.cpp \
           ktscene.cpp \
           ktproject.cpp \
           ktprojectmanager.cpp \
           ktprojectcommand.cpp \
           ktabstractprojecteventhandler.cpp \
           ktframegroup.cpp \
           ktphotogram.cpp \
           ktpathitem.cpp \
           ktitemevent.cpp \
           ktrectitem.cpp \
           ktpixmapitem.cpp \
           kttextitem.cpp \
           ktbuttonitem.cpp 
QT += xml gui
KDEV_QTVER = 4
INCLUDEPATH += ../../src/store \
../../src/libktoon \
../../src/dlib/dgui \
../../src/dlib/dcore \
../../src/dlib
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
dll
TEMPLATE = lib
macx{
  LIBS += -ldcore -ldgui -lktoon
  QMAKE_LIBDIR = ../../src/dlib/dcore ../../src/dlib/dgui ../../src/ktoonlib
}
