SUBDIRS = gda

# Translation memory plugin
plugindir = $(libdir)/gtranslator/plugins

INCLUDES = \
	-I$(top_srcdir)					\
	-I$(top_srcdir)/src				\
	$(GTRANSLATOR_CFLAGS)				\
	$(WARN_CFLAGS)					\
	$(DISABLE_DEPRECATED)

plugin_LTLIBRARIES = \
	libtranslation-memory.la

BUILT_SOURCES = \
	gtr-translation-memory-resources.c

libtranslation_memory_la_SOURCES = \
	gtr-translation-memory-window-activatable.c \
	gtr-translation-memory-window-activatable.h \
	gtr-translation-memory-tab-activatable.c \
	gtr-translation-memory-utils.c \
	gtr-translation-memory-utils.h \
	gtr-translation-memory-tab-activatable.h \
	gtr-translation-memory-ui.c \
	gtr-translation-memory-ui.h \
	gtr-translation-memory-dialog.c \
	gtr-translation-memory-dialog.h \
	gtr-translation-memory.c \
	gtr-translation-memory.h \
	gtr-translation-memory-plugin.h \
	gtr-translation-memory-plugin.c \
	$(BUILT_SOURCES)

libtranslation_memory_la_LDFLAGS = \
	$(PLUGIN_LIBTOOL_FLAGS)				\
	$(GTRANSLATOR_LIBS)				\
	gda/libgda.la

gtr-translation-memory-resources.c: gtr-translation-memory.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies gtr-translation-memory.gresource.xml)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-source gtr-translation-memory.gresource.xml

# Plugin Info

plugin_in_files = gtr-translation-memory.plugin.desktop.in

%.plugin: %.plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.plugin.desktop.in=.plugin)

gsettings_SCHEMAS = org.gnome.gtranslator.plugins.translation-memory.gschema.xml
@GSETTINGS_RULES@

EXTRA_DIST = \
	$(plugin_in_files) \
	gtr-translation-memory.gresource.xml \
	gtr-translation-memory-dialog.ui

CLEANFILES = $(plugin_DATA) $(BUILT_SOURCES)
DISTCLEANFILES = $(plugin_DATA) $(BUILT_SOURCES)

-include $(top_srcdir)/git.mk
