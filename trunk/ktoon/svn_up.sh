#!/bin/bash

\rm configure.tests/aspell/aspell
\rm configure.tests/aspell/Makefile
\rm configure.tests/ffmpeg/ffmpeg
\rm configure.tests/ffmpeg/Makefile
\rm configure.tests/opengl/opengl
\rm configure.tests/opengl/Makefile
\rm src/plugins/export/ffmpegplugin/Makefile
\rm src/themes/default/cursors/Makefile
\rm src/themes/default/icons/Makefile
\rm src/components/animation/Makefile
\rm ktconfig.pri

svn up
