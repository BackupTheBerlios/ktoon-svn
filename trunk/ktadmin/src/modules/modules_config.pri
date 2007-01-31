
!include(../../../ktadmin_config.pri) {
	error("configure first")
}

LIB_DIR = ../../lib
!include($$LIB_DIR/lib.pri) {
	error("err");
}

