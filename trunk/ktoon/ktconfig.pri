# Generated automatically at Thu Jul 05 18:31:01 COT 2007! PLEASE DO NOT EDIT!
INCLUDEPATH += /usr/include/gstreamer-0.10 /usr/include/glib-2.0 /usr/lib/glib-2.0/include /usr/include/libxml2 /usr/lib/kom/include
LIBS += -laspell -lavcodec -lavformat -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -ldl -lgthread-2.0 -lxml2 -lz -lm -lglib-2.0 -Wl,--export-dynamic -pthread -lkgui -lkcore -lksound -L/usr/lib/kom/lib
QT += opengl core gui svg xml network
DEFINES += HAVE_ASPELL HAVE_FFMPEG HAVE_SOUND HAVE_GST10 VERSION=0.9+svn VERSION_STR=\\\"0.9+svn\\\"
unix {
	OBJECTS_DIR = .obj
	UI_DIR = .ui
	MOC_DIR = .moc
}
