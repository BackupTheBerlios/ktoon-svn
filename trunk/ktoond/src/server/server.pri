
INCLUDEPATH += $$SERVER_DIR 

LIBS += -L$$SERVER_DIR/core -lserver -L$$SERVER_DIR/packages -lserverpackages -L$$SERVER_DIR/base -lbase

linux-g++ {
TARGETDEPS += $$SERVER_DIR/base/libbase.a $$SERVER_DIR/core/libserver.a $$SERVER_DIR/packages/libserverpackages.a
}

SERVERMODULES_DIR = $$SERVER_DIR/modules
include($$SERVERMODULES_DIR/servermodules.pri)


