include $(top_srcdir)/globus_automake_pre

## segconf = [ DATA ]
segconfdir = @SEG_CONFIGDIR@
segconf_DATA = @SEG_CONFIGFILE@

@SEG_CONFIGFILE@: globus-scheduler-event-generator.in
	../config.status --file @SEG_CONFIGFILE@:$(srcdir)/globus-scheduler-event-generator.in

CLEANFILES = @SEG_CONFIGFILE@

EXTRA_DIST = globus-scheduler-event-generator.in

include $(top_srcdir)/globus_automake_post
