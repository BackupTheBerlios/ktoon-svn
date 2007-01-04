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
           ktprojectrequest.h \
           ktscene.h \
           ktproject.h \
           ktprojectmanager.h \
           ktprojectcommand.h \
           ktabstractprojectrequesthandler.h \
           ktabstractserializable.h \
           ktframegroup.h \
           ktphotogram.h \
           ktpathitem.h \
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
           ktcommandexecutor.h \
           ktgraphicobject.h \
           ktlibrary.h \
           ktlibraryobject.h \
           ktlibraryfolder.h \
           ktrequestbuilder.h \
           ktrequestparser.h \
           ktprojectresponse.h \
           ktabstractprojectresponsehandler.h  \
           ktsaveproject.h
SOURCES += ktlayer.cpp \
           ktpalettedocument.cpp \
           ktframe.cpp \
           ktprojectrequest.cpp \
           ktscene.cpp \
           ktproject.cpp \
           ktprojectmanager.cpp \
           ktprojectcommand.cpp \
           ktabstractprojectrequesthandler.cpp \
           ktframegroup.cpp \
           ktphotogram.cpp \
           ktpathitem.cpp \
           ktrectitem.cpp \
           ktpixmapitem.cpp \
           kttextitem.cpp \
           ktbuttonitem.cpp \
           ktellipseitem.cpp \
           ktpaintareaevent.cpp \
           ktserializer.cpp \
           ktlineitem.cpp \
           ktitemconverter.cpp \
           ktitemgroup.cpp \
           ktprojectmanagerparams.cpp \
           ktabstractprojectmanagerhandler.cpp \
           ktcommandexecutor.cpp \
           ktcommandexecutor_frames.cpp \
           ktcommandexecutor_items.cpp \
           ktcommandexecutor_layers.cpp \
           ktgraphicobject.cpp \
           ktlibrary.cpp \
           ktlibraryobject.cpp \
           ktlibraryfolder.cpp \
           ktcommandexecutor_library.cpp \
           ktrequestbuilder.cpp \
           ktrequestparser.cpp \
           ktprojectresponse.cpp \
           ktabstractprojectresponsehandler.cpp \
           ktitemfactory.cpp  \
           ktsaveproject.cpp
KDEV_QTVER = 4
STORE_DIR = ../../src/store
LIBKTOON_DIR = ../../src/libktoon
DLIB_DIR = ../../src/dlib
include($$LIBKTOON_DIR/libktoon.pri)
include($$DLIB_DIR/dlib.pri)
CONFIG += dll
TEMPLATE = lib
include(../../ktconfig.pri)
