
INCLUDEPATH += $$SERVERMODULES_DIR

LIBS += -L$$SERVERMODULES_DIR/users -lusers -L$$SERVERMODULES_DIR/backups -lbackups -L$$SERVERMODULES_DIR/bans -lbans

linux-g++ {
TARGETDEPS += $$SERVERMODULES_DIR/users/libusers.a $$SERVERMODULES_DIR/backups/libbackups.a $$SERVERMODULES_DIR/bans/libbans.a
}



