
INCLUDEPATH += $$MODULES_DIR
LIBS += -L$$MODULES_DIR/users -lusers -L$$MODULES_DIR/projects -lprojects -L$$MODULES_DIR/server -lserver

linux-g++ {
TARGETDEPS += $$MODULES_DIR/users/libusers.a $$MODULES_DIR/projects/libprojects.a $$MODULES_DIR/server/libserver.a
}

