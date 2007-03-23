# Generated automatically at Thu Mar 22 19:34:15 -0500 2007! PLEASE DO NOT EDIT!
INCLUDEPATH += /usr/include/gstreamer-0.10 /usr/include/glib-2.0 /usr/lib64/glib-2.0/include /usr/include/libxml2
LIBS += -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -ldl -lgthread-2.0 -lxml2 -lz -lm -lglib-2.0 -pthread -laspell -lavcodec -lavformat
QT += opengl core gui svg xml network
DEFINES += HAVE_SOUND HAVE_GST10 HAVE_ASPELL HAVE_FFMPEG D_NODEBUG
unix {
	OBJECTS_DIR = .obj
	UI_DIR = .ui
	MOC_DIR = .moc
}
CONFIG += silent
