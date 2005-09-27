# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/lib
# Destiono es una biblioteca:  

HEADERS += glgraphiccomponent.h \
           ktapplication.h \
           configwizard.h \
           ktxmlparser.h \
           ktxmlreader.h \
           ktanimwidget.h \
           ktfontwidget.h \
           ktdialogtitle.h \
           ktdialogmenu.h \
           status.h \
           ktthememanager.h \
           ktthemedocument.h \
           kteditspinbox.h \
           ktexporter.h \
           ktmdiwindow.h \
           ktconfigdocument.h \
           ktconfig.h \
           ktimagebutton.h \
           ktdebug.h \
           kseparator.h \
           ktrulerbase.h \
           ktsqueezelabel.h \
           kcpuinfo.h \
           kimageeffect.h \
           ktfiledialog.h \
           ktvhbox.h \
           ktelabel.h \
           ktmodulewidgetbase.h \
           ktdisplaybrush.h 
SOURCES += glgraphiccomponent.cpp \
           ktapplication.cpp \
           configwizard.cpp \
           ktxmlparser.cpp \
           ktxmlreader.cpp \
           ktanimwidget.cpp \
           ktfontwidget.cpp \
           ktdialogtitle.cpp \
           ktdialogmenu.cpp \
           status.cpp \
           ktthememanager.cpp \
           ktthemedocument.cpp \
           kteditspinbox.cpp \
           ktexporter.cpp \
           ktmdiwindow.cpp \
           ktconfigdocument.cpp \
           ktconfig.cpp \
           ktimagebutton.cpp \
           ktdebug.cpp \
           kseparator.cpp \
           ktrulerbase.cpp \
           ktsqueezelabel.cpp \
           kcpuinfo.cpp \
           kimageeffect.cpp \
           ktfiledialog.cpp \
           ktvhbox.cpp \
           ktelabel.cpp \
           ktmodulewidgetbase.cpp \
           ktdisplaybrush.cpp 
INCLUDEPATH += ../../src/core \
../../src/exposure \
../../src/dialogs \
../../src/qt/cameras \
../../src/qt/tools \
../../src/qt/draw \
../../src/opengl/draw \
../../src/opengl/tools \
../../src/opengl/cameras \
../../src/store \
../../src/library \
../../src/timeline \
../../src/lib \
../../src/images/images
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
QT += xml opengl qt3support