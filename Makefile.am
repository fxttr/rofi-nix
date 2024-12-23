ACLOCAL_AMFLAGS=-I m4
plugindir=${libdir}/rofi/

plugin_LTLIBRARIES = rofinix.la

rofinix_la_SOURCES=\
		 src/rofi-nix.c

rofinix_la_CFLAGS= @glib_CFLAGS@ @rofi_CFLAGS@
rofinix_la_LIBADD= @glib_LIBS@ @rofi_LIBS@
rofinix_la_LDFLAGS= -module -avoid-version
