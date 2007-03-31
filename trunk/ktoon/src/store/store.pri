
INCLUDEPATH += $$STORE_DIR

LIBS += -L$$STORE_DIR -lstore

linux-g++ {
	TARGETDEPS += $$STORE_DIR/libstore.so
}

