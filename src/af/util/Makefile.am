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

noinst_LTLIBRARIES = libutil.la

AM_CPPFLAGS = \
	$(AF_CPPFLAGS)

libutil_la_SOURCES = \
	xp/ut_AdobeEncoding.cpp \
	xp/ut_AdobeEncoding.h \
	xp/ut_assert.h \
	xp/ut_base64.cpp \
	xp/ut_base64.h \
	xp/ut_base64_TestRoutines.cpp \
	xp/ut_bijection.cpp \
	xp/ut_bijection.h \
	xp/ut_bytebuf.cpp \
	xp/ut_bytebuf.h \
	xp/ut_case.h \
	xp/ut_color.h \
	xp/ut_color.cpp \
	xp/ut_compiler.h \
	xp/ut_crc32.cpp \
	xp/ut_crc32.h \
	xp/ut_debugmsg.h \
	xp/ut_decompress.cpp \
	xp/ut_decompress.h \
	xp/ut_Encoding.cpp \
	xp/ut_Encoding.h \
	xp/ut_endian.h \
	xp/ut_exception.h \
	xp/ut_export.h \
	xp/ut_go_file.cpp \
	xp/ut_go_file.h \
	xp/ut_growbuf.cpp \
	xp/ut_growbuf.h \
	xp/ut_hash.cpp \
	xp/ut_hash.h \
	xp/ut_html.cpp \
	xp/ut_html.h \
	xp/ut_iconv.cpp \
	xp/ut_iconv.h \
	xp/ut_idle.cpp \
	xp/ut_idle.h \
	xp/ut_jpeg.cpp \
	xp/ut_jpeg.h \
	xp/ut_Language.cpp \
	xp/ut_Language.h \
	xp/ut_locale.cpp \
	xp/ut_locale.h \
	xp/ut_math.cpp \
	xp/ut_math.h \
	xp/ut_mbtowc.cpp \
	xp/ut_mbtowc.h \
	xp/ut_misc.cpp \
	xp/ut_misc.h \
	xp/ut_mutex.cpp \
	xp/ut_mutex.h \
	xp/ut_null.h \
	xp/ut_OverstrikingChars.cpp \
	xp/ut_OverstrikingChars.h \
	xp/ut_path.h \
	xp/ut_path.cpp \
	xp/ut_png.cpp \
	xp/ut_png.h \
	xp/ut_rand.cpp \
	xp/ut_rand.h \
	xp/ut_Script.cpp \
	xp/ut_Script.h \
	xp/ut_stack.cpp \
	xp/ut_stack.h \
	xp/ut_std_algo.h \
	xp/ut_std_map.h \
	xp/ut_std_string.h \
	xp/ut_std_string.cpp \
	xp/ut_std_vector.h \
	xp/ut_stringbuf.cpp \
	xp/ut_stringbuf.h \
	xp/ut_string_class.cpp \
	xp/ut_string_class.h \
	xp/ut_string.cpp \
	xp/ut_string.h \
	xp/ut_strptime.c \
	xp/ut_svg.cpp \
	xp/ut_svg.h \
	xp/ut_test.cpp \
	xp/ut_test.h \
	xp/ut_TextIterator.h \
	xp/ut_timer.cpp \
	xp/ut_timer.h \
	xp/ut_types.cpp \
	xp/ut_types.h \
	xp/ut_unicode.cpp \
	xp/ut_unicode.h \
	xp/ut_units.cpp \
	xp/ut_units.h \
	xp/ut_uuid.cpp \
	xp/ut_uuid.h \
	xp/ut_vector.h \
	xp/ut_warning.h \
	xp/ut_wctomb.cpp \
	xp/ut_wctomb.h \
	xp/ut_worker.cpp \
	xp/ut_worker.h \
	xp/ut_xml.cpp \
	xp/ut_xml.h \
	xp/ut_xml_libxml2.cpp \
	xp/ut_conversion.h

if PLATFORM_UNIX
libutil_la_SOURCES += \
	unix/ut_debugmsg.cpp \
	unix/ut_files.cpp \
	unix/ut_files.h \
	unix/ut_mutexImpl.h \
	unix/ut_path.cpp \
	unix/ut_sleep.h \
	unix/ut_unixAssert.cpp \
	unix/ut_unixAssert.h \
	unix/ut_unixIdle.cpp \
	unix/ut_unixIdle.h \
	unix/ut_unixMisc.cpp \
	unix/ut_unixColor.cpp \
	unix/ut_unixColor.h \
	unix/ut_unixTimer.cpp \
	unix/ut_unixTimer.h \
	$(NULL)
endif
if PLATFORM_WIN
libutil_la_SOURCES += \
	win/ut_debugmsg.cpp \
	win/ut_mutexImpl.h \
	win/ut_path.cpp \
	win/ut_sleep.cpp \
	win/ut_sleep.h \
	win/ut_Win32Idle.cpp \
	win/ut_Win32Idle.h \
	win/ut_Win32Locale.cpp \
	win/ut_Win32Locale.h \
	win/ut_Win32Misc.cpp \
	win/ut_Win32OS.cpp \
	win/ut_Win32OS.h \
	win/ut_Win32String.cpp \
	win/ut_Win32Timer.cpp \
	win/ut_Win32Timer.h \
	win/ut_Win32Uuid.cpp \
	win/ut_Win32Uuid.h \
	win/ut_Xpm2Bmp.cpp \
	win/ut_Xpm2Bmp.h \
	win/ut_Win32LocaleString.cpp \
	win/ut_Win32LocaleString.h \
	$(NULL)

rc2files = \
	win/ut_Win32Resources.rc2

endif

EXTRA_DIST = \
	xp/t/ut_bytebuf.t.cpp \
	xp/t/ut_locale.t.cpp \
	xp/t/ut_misc.t.cpp \
	xp/t/ut_stack.t.cpp \
	xp/t/ut_std_string.t.cpp \
	xp/t/ut_string.t.cpp \
	xp/t/ut_string_class.t.cpp \
	xp/t/ut_types.t.cpp \
	xp/t/ut_units.t.cpp \
	xp/t/ut_uuid.t.cpp \
	xp/t/ut_vector.t.cpp \
	$(rc2files)
