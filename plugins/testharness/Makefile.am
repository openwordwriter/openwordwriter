AM_CPPFLAGS = \
	$(TESTHARNESS_CFLAGS) \
	-I$(top_srcdir)/src/af/tf/xp/ \
	-DABI_BUILD_VERSION=\"$(VERSION)\"

if !TOOLKIT_WIN

plugindir = $(ABIWORD_PLUGINSDIR)
plugin_LTLIBRARIES = testharness.la

testharness_la_LIBADD = \
	$(TESTHARNESS_LIBS)

testharness_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined \
	-L$(top_builddir)/src/af/tf -labiword-@ABIWORD_SERIES@-test \
	$(null)

testharness_la_SOURCES = \
	xp/AbiTest.cpp \
	xp/AbiTest.h \
	@top_srcdir@/src/af/util/xp/t/ut_bytebuf.t.cpp \
	@top_srcdir@/src/af/util/xp/t/ut_misc.t.cpp \
	@top_srcdir@/src/af/util/xp/t/ut_stack.t.cpp \
	@top_srcdir@/src/af/util/xp/t/ut_string_class.t.cpp \
	@top_srcdir@/src/af/util/xp/t/ut_std_string.t.cpp \
	@top_srcdir@/src/af/util/xp/t/ut_types.t.cpp \
	@top_srcdir@/src/af/util/xp/t/ut_vector.t.cpp \
	@top_srcdir@/src/af/util/xp/t/ut_string.t.cpp \
	@top_srcdir@/src/af/util/xp/t/ut_units.t.cpp \
	@top_srcdir@/src/af/util/xp/t/ut_uuid.t.cpp \
	@top_srcdir@/src/af/util/xp/t/ut_locale.t.cpp \
	@top_srcdir@/src/af/xap/xp/t/xap_Prefs.t.cpp \
	@top_srcdir@/src/text/ptbl/xp/t/pt_PieceTable.t.cpp \
	@top_srcdir@/src/text/ptbl/xp/t/pp_PropertyMap.t.cpp \
	@top_srcdir@/src/text/ptbl/xp/t/pp_AttrProp.t.cpp \
	@top_srcdir@/src/text/ptbl/xp/t/pf_Fragments.t.cpp \
	@top_srcdir@/src/text/fmt/xp/t/fl_AutoNum.t.cpp \
	@top_srcdir@/src/wp/impexp/xp/t/ie_mailmerge.t.cpp \
	@top_srcdir@/src/wp/impexp/xp/t/ie_Table.t.cpp \
	$(NULL)

endif
