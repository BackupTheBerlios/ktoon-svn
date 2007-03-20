# Subdir relative project main directory: ./src/themes/default/icons
# Target is a library:  

INSTALLS += icons 
icons.files += *.png 
icons.path = /themes/default/icons 
CONFIG += release \
          warn_on \
          staticlib 

TEMPLATE = lib

# little hack
macx {
TEMPLATE=subdirs
}

