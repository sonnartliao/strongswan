ipsec_PROGRAMS = charon

charon_SOURCES = \
charon.c

charon.o :	$(top_builddir)/config.status

AM_CPPFLAGS = \
	-I$(top_srcdir)/src/libstrongswan \
	-I$(top_srcdir)/src/libcharon \
	-DIPSEC_DIR=\"${ipsecdir}\" \
	-DIPSEC_PIDDIR=\"${piddir}\" \
	-DPLUGINS=\""${charon_plugins}\"" \
	-I$(top_srcdir)/src/libipcmsgqueue
	

charon_LDADD = \
	$(top_builddir)/src/libstrongswan/libstrongswan.la \
	$(top_builddir)/src/libcharon/libcharon.la \
	-lm $(PTHREADLIB) $(ATOMICLIB) $(DLLIB) \
	-L$(top_builddir)/src/libipcmsgqueue -lipcmsgqueue

EXTRA_DIST = Android.mk
