
!include(../../ktoond_config.pri){
    error("cannot include config")
}

LIB_DIR = ../../lib
include($$LIB_DIR/lib.pri)

PACKAGES_DIR = ../../packages
!include($$PACKAGES_DIR/packages.pri){
    error("cannot include packages")
}

SERVER_DIR = ../../server/
!include($$SERVER_DIR/server.pri){
    error("cannot include server.pri")
}

