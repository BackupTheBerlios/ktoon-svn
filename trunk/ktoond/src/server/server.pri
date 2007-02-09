
INCLUDEPATH += $$SERVER_DIR/core $$SERVER_DIR/packages $$SERVER_DIR/users 

LIBS += -L$$SERVER_DIR/core -lserver -L$$SERVER_DIR/packages -lserverpackages -L$$SERVER_DIR/users -lusers 

linux-g++ {
TARGETDEPS += $$SERVER_DIR/core/libserver.a $$SERVER_DIR/packages/libserverpackages.a $$SERVER_DIR/users/libusers.a 
}

