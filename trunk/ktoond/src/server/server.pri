
INCLUDEPATH += $$SERVER_DIR/core $$SERVER_DIR/packages $$SERVER_DIR/users 

LIBS += -L$$SERVER_DIR/core -lserver -L$$SERVER_DIR/packages -lserverpackages 

linux-g++ {
TARGETDEPS += $$SERVER_DIR/core/libserver.a $$SERVER_DIR/packages/libserverpackages.a
}

SERVERMODULES_DIR = $$SERVER_DIR/modules
include($$SERVERMODULES_DIR/servermodules.pri)


