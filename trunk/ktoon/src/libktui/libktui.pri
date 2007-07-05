
INCLUDEPATH += $$LIBUI_DIR

LIBS += -L$$LIBUI_DIR -lktui

linux-g++ {
PRE_TARGETDEPS += $$LIBUI_DIR/libktui.a
}

