# Generated automatically at Wed Jul 04 23:01:30 COT 2007! PLEASE DO NOT EDIT!
INCLUDEPATH += /usr/include/gstreamer-0.10 /usr/include/glib-2.0 /usr/lib/glib-2.0/include /usr/include/libxml2
LIBS += -laspell -lavcodec -lavformat -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -ldl -lgthread-2.0 -lxml2 -lz -lm -lglib-2.0 -Wl,--export-dynamic -pthread
QT += opengl core gui svg xml network
DEFINES += HAVE_ASPELL HAVE_FFMPEG HAVE_SOUND HAVE_GST10 KLIB_PREFIX='\\"'/usr/lib/kom'\\"'
unix {
	OBJECTS_DIR = .obj
	UI_DIR = .ui
	MOC_DIR = .moc
}
