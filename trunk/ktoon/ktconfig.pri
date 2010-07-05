# Generated automatically at Mon Jul 05 09:51:02 -0500 2010! PLEASE DO NOT EDIT!
INCLUDEPATH += /usr/local/ktoon/include
LIBS += -laspell -lavcodec -lavformat -lkgui -lkcore -lksound -lz -L/usr/local/ktoon/lib
QT += opengl core gui svg xml network
DEFINES += HAVE_ASPELL HAVE_FFMPEG VERSION=\\\"0.9a\\\" CODE_NAME=\\\"Phoenix\\\" REVISION=\\\"1499\\\" K_DEBUG
unix {
    OBJECTS_DIR = .obj
    UI_DIR = .ui
    MOC_DIR = .moc
}
