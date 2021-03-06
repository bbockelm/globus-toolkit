## Special globus installation directories

setupdir = @setupdir@
## setup = [ DATA,PROGRAMS,SCRIPTS ]

testdir = @testdir@
## test = [ DATA,PROGRAMS,SCRIPTS ]

scriptsysconfdir = @sysconfdir@
## scriptsysconf = [ PROGRAMS,SCRIPTS ]

flavorincludedir = @flavorincludedir@
## flavorinclude = [ HEADERS ]

pkgdir = @pkgdir@
## pkg = [ DATA ]

aclocaldir = @aclocaldir@
## aclocal = [ DATA ]

amdirdir = @amdirdir@
## amdir = [ DATA ]

doxygendir = @doxygendir@
## doxygen = [ DATA ]

docdir = @docdir@
## doc = [ DATA ]

flavordir = @flavordir@
## flavor = [ DATA ]

pkgconfdir = @libdir@/pkgconfig
## pkgconfdir = [ DATA ]

pkgconf_DATA = globus-core.pc

flavor_DATA = flavor_$(GLOBUS_FLAVOR_NAME).gpt

EXTRA_DIST = pkg_data_src.pc.in

LINK_BAK = orig
AUTOMAKE_OPTIONS=no-dependencies

FILELIST_FILES = noflavor_data.filelist \
	$(GLOBUS_FLAVOR_NAME)_dev.filelist \
	noflavor_doc.filelist \
	$(GLOBUS_FLAVOR_NAME)_pgm.filelist \
	$(GLOBUS_FLAVOR_NAME)_pgm_static.filelist \
	$(GLOBUS_FLAVOR_NAME)_rtl.filelist


METADATA_FILES = \
	pkg_data_noflavor_data.gpt \
	pkg_data_$(GLOBUS_FLAVOR_NAME)_dev.gpt \
	pkg_data_noflavor_doc.gpt \
	pkg_data_$(GLOBUS_FLAVOR_NAME)_pgm.gpt \
	pkg_data_$(GLOBUS_FLAVOR_NAME)_pgm_static.gpt \
	pkg_data_$(GLOBUS_FLAVOR_NAME)_rtl.gpt

pkg_DATA = $(FILELIST_FILES) $(METADATA_FILES)

filelist_file: $(MAKEFILE_AMS)
	@for p in $(FILELIST_FILES); do \
	    if test -f "$$p"; then \
	        rm -f $$p; \
	    fi; \
        done

all_filelists: filelist_file
	 @(cd .. && $(MAKE) $(AM_MAKEFLAGS) filelist) \
## This trick allows "-k" to keep its natural meaning when running a
## recursive rule.
	  || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac;


$(FILELIST_FILES): all_filelists

# Override the normal install target to avoid dying when a given
# kind of metadata file is missing by calling "true"
install-pkgDATA: $(pkg_DATA)
	@$(NORMAL_INSTALL)
	test -z "$(pkgdir)" || $(mkinstalldirs) "$(DESTDIR)$(pkgdir)";
	@list='$(pkg_DATA)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f=`echo $$p | sed -e 's|^.*/||'`; \
	  if test -f "$$d$$p"; then \
	      echo " $(INSTALL_DATA) '$$d$$p' '$(DESTDIR)$(pkgdir)/$$f'"; \
	      $(INSTALL_DATA) "$$d$$p" "$(DESTDIR)$(pkgdir)/$$f"; \
	    fi; \
	done || true

$(flavor_DATA): build-parameters
	$(GPT_LOCATION)/sbin/gpt_save_flavor --flavor=$(GLOBUS_FLAVOR_NAME) build-parameters

$(METADATA_FILES): pkg_data_src.gpt
	$(GPT_LOCATION)/sbin/gpt_generate_bin_pkg_data  \
	--flavor=$(GLOBUS_FLAVOR_NAME) pkg_data_src.gpt


distclean-local:
	@if test -f master.filelist; then \
		rm *.filelist; \
	fi; \
	if test -f pkg_data_noflavor_data.gpt; then \
		rm pkg_data_*.gpt; \
	fi

dist-hook: filelist_file

install-data-hook:
	@list='$(FILELIST_FILES)'; for p in $$list; do \
	    if test `cat "$(DESTDIR)$(pkgdir)/$$p" | wc -l` = 2; then \
		rm "$(DESTDIR)$(pkgdir)/$$p" ; \
	    fi; \
	done
	@list='$(METADATA_FILES)'; for p in $$list; do \
	    related_filelist=`echo $$p | sed -e 's/pkg_data_//' -e 's/\.gpt$$/.filelist/'`; \
	    if test ! -f "$(DESTDIR)$(pkgdir)/$$related_filelist"; then \
		rm $(DESTDIR)$(pkgdir)/$$p; \
	    fi ; \
	done
