# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/store
# Target is a library:  

INSTALLS += headers \
            target 
target.path = /lib/ 
headers.files += *.h 
headers.path = /include/store/ 
HEADERS += ktlayer.h \
           ktpalettedocument.h \
           ktframe.h \
           ktframerequest.h \
           ktlayerrequest.h \
           ktprojectrequest.h \
           ktscenerequest.h \
           ktscene.h \
           ktproject.h \
           ktprojectmanager.h \
           ktprojectcommand.h \
           ktabstractprojectrequesthandler.h \
           ktabstractserializable.h \
           ktframegroup.h \
           ktphotogram.h \
           ktpathitem.h \
           ktitemrequest.h \
           ktrectitem.h \
           ktpixmapitem.h \
           kttextitem.h \
           ktbuttonitem.h \
           ktitemfactory.h \
           ktellipseitem.h \
           ktglobal_store.h \
           ktpaintareaevent.h \
           ktserializer.h \
           ktlineitem.h \
           ktitemconverter.h \
           ktitemgroup.h \
           ktprojectmanagerparams.h \
           ktabstractprojectmanagerhandler.h \
           ktrequestpackage.h \
           ktrequestfactory.h 
SOURCES += ktlayer.cpp \
           ktpalettedocument.cpp \
           ktframe.cpp \
           ktframerequest.cpp \
           ktlayerrequest.cpp \
           ktprojectrequest.cpp \
           ktscenerequest.cpp \
           ktscene.cpp \
           ktproject.cpp \
           ktprojectmanager.cpp \
           ktprojectcommand.cpp \
           ktabstractprojectrequesthandler.cpp \
           ktframegroup.cpp \
           ktphotogram.cpp \
           ktpathitem.cpp \
           ktitemrequest.cpp \
           ktrectitem.cpp \
           ktpixmapitem.cpp \
           kttextitem.cpp \
           ktbuttonitem.cpp \
           ktitemfactory.cpp \
           ktellipseitem.cpp \
           ktproject_frames.cpp \
           ktproject_layers.cpp \
           ktproject_items.cpp \
           ktpaintareaevent.cpp \
           ktserializer.cpp \
           ktlineitem.cpp \
           ktitemconverter.cpp \
           ktitemgroup.cpp \
           ktprojectmanagerparams.cpp \
           ktabstractprojectmanagerhandler.cpp \
           ktrequestpackage.cpp \
           ktrequestfactory.cpp 
KDEV_QTVER = 4
STORE_DIR = ../../src/store
LIBKTOON_DIR = ../../src/libktoon
DLIB_DIR = ../../src/dlib
include($$LIBKTOON_DIR/libktoon.pri)
include($$DLIB_DIR/dlib.pri)
CONFIG += dll
TEMPLATE = lib
include(../../ktconfig.pri)
