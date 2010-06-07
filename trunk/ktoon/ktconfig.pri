# Generated automatically at Sun Jun 06 17:26:11 -0500 2010! PLEASE DO NOT EDIT!
INCLUDEPATH += /usr/include/gstreamer-0.10 /usr/include/glib-2.0 /usr/lib/glib-2.0/include /usr/include/libxml2 /usr/include
LIBS += -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -lxml2 -lgthread-2.0 -lrt -lglib-2.0 -pthread -laspell -lavcodec -lavformat -lkgui -lkcore -lksound -lz -L/usr/lib
QT += opengl core gui svg xml network
DEFINES += HAVE_SOUND HAVE_GST10 HAVE_ASPELL HAVE_FFMPEG VERSION=\\\"0.9a\\\" CODE_NAME=\\\"Phoenix\\\" REVISION=\\\"1480\\\" K_DEBUG
unix {
    OBJECTS_DIR = .obj
    UI_DIR = .ui
    MOC_DIR = .moc
}
