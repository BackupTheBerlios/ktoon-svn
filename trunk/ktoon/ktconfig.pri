# Generated automatically at Sat May 03 13:51:33 -0500 2008! PLEASE DO NOT EDIT!
INCLUDEPATH += /usr/include/gstreamer-0.10 /usr/include/glib-2.0 /usr/lib/glib-2.0/include /usr/include/libxml2 /usr/include
LIBS += -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -ldl -lgthread-2.0 -lrt -lxml2 -lglib-2.0 -pthread -laspell -lavcodec -lavformat -lkgui -lkcore -lksound -L/usr/lib
QT += opengl core gui svg xml network
DEFINES += HAVE_SOUND HAVE_GST10 HAVE_ASPELL HAVE_FFMPEG VERSION=0.9+svn VERSION_STR=\\\"0.9+svn\\\" D_NODEBUG
unix {
	OBJECTS_DIR = .obj
	UI_DIR = .ui
	MOC_DIR = .moc
}
CONFIG += silent
