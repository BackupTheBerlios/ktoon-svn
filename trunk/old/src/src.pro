# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src
# Destiono es una aplicaci??n: ../bin/ktoon

TRANSLATIONS += trans/ktoon_es.ts \
                trans/ktoon_fr.ts \
                trans/ktoon_tr.ts 
HEADERS += store/animation.h \
           store/brush.h \
           store/camera.h \
           store/color.h \
           store/document.h \
           store/gldrawing.h \
           store/keyframe.h \
           store/layer.h \
           store/palette.h \
           store/point3d.h \
           store/scene.h \
           store/soundclip.h \
           store/soundlayer.h \
           store/status.h \
           store/librarydata.h \
           store/item.h \
           store/folder.h \
           store/symbol.h \
           brushes/brushes.h \
           color/colorcells.h \
           color/colordisplay.h \
           color/colorgradientselector.h \
           color/colormixer.h \
           color/colorpalette.h \
           color/custompalette.h \
           color/valueselector.h \
           color/gradientswitch.h \
           color/gradientviewer.h \
           color/fillcolor.h \
           color/outlinecolor.h \
           exposure/esframe.h \
           exposure/eslayer.h \
           exposure/exposuresheet.h \
           exposure/ilayer.h \
           gl/glbitmap.h \
           gl/drawingarea.h \
           gl/glbrush.h \
           gl/glcontrol.h \
           gl/glellipse.h \
           gl/glgraphiccomponent.h \
           gl/glgrid.h \
           gl/glline.h \
           gl/glpencil.h \
           gl/glpen.h \
           gl/glrectangle.h \
           gl/glimage.h \
           cameras/glrendercamerapreview.h \
           cameras/glsidecameraview.h \
           cameras/gltopcameraview.h \
           modal/preferences.h \
           modal/properties.h \
           modal/about.h \
           modal/splash.h \
           modal/import.h \
           modal/export.h \
           scenes/scenes.h \
           timeline/timeline.h \
           timeline/tlframe.h \
           timeline/tlframesequence.h \
           timeline/tlframesequencelayout.h \
           timeline/tlframesequencemanager.h \
           timeline/tllayer.h \
           timeline/tllayermanager.h \
           timeline/tllayersequence.h \
           timeline/tlruler.h \
           timeline/soundwidget.h \
           tools/tools.h \
           ktoon.h \
           images/images.h \
           properties/ellipseproperties.h \
           properties/brushproperties.h \
           properties/rectangleproperties.h \
           properties/lineproperties.h \
           properties/pencilproperties.h \
           properties/penproperties.h \
           properties/imageproperties.h \
           library/library.h \
           library/symbolview.h \
           library/symboltable.h \
           library/symbolitem.h \
           swf/mingpp.h \
           swf/ming.h \
           drawqt/drawingareaqt.h \
           drawqt/qtgrid.h \
           drawqt/qtline.h \
           drawqt/qtellipse.h \
           drawqt/qtrectangle.h \
           drawqt/qtbrush.h \
           drawqt/qtpencil.h \
           drawqt/qtfill.h \
           drawqt/qtpen.h \
           drawqt/rendercamerapreviewqt.h 
SOURCES += store/animation.cpp \
           store/brush.cpp \
           store/camera.cpp \
           store/color.cpp \
           store/document.cpp \
           store/gldrawing.cpp \
           store/keyframe.cpp \
           store/layer.cpp \
           store/palette.cpp \
           store/point3d.cpp \
           store/scene.cpp \
           store/soundclip.cpp \
           store/soundlayer.cpp \
           store/status.cpp \
           store/librarydata.cpp \
           store/item.cpp \
           store/folder.cpp \
           store/symbol.cpp \
           brushes/brushes.cpp \
           color/colorcells.cpp \
           color/colordisplay.cpp \
           color/colorgradientselector.cpp \
           color/colormixer.cpp \
           color/colorpalette.cpp \
           color/custompalette.cpp \
           color/valueselector.cpp \
           color/gradientswitch.cpp \
           color/gradientviewer.cpp \
           color/fillcolor.cpp \
           color/outlinecolor.cpp \
           exposure/esframe.cpp \
           exposure/eslayer.cpp \
           exposure/exposuresheet.cpp \
           exposure/ilayer.cpp \
           gl/glbitmap.cpp \
           gl/drawingarea.cpp \
           gl/glbrush.cpp \
           gl/glcontrol.cpp \
           gl/glellipse.cpp \
           gl/glgraphiccomponent.cpp \
           gl/glgrid.cpp \
           gl/glline.cpp \
           gl/glpencil.cpp \
           gl/glpen.cpp \
           gl/glrectangle.cpp \
           gl/glimage.cpp \
           cameras/glrendercamerapreview.cpp \
           cameras/glsidecameraview.cpp \
           cameras/gltopcameraview.cpp \
           modal/preferences.cpp \
           modal/properties.cpp \
           modal/about.cpp \
           modal/splash.cpp \
           modal/import.cpp \
           modal/export.cpp \
           scenes/scenes.cpp \
           timeline/timeline.cpp \
           timeline/tlframe.cpp \
           timeline/tlframesequence.cpp \
           timeline/tlframesequencelayout.cpp \
           timeline/tlframesequencemanager.cpp \
           timeline/tllayer.cpp \
           timeline/tllayermanager.cpp \
           timeline/tllayersequence.cpp \
           timeline/tlruler.cpp \
           timeline/soundwidget.cpp \
           tools/tools.cpp \
           main.cpp \
           ktoon.cpp \
           images/images.cpp \
           library/library.cpp \
           library/symbolview.cpp \
           library/symboltable.cpp \
           library/symbolitem.cpp \
           properties/ellipseproperties.cpp \
           properties/brushproperties.cpp \
           properties/rectangleproperties.cpp \
           properties/lineproperties.cpp \
           properties/pencilproperties.cpp \
           properties/penproperties.cpp \
           properties/imageproperties.cpp \
           drawqt/drawingareaqt.cpp \
           drawqt/qtgrid.cpp \
           drawqt/qtline.cpp \
           drawqt/qtellipse.cpp \
           drawqt/qtrectangle.cpp \
           drawqt/qtbrush.cpp \
           drawqt/qtpencil.cpp \
           drawqt/qtfill.cpp \
           drawqt/qtpen.cpp \
           drawqt/rendercamerapreviewqt.cpp 
TEMPLATE = app
CONFIG += release \
warn_on \
qt \
opengl \
x11
TARGET = ../bin/ktoon
QMAKE_LIBDIR = ../lib/
OBJECTS_DIR = ./obj
MOC_DIR = ./moc
INCLUDEPATH = ../src
LIBS += ../lib/libming.a