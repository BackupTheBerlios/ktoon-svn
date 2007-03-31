
INCLUDEPATH += $$LIBKTOON_DIR

LIBS += -L$$LIBKTOON_DIR -lktoon

linux-g++ {
	 TARGETDEPS += $$LIBKTOON_DIR/libktoon.so
}
