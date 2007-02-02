

LIB_DIR = ../../lib
!include($$LIB_DIR/lib.pri) {
	error("err");
}

PACKAGES_DIR = ../../packages
include($$PACKAGES_DIR/packages.pri)

!include(../../ktadmin_config.pri) {
	error("configure first")
}

