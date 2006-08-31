
QT += xml opengl svg

COLORPALETTE_DIR = ../../src/components/colorpalette
PAINTAREA_DIR = ../../src/components/paintarea
PEN_DIR = ../../src/components/pen
KINAS_DIR = ../../src/components/kinas 
HELP_DIR = ../../src/components/help 
IMPORT_DIR = ../../src/components/import 
EXPORT_DIR = ../../src/components/export 
EXPOSURE_DIR = ../../src/components/exposure 
TIMELINE_DIR = ../../src/components/timeline 
LIBRARY_DIR = ../../src/components/library
SCENES_DIR = ../../src/components/scenes 
LIBUI_DIR = ../../src/libktui
STORE_DIR = ../../src/store 
LIBKTOON_DIR = ../../src/libktoon 
QUAZIP_DIR = ../../3rdparty/quazip 
DLIB_DIR = ../../src/dlib 
LIBBASE_DIR = ../../src/libbase

include($$COLORPALETTE_DIR/colorpalette.pri)
include($$PAINTAREA_DIR/paintarea.pri)
include($$PEN_DIR/pen.pri)
include($$KINAS_DIR/kinas.pri)
include($$HELP_DIR/help.pri)
include($$IMPORT_DIR/import.pri)
include($$EXPORT_DIR/export.pri)
include($$EXPOSURE_DIR/exposure.pri)
include($$TIMELINE_DIR/timeline.pri)
include($$LIBRARY_DIR/library.pri)
include($$SCENES_DIR/scenes.pri)
include($$LIBUI_DIR/libktui.pri)
include($$STORE_DIR/store.pri)
include($$LIBKTOON_DIR/libktoon.pri)
include($$QUAZIP_DIR/quazip.pri)
include($$DLIB_DIR/dlib.pri)
include($$LIBBASE_DIR/libbase.pri)