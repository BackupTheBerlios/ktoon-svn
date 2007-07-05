
INCLUDEPATH += $$KLIB_DIR $$KLIB_DIR/kcore $$KLIB_DIR/kgui $$KLIB_DIR/ksound

LIBS += -L$$KLIB_DIR/kcore -L$$KLIB_DIR/kgui -L$$KLIB_DIR/ksound -lkgui -lkcore -lksound
