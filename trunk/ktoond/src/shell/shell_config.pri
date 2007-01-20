
!include(../../ktoond_config.pri){
    error("Please run \"./configure --help\" first")
}
QT += network xml


LIB_DIR = ../lib/
include($$LIB_DIR/lib.pri)


