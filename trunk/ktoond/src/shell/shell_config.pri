
!include(../../ktoond_config.pri){
    error("Please run \"./configure --help\" first")
}
QT += network xml


LIB_DIR = ../lib/
include($$LIB_DIR/lib.pri)

PACKAGES_DIR = ../packages/
include($$PACKAGES_DIR/packages.pri)

