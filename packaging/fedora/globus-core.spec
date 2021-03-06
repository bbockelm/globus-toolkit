%ifarch alpha ia64 ppc64 s390x sparc64 x86_64
%global flavor gcc64
%global enable64 yes
%else
%global flavor gcc32
%global enable64 no
%endif

%global debug_package %{nil}

%if "%{?rhel}" == "4" || "%{?rhel}" == "5" 
%global docdiroption "with-docdir"
%else
%global docdiroption "docdir"
%endif

%{!?perl_vendorlib: %global perl_vendorlib %(eval "`perl -V:installvendorlib`"; echo $installvendorlib)}

Name:		globus-core
%global _name %(tr - _ <<< %{name})
Version:	8.17
Release:	1%{?dist}
Summary:	Globus Toolkit - Globus Core

Group:		Development/Tools
License:	ASL 2.0
URL:		http://www.globus.org/
Source:		http://www.globus.org/ftppub/gt5/5.2/testing/packages/src/%{_name}-%{version}.tar.gz
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

Provides:	%{name}-devel = %{version}-%{release}
Obsoletes:	%{name}-devel < 5.15-0.5
Requires:	pkgconfig
Requires:	perl(XML::Parser)
BuildRequires:	grid-packaging-tools >= 3.4
BuildRequires:	perl(XML::Parser)

%description
The Globus Toolkit is an open source software toolkit used for building Grid
systems and applications. It is being developed by the Globus Alliance and
many others all over the world. A growing number of projects and companies are
using the Globus Toolkit to unlock the potential of grids for their cause.

The %{name} package contains:
Globus Core

%prep
%setup -q -n %{_name}-%{version}

# custom perl requires that removes dependency on gpt perl modules
cat << EOF > %{name}-req
#!/bin/sh
%{__perl_requires} $* |\
sed -e '/perl(Grid::GPT::.*)/d'
EOF
%global __perl_requires %{_builddir}/%{_name}-%{version}/%{name}-req
chmod +x %{__perl_requires}

%build
rm -rf autom4te.cache
unset GLOBUS_LOCATION
unset GPT_LOCATION
./bootstrap

%configure --includedir='${prefix}/include/globus' \
	   --libexecdir='${datadir}/globus' \
	   --with-flavor=%{flavor} \
	   --enable-64bit=%{enable64} \
           --enable-debug \
           --%{docdiroption}=%{_docdir}/%{name}-%{version} \
	   --with-setupdir='${datadir}/globus/setup' \
	   --with-testdir='${datadir}/globus/test/${PACKAGE}' \
	   --with-flavorincludedir='${libdir}/globus/include' \
	   --with-perlmoduledir=%{perl_vendorlib} \
	   --with-doxygendir='${datadir}/globus/doxygen' \
           --with-initializer-libdir-based-on-machine-type

make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

# These scripts are intended to be sourced, not executed
chmod 644 $RPM_BUILD_ROOT%{_datadir}/globus/globus-build-env-*.sh

GLOBUSPACKAGEDIR=$RPM_BUILD_ROOT%{_datadir}/globus/packages

# Don't use /usr/bin/env
sed 's!/usr/bin/env perl!/usr/bin/perl!' -i $RPM_BUILD_ROOT%{_sbindir}/globus-*

# Remove license file installed directly in the buildroot
rm -f $RPM_BUILD_ROOT/GLOBUS_LICENSE
sed /GLOBUS_LICENSE/d -i $GLOBUSPACKAGEDIR/%{_name}/noflavor_data.filelist

# Generate package filelists
cat $GLOBUSPACKAGEDIR/%{_name}/%{flavor}_pgm.filelist \
    $GLOBUSPACKAGEDIR/%{_name}/%{flavor}_dev.filelist \
    $GLOBUSPACKAGEDIR/%{_name}/noflavor_data.filelist \
  | grep -v noflavor_data.filelist \
  | sed s!^!%{_prefix}! > package.filelist

rm $GLOBUSPACKAGEDIR/%{_name}/noflavor_data.filelist

# man_MANS may get compressed
cat $GLOBUSPACKAGEDIR/%{_name}/noflavor_doc.filelist \
  | sed -e s!^!%{_prefix}! -e 's!\.[0-9]$!&*!' >> package.filelist

%clean
rm -rf $RPM_BUILD_ROOT

%files -f package.filelist
%defattr(-,root,root,-)
%dir %{_datadir}/globus
%dir %{_datadir}/globus/aclocal
%dir %{_datadir}/globus/doxygen
%dir %{_datadir}/globus/flavors
%dir %{_datadir}/globus/packages
%dir %{_datadir}/globus/packages/%{_name}
%dir %{_libdir}/globus
%dir %{_libdir}/globus/include
%{_bindir}/globus-spec-creator
%dir %{_docdir}/%{name}-%{version}

%changelog
* Fri Jan 10 2014 Globus Toolkit <support@globus.org> - 8.17-1
- Fix some bootstrapping issues

* Thu Oct 10 2013 Globus Toolkit <support@globus.org> - 8.16-2
- GT-446: Documentation built twice

* Wed Jun 26 2013 Globus Toolkit <support@globus.org> - 8.15-1
- GT-424: New Fedora Packaging Guideline - no %_isa in BuildRequires

* Thu Jun 20 2013 Globus Toolkit <support@globus.org> - 8.14-1
- GT-343: Add additional latex build requirements for newer fedora releases

* Thu Jun 20 2013 Globus Toolkit <support@globus.org> - 8.13-1
- GT-404: 5 times latex sometimes not enough

* Thu Jun 20 2013 Globus Toolkit <support@globus.org> - 8.12-1
- GT-403: More porting issues (aarch64 and x32)

* Thu Jun 20 2013 Globus Toolkit <support@globus.org> - 8.11-1
- GT-402: gpt-bootstrap should call automake with --force-missing

* Mon Mar 18 2013 Globus Toolkit <support@globus.org> - 8.10-1
- GT-354: Compatibility with automake 1.13

* Mon Nov 26 2012 Globus Toolkit <support@globus.org> - 8.9-2
- 5.2.3

* Tue Jul 17 2012 Joseph Bester <bester@mcs.anl.gov> - 8.9-1
- GT-251: Bad compile flags for S390 build

* Mon Jul 16 2012 Joseph Bester <bester@mcs.anl.gov> - 8.8-4
- GT 5.2.2 final

* Fri Jun 29 2012 Joseph Bester <bester@mcs.anl.gov> - 8.8-3
- GT 5.2.2 Release

* Wed May 09 2012 Joseph Bester <bester@mcs.anl.gov> - 8.8-2
- RHEL 4 patches

* Fri Apr 06 2012 Joseph Bester <bester@mcs.anl.gov> - 8.8-1
- RIC-252: solaris build failure

* Tue Feb 14 2012 Joseph Bester <bester@mcs.anl.gov> - 8.7-1
- RIC-206: globus-makefile-header doesn't set flavor header dir correctly
- RIC-232: Simplify search for OpenSSL headers and libraries

* Thu Dec 22 2011 Joseph Bester <bester@mcs.anl.gov> - 8.6-1
- RIC-206: globus-makefile-header doesn't set flavor header dir correctly

* Thu Dec 08 2011 Joseph Bester <bester@mcs.anl.gov> - 8.5-2
- Fix @INC handling for GLOBUS_LOCATION

* Mon Dec 05 2011 Joseph Bester <bester@mcs.anl.gov> - 8.4-2
- Update for 5.2.0 release

* Mon Dec 05 2011 Joseph Bester <bester@mcs.anl.gov> - 8.4-1
- Last sync prior to 5.2.0

* Thu Nov 03 2011 Joseph Bester <bester@mcs.anl.gov> - 8.3-1
- RIC-199: Can't install 32 and 64 bit Globus RPMs at the same time (missed
  perl libdir change)

* Fri Oct 28 2011 Joseph Bester <bester@mcs.anl.gov> - 8.2-1
- Minor fixes to build env sand spec creator scripts

* Tue Oct 25 2011 Joseph Bester <bester@mcs.anl.gov> - 8.1-5
- RIC-199: Can't install 32 and 64 bit Globus RPMs at the same time
- --with-initializer-libdir-based-on-machine-type

* Tue Oct 11 2011 Joseph Bester <bester@mcs.anl.gov> - 8.0-3
- Add explicit dependencies on >= 5.2 libraries

* Thu Sep 01 2011 Joseph Bester <bester@mcs.anl.gov> - 8.0-2
- Update for 5.1.2 release

* Sat Jul 17 2010 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.17-1
- Update to Globus Toolkit 5.0.2

* Mon Apr 12 2010 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.16-1
- Update to Globus Toolkit 5.0.1

* Thu Jan 21 2010 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.15-8
- Update to Globus Toolkit 5.0.0

* Mon Dec 07 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.15-7
- rebuild against perl 5.10.1

* Thu Jul 23 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.15-6
- The globus-spec-creator script now uses isa tags and noarch doc subpackages
- Replace /usr/bin/env shebangs

* Tue Jun 02 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.15-5
- Update to official Fedora Globus packaging guidelines
- Fix build configuration for s390x and kfreebsd
- Make globus-core work with automake 1.11

* Mon Apr 27 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.15-4
- Install the globus-spec-creator script
- Add -Wl,--as-needed to the libtool script

* Tue Apr 21 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.15-3
- Update after clarification of packaging guidelines

* Wed Apr 15 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.15-2
- Make comment about source retrieval more explicit

* Fri Mar 20 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.15-1
- Change defines to globals

* Sun Mar 15 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.15-0.5
- Merge devel with main

* Thu Feb 26 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.15-0.4
- Add s390x to the list of 64 bit platforms

* Mon Dec 29 2008 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.15-0.3
- Adapt to updated GPT package

* Sun Oct 12 2008 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.15-0.2
- Update to Globus Toolkit 4.2.1

* Mon Jul 14 2008 Mattias Ellert <mattias.ellert@fysast.uu.se> - 5.14-0.1
- Autogenerated
