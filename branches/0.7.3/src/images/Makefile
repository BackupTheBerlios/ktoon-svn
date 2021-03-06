#############################################################################
# Makefile for building: libimages.a
# Generated by qmake (1.07a) (Qt 3.3.3) on: Tue Apr 26 11:11:34 2005
# Project:  images.pro
# Template: lib
# Command: $(QMAKE) -o Makefile images.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -Wall -W -O2  -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT
CXXFLAGS = -pipe -Wall -W -O2  -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/usr/share/qt3/mkspecs/default -I. -I/usr/include/qt3
AR       = ar cqs
RANLIB   = 
MOC      = /usr/share/qt3/bin/moc
UIC      = /usr/share/qt3/bin/uic
QMAKE    = qmake
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY)
COPY_DIR = $(COPY) -r
INSTALL_FILE= $(COPY_FILE)
INSTALL_DIR = $(COPY_DIR)
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = ./

####### Files

HEADERS = images.h
SOURCES = images.cpp
OBJECTS = images.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = 
OBJMOC = 
DIST	   = images.pro
QMAKE_TARGET = images
DESTDIR  = 
TARGET   = libimages.a

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile $(TARGET) 

staticlib: $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC) $(OBJCOMP) 
	-$(DEL_FILE) $(TARGET)
	$(AR) $(TARGET) $(OBJECTS) $(OBJMOC)


mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd $(QTDIR)/src/moc && $(MAKE) )

Makefile: images.pro  /usr/share/qt3/mkspecs/default/qmake.conf /usr/share/qt3/lib/libqt-mt.prl
	$(QMAKE) -o Makefile images.pro
qmake: 
	@$(QMAKE) -o Makefile images.pro

dist: 
	@mkdir -p .tmp/images && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .tmp/images/ && ( cd `dirname .tmp/images` && $(TAR) images.tar images && $(GZIP) images.tar ) && $(MOVE) `dirname .tmp/images`/images.tar.gz . && $(DEL_FILE) -r .tmp/images

mocclean:

uiclean:

yaccclean:
lexclean:
clean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

images.o: images.cpp cur/brush.xpm \
		cur/circle.xpm \
		cur/contour.xpm \
		cur/dropper.xpm \
		cur/eraser.xpm \
		cur/hand.xpm \
		cur/magnifying.xpm \
		cur/nodes.xpm \
		cur/paint.xpm \
		cur/pencil.xpm \
		cur/pen.xpm \
		cur/select.xpm \
		cur/slicer.xpm \
		cur/square.xpm \
		cur/unpaint.xpm \
		ico/arrowdown.xpm \
		ico/arrowup.xpm \
		ico/bezier.xpm \
		ico/brush.xpm \
		ico/close.xpm \
		ico/contour.xpm \
		ico/copy.xpm \
		ico/cut.xpm \
		ico/disable.xpm \
		ico/dropper.xpm \
		ico/ellipse.xpm \
		ico/enable.xpm \
		ico/eraser.xpm \
		ico/export.xpm \
		ico/fillcolor.xpm \
		ico/fill.xpm \
		ico/fit.xpm \
		ico/frame-.xpm \
		ico/frame+.xpm \
		ico/front_back_grid.xpm \
		ico/hand.xpm \
		ico/import.xpm \
		ico/kilit_pic.xpm \
		ico/layer_pic.xpm \
		ico/layer-.xpm \
		ico/layer+.xpm \
		ico/line.xpm \
		ico/lock.xpm \
		ico/magnifying.xpm \
		ico/minussign.xpm \
		ico/new.xpm \
		ico/nodes.xpm \
		ico/open.xpm \
		ico/outline_pic.xpm \
		ico/paste.xpm \
		ico/pencil.xpm \
		ico/perspective.xpm \
		ico/plussign.xpm \
		ico/redo.xpm \
		ico/removefill.xpm \
		ico/save.xpm \
		ico/selection.xpm \
		ico/show_hide_all_layers.xpm \
		ico/slicer.xpm \
		ico/square.xpm \
		ico/undo.xpm \
		ico/written_pic.xpm \
		ico/zoom-in.xpm \
		ico/zoom-out.xpm \
		ico/grid12.xpm \
		ico/grid16.xpm \
		ico/subgrid.xpm \
		ico/nogrid.xpm \
		ico/insert_cg.xpm \
		ico/addfolder.xpm \
		ico/folder_icon.xpm \
		ico/redhat-graphics.xpm \
		ico/sound_widget.xpm \
		ico/ani_mode.xpm \
		ico/bring_to_front.xpm \
		ico/brushes.xpm \
		ico/camera_preview.xpm \
		ico/color_palette.xpm \
		ico/drawing_area.xpm \
		ico/exposure_sheet.xpm \
		ico/group.xpm \
		ico/illus_mode.xpm \
		ico/library.xpm \
		ico/light_table.xpm \
		ico/ntsc.xpm \
		ico/one_backward.xpm \
		ico/one_forward.xpm \
		ico/properties.xpm \
		ico/render.xpm \
		ico/scenes.xpm \
		ico/send_to_back.xpm \
		ico/side_camera.xpm \
		ico/time_line.xpm \
		ico/tools.xpm \
		ico/top_camera.xpm \
		ico/ungroup.xpm \
		ico/no_previous.xpm \
		ico/previous.xpm \
		ico/previous2.xpm \
		ico/previous3.xpm \
		ico/no_next.xpm \
		ico/next.xpm \
		ico/next2.xpm \
		ico/next3.xpm \
		ico/align_l.xpm \
		ico/align_r.xpm \
		ico/align_t.xpm \
		ico/align_b.xpm \
		ico/align_cv.xpm \
		ico/align_ch.xpm \
		ima/background.xpm \
		ima/splash.xpm

####### Install

install:  

uninstall:  

