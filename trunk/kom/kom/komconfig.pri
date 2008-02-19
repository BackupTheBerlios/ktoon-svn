# Generated automatically at Sun Feb 17 06:45:39 -0500 2008! PLEASE DO NOT EDIT!
INCLUDEPATH += /usr/include/gstreamer-0.10 /usr/include/glib-2.0 /usr/lib/glib-2.0/include /usr/include/libxml2
LIBS += -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -ldl -lgthread-2.0 -lrt -lxml2 -lglib-2.0 -pthread -lavcodec -lavformat -laspell
QT += opengl core gui svg xml network
DEFINES += HAVE_SOUND HAVE_GST10 HAVE_FFMPEG HAVE_ASPELL K_NODEBUG KLIB_PREFIX='\\"'/usr'\\"'
unix {
	OBJECTS_DIR = .obj
	UI_DIR = .ui
	MOC_DIR = .moc
}
CONFIG += silent
