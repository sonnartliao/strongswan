AM_CPPFLAGS = \
	-I$(top_srcdir)/src/libstrongswan \
	-I$(top_srcdir)/src/libcharon \
	-I$(top_srcdir)/src/libipcmsgqueue

AM_CFLAGS = \
	$(PLUGIN_CFLAGS)

if MONOLITHIC
noinst_LTLIBRARIES = libstrongswan-counters.la
else
plugin_LTLIBRARIES = libstrongswan-counters.la
endif

libstrongswan_counters_la_SOURCES = \
	counters_plugin.h counters_plugin.c \
	counters_listener.h counters_listener.c \
	counters_query.h

libstrongswan_counters_la_LDFLAGS = -module -avoid-version
