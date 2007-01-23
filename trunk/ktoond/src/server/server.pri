
INCLUDEPATH += $$SERVER_DIR/core $$SERVER_DIR/users $$SERVER_DIR/packages

LIBS += -L$$SERVER_DIR/core -lserver -L$$SERVER_DIR/users -lusers -L$$SERVER_DIR/packages -lserverpackages


TARGETDEPS += $$SERVER_DIR/core/libserver.a $$SERVER_DIR/users/libusers.a $$SERVER_DIR/packages/libserverpackages.a

