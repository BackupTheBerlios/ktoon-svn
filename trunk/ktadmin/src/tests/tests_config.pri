
include(../../ktadmin_config.pri)

LIB_DIR = ../lib
!include($$LIB_DIR/lib.pri) {
	error("Can't include lib!")
}

PACKAGES_DIR = ../packages
!include($$PACKAGES_DIR/packages.pri) {
	error("Can't include packages module!")
}





