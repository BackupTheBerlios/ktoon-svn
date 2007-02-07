
LIB_DIR = ../lib
!include($$LIB_DIR/lib.pri) {
	error("Can't include lib!")
}

PACKAGES_DIR = ../packages
!include($$PACKAGES_DIR/packages.pri) {
	error("Can't include packages module!")
}

SERVER_DIR = ../server/
!include($$SERVER_DIR/server.pri) {
	error("Can't include server!")
}

