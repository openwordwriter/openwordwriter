## AbiSource Applications
## Copyright (C) 2001 Sam Tobin-Hochstadt
## Copyright (C) 2023 Hubert Figuière
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
## 02110-1301 USA.

if PLATFORM_UNIX
TESTS = unix/testwrap.sh

check_SCRIPTS = testwrap.sh-stamp unix/testwrap.sh
check_PROGRAMS = AbiWord-test

AbiWord_test_CPPFLAGS = $(WP_CPPFLAGS) \
	-DABIWORD_DATADIR="\"$(ABIWORD_DATADIR)\"" \
	-I$(top_srcdir)/src/af/tf/xp/

AbiWord_test_LDFLAGS =  -L$(top_builddir)/src -labiword-@ABIWORD_SERIES@ \
	-L$(top_builddir)/src/af/tf -labiword-@ABIWORD_SERIES@-test

AbiWord_test_LDADD =  $(DEPS_LIBS)

AbiWord_test_SOURCES = unix/test_main.cpp \
	unix/all_test_unix.h

testwrap.sh-stamp: unix/testwrap.sh
	chmod a+x unix/testwrap.sh
	touch $@
endif

CLEANFILES = vgcore.* \
	supp.txt \
	testwrap.sh-stamp \
	tf-vgdump

EXTRA_DIST = \
	xp/main.cpp \
	xp/all_test.h \
	win/test_main.cpp \
	cocoa/test_main.cpp
