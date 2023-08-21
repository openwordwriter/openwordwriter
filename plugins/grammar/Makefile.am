AM_CPPFLAGS = \
	$(GRAMMAR_CFLAGS) \
	-DABI_BUILD_VERSION=\"$(VERSION)\"

plugindir = $(ABIWORD_PLUGINSDIR)
plugin_LTLIBRARIES = grammar.la

grammar_la_LIBADD = \
	$(GRAMMAR_LIBS)

grammar_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined

grammar_la_SOURCES = \
	xp/AbiGrammarCheck.cpp \
	xp/AbiGrammarCheck.h \
	xp/AbiGrammar.cpp \
	xp/AbiGrammarUtil.cpp \
	xp/AbiGrammarUtil.h \
	linkgrammarwrap/LinkGrammarWrap.cpp \
	linkgrammarwrap/LinkGrammarWrap.h

EXTRA_DIST = \
	plugin.m4
