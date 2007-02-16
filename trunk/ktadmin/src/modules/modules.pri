
INCLUDEPATH += $$MODULES_DIR
LIBS += -L$$MODULES_DIR/users -lusers -L$$MODULES_DIR/projects -lprojects -L$$MODULES_DIR/server -lserver -L$$MODULES_DIR/bans -lbans -L$$MODULES_DIR/backups -lbackups -L$$MODULES_DIR/registers -lregisters

linux-g++ {
TARGETDEPS += $$MODULES_DIR/users/libusers.a $$MODULES_DIR/projects/libprojects.a $$MODULES_DIR/server/libserver.a $$MODULES_DIR/bans/libbans.a $$MODULES_DIR/backups/libbackups.a $$MODULES_DIR/registers/libregisters.a
}

