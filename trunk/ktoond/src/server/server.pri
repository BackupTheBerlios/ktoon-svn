
INCLUDEPATH += $$SERVER_DIR/core $$SERVER_DIR/users $$SERVER_DIR/packages

LIBS += -L$$SERVER_DIR/core -lserver -L$$SERVER_DIR/users -lusers -L$$SERVER_DIR/packages -lpackages


