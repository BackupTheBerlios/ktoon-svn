
INCLUDEPATH += $$CAMERAS_DIR

LIBS += -L$$CAMERAS_DIR -lcameras

linux-g++ {
PRE_TARGETDEPS += $$CAMERAS_DIR/libcameras.a
}

