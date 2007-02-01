
INCLUDEPATH += $$LIB_DIR
LIBS += -L$$LIB_DIR -lktadmin

linux-g++ {
	TARGETDEPS += $$LIB_DIR/libktadmin.a
}


