# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/lib
# Target is a library:  ktoon

INSTALLS += include \
            target 
target.path = /lib/ 
include.files += *.h 
include.path = /include/ 
HEADERS += ktapplication.h \
           ktxmlparser.h \
           ktxmlreader.h \
           ktanimwidget.h \
           ktfontwidget.h \
           ktthememanager.h \
           ktthemedocument.h \
           kteditspinbox.h \
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
           ktactionmanager.h \
           ktaction.h \
           ktmodulewidgettitle.h \
           kttabdialog.h \
           ktwizard.h \
           kttabwidget.h \
           ccbar.h \
           ccbutton.h \
           ktwidgetlistview.h \
           ktxyspinbox.h \
           ktcellview.h \
           kttoolbox.h \
           ktdisplaypath.h \
           ktradiobuttongroup.h \
           ktoptionaldialog.h \
           kttreelistwidget.h \
           ktgradientadjuster.h \
           ktpathadjuster.h \
           collapsiblewidget.h \
           ktconfigurationdialog.h \
           ktflatbutton.h \
           ktcolorbutton.h \
           kttoolpluginobject.h \
           ktexportpluginobject.h \
           agraphic.h \
           ktalgorithm.h \
           kttip.h \
           ktguiitem.h \
           ktfontchooser.h \
           ktformfactory.h \
           ktglobal.h \
           aspelliface.h \
           ispelliface.h \
           speller.h \
           spellhighlighter.h \
           spellinterface.h \
           ktgradientmanager.h \
           ktgradientselector.h \
           ktgradientviewer.h 
SOURCES += ktapplication.cpp \
           configwizard.cpp \
           ktxmlparser.cpp \
           ktxmlreader.cpp \
           ktanimwidget.cpp \
           ktfontwidget.cpp \
           ktthememanager.cpp \
           ktthemedocument.cpp \
           kteditspinbox.cpp \
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
           ktactionmanager.cpp \
           ktaction.cpp \
           ktmodulewidgettitle.cpp \
           kttabdialog.cpp \
           ktwizard.cpp \
           kttabwidget.cpp \
           ccbar.cpp \
           ccbutton.cpp \
           ktwidgetlistview.cpp \
           ktxyspinbox.cpp \
           ktcellview.cpp \
           kttoolbox.cpp \
           ktdisplaypath.cpp \
           ktradiobuttongroup.cpp \
           ktoptionaldialog.cpp \
           kttreelistwidget.cpp \
           ktgradientadjuster.cpp \
           ktpathadjuster.cpp \
           collapsiblewidget.cpp \
           ktconfigurationdialog.cpp \
           ktflatbutton.cpp \
           ktcolorbutton.cpp \
           kttoolpluginobject.cpp \
           ktexportpluginobject.cpp \
           agraphic.cpp \
           ktalgorithm.cpp \
           kttip.cpp \
           ktguiitem.cpp \
           ktfontchooser.cpp \
           ktformfactory.cpp \
           aspelliface.cpp \
           ispelliface.cpp \
           speller.cpp \
           spellhighlighter.cpp \
           ktgradientmanager.cpp \
           ktgradientselector.cpp \
           ktgradientviewer.cpp 
QT += xml
KDEV_QTVER = 4
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
TARGET = ktoon
CONFIG += release \
warn_on \
dll
TEMPLATE = lib
!include(../../ktconfig.pri) {
error("Run configure script first")
}
