
INCLUDEPATH += $$SERVERMODULES_DIR

LIBS += -L$$SERVERMODULES_DIR/users -lusers -L$$SERVERMODULES_DIR/backups -lbackups -L$$SERVERMODULES_DIR/bans -lbans -L$$SERVERMODULES_DIR/registers -lregisters -L$$SERVERMODULES_DIR/comunications -lcomunications

linux-g++ {
TARGETDEPS += $$SERVERMODULES_DIR/users/libusers.a $$SERVERMODULES_DIR/backups/libbackups.a $$SERVERMODULES_DIR/bans/libbans.a $$SERVERMODULES_DIR/registers/libregisters.a
}



