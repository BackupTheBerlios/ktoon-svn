# Generated automatically at Sun Mar 14 20:41:11 -0500 2010! PLEASE DO NOT EDIT!
INCLUDEPATH += /usr/include/gstreamer-0.10 /usr/include/glib-2.0 /usr/lib/glib-2.0/include /usr/include/libxml2
LIBS += -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lxml2 -lglib-2.0 -pthread -laspell -lavcodec -lavformat
QT += opengl core gui svg xml network
DEFINES += HAVE_SOUND HAVE_GST10 HAVE_ASPELL HAVE_FFMPEG K_DEBUG KLIB_PREFIX='\\"'/usr/local/ktoon'\\"'
unix {
    OBJECTS_DIR = .obj
    UI_DIR = .ui
    MOC_DIR = .moc
}
