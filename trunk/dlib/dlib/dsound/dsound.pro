# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/dlib/dsound
# Target is a library:  

DEFINES += DLIB_SOUND

INSTALLS += target  \
headers
target.path = /lib/ 

INCLUDEPATH += ../ 

CONFIG += release \
          warn_on \
          dll 


TEMPLATE = lib 
HEADERS += daudioengineiface.h \
           daudioplayer.h 
SOURCES += daudioplayer.cpp 

include(../dlibconfig.pri)


headers.path = /include/dsound
headers.files += *.h


