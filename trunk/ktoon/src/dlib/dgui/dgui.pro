# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/dlib/dgui
# Target is a library:  

INSTALLS += include \
            target 
target.path = /lib/ 
include.files += *.h 
include.path = /include/dgui 
HEADERS += danimwidget.h \
           dapplication.h \
           dcellview.h \
           dcolorbutton.h \
           dconfigurationdialog.h \
           ddisplaypath.h \
           deditspinbox.h \
           delabel.h \
           dflatbutton.h \
           dfontchooser.h \
           dformfactory.h \
           doptionaldialog.h \
           dradiobuttongroup.h \
           drulerbase.h \
           dsqueezelabel.h \
           dtabdialog.h \
           dtabwidget.h \
           dtoolbox.h \
           dtreelistwidget.h \
           dvhbox.h \
           dwidgetlistview.h \
           dwizard.h \
           dxyspinbox.h \
           daction.h \
           dactionmanager.h \
           dmainwindow.h \
           dmdiwindow.h \
           ddatepicker.h \
           ddatetable.h \
           dthemedocument.h \
           dthememanager.h \
           dclicklineedit.h \
           dtreewidgetsearchline.h \
           ddatewidget.h \
           dosd.h \
           dtipdialog.h \
           dwaitstyle.h \
           dpagedialog.h \
           dpagemodel.h \
           dpageview.h \
           dpageview_p.h \
           dpagewidget.h \
           dpagewidgetmodel.h \
           dcirclebutton.h \
           dcirclebuttonbar.h \
           dcollapsiblewidget.h \
           dseparator.h \
           dimagebutton.h \
           dstylecombobox.h \
           dterm.h \
           dtermtab.h \
           dflashwidget.h \
           ddualcolorbutton.h \
           dbuttonbar.h \
           dtabbedmainwindow.h \
           dtoolview.h \
           dviewbutton.h \
           dcommandhistory.h 
SOURCES += danimwidget.cpp \
           dapplication.cpp \
           dcellview.cpp \
           dcolorbutton.cpp \
           dconfigurationdialog.cpp \
           ddisplaypath.cpp \
           deditspinbox.cpp \
           delabel.cpp \
           dflatbutton.cpp \
           dfontchooser.cpp \
           dformfactory.cpp \
           dimagebutton.cpp \
           doptionaldialog.cpp \
           dradiobuttongroup.cpp \
           drulerbase.cpp \
           dsqueezelabel.cpp \
           dtabdialog.cpp \
           dtabwidget.cpp \
           dtoolbox.cpp \
           dtreelistwidget.cpp \
           dvhbox.cpp \
           dwidgetlistview.cpp \
           dwizard.cpp \
           dxyspinbox.cpp \
           daction.cpp \
           dactionmanager.cpp \
           dmainwindow.cpp \
           dmdiwindow.cpp \
           ddatepicker.cpp \
           ddatetable.cpp \
           dthemedocument.cpp \
           dthememanager.cpp \
           dclicklineedit.cpp \
           dtreewidgetsearchline.cpp \
           ddatewidget.cpp \
           dosd.cpp \
           dtipdialog.cpp \
           dwaitstyle.cpp \
           dpagedialog.cpp \
           dpagemodel.cpp \
           dpageview.cpp \
           dpageview_p.cpp \
           dpagewidget.cpp \
           dpagewidgetmodel.cpp \
           dcirclebutton.cpp \
           dcirclebuttonbar.cpp \
           dcollapsiblewidget.cpp \
           dseparator.cpp \
           dstylecombobox.cpp \
           dterm.cpp \
           dtermtab.cpp \
           dflashwidget.cpp \
           ddualcolorbutton.cpp \
           dbuttonbar.cpp \
           dtabbedmainwindow.cpp \
           dtoolview.cpp \
           dviewbutton.cpp \
           dcommandhistory.cpp 
DEFINES += DLIB_GUI
RESOURCES += dgui_images.qrc
QT += xml
KDEV_QTVER = 4
LIBS += -ldcore
INCLUDEPATH += ../dcore \
../
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
QMAKE_LIBDIR = ../../../src/dlib/dcore \
../dcore
CONFIG += release \
warn_on \
dll
TEMPLATE = lib
QT += opengl
! include(../dlibconfig.pri){
  error("Run ./configure first!")
}
linux-g++{
  TARGETDEPS += ../dcore/libdcore.so
}
