
INCLUDEPATH += $$LIBBASE_DIR

LIBS += -L$$LIBBASE_DIR -lktbase

linux-g++ {
	TARGETDEPS += $$LIBBASE_DIR/libktbase.so
}
