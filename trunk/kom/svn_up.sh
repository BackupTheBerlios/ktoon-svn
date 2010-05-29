#!/bin/bash

\rm configure.tests/aspell/aspell
\rm configure.tests/aspell/Makefile
\rm configure.tests/ffmpeg/ffmpeg
\rm configure.tests/ffmpeg/Makefile
\rm configure.tests/opengl/opengl
\rm configure.tests/opengl/Makefile
\rm configure.tests/sound/sound
\rm configure.tests/sound/Makefile
\rm kom/komconfig.pri

svn up
