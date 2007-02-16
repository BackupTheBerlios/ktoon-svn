
INCLUDEPATH += $$MODULES_DIR

LIBS += -L$$MODULES_DIR/project -lproject


linux-g++ {
TARGETDEPS += $$MODULES_DIR/project/libproject.a
}

