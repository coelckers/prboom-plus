#
# Copyright (C) 2000 by Colin Phipps (cph@lxdoom.linuxgames.com)
# License: GPL

CFLAGS = @CFLAGS@ @SDL_CFLAGS@

noinst_LIBRARIES = libpcsound.a

if BUILD_WINDOWS_PC_SPEAKER
PCS_SRC = pcsound_win32.c
else
PCS_SRC =
endif

libpcsound_a_SOURCES = \
  pcsound.c \
  pcsound.h \
  pcsound_sdl.c \
  $(PCS_SRC)

EXTRA_DIST = pcsound_linux.c
