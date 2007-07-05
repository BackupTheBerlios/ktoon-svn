
INCLUDEPATH += $$NET_DIR

LIBS += -L$$NET_DIR -lktnet

QT += network

linux-g++{
  TARGETDEPS += $$NET_DIR/libktnet.a
}

