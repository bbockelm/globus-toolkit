%ifarch alpha ia64 ppc64 s390x sparc64 x86_64
%global flavor gcc64
%else
%global flavor gcc32
%endif

%if "%{?rhel}" == "4" || "%{?rhel}" == "5"
%global docdiroption "with-docdir"
%else
%global docdiroption "docdir"
%endif

Name:		globus-gass-transfer
%global _name %(tr - _ <<< %{name})
Version:	7.2
Release:	8%{?dist}
Summary:	Globus Toolkit - Globus Gass Transfer

Group:		System Environment/Libraries
License:	ASL 2.0
URL:		http://www.globus.org/
Source:		http://www.globus.org/ftppub/gt5/5.2/testing/packages/src/%{_name}-%{version}.tar.gz
#		This is a workaround for the broken epstopdf script in RHEL5
#		See: https://bugzilla.redhat.com/show_bug.cgi?id=450388
Source9:	epstopdf-2.9.5gw
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

Requires:	globus-common%{?_isa} >= 14
Requires:	globus-io%{?_isa} >= 8
Requires:	globus-gssapi-gsi%{?_isa} >= 9

BuildRequires:	grid-packaging-tools >= 3.4
BuildRequires:	globus-common-devel >= 14
BuildRequires:	globus-io-devel >= 8
BuildRequires:	globus-gssapi-gsi-devel >= 9
BuildRequires:	globus-core >= 8
BuildRequires:	doxygen
BuildRequires:	graphviz
%if "%{?rhel}" == "5"
BuildRequires:	graphviz-gd
%endif
BuildRequires:	ghostscript
%if %{?fedora}%{!?fedora:0} >= 9 || %{?rhel}%{!?rhel:0} >= 6
BuildRequires:	tex(latex)
%else
%if 0%{?suse_version} > 0
BuildRequires:  texlive-latex
%else
BuildRequires:	tetex-latex
%endif
%endif

%if %{?fedora}%{!?fedora:0} == 18
BuildRequires: tex(sectsty.sty)
BuildRequires: tex(tocloft.sty)
BuildRequires: tex(xtab.sty)
BuildRequires: tex(multirow.sty)
BuildRequires: tex(fullpage.sty)
%endif

%package devel
Summary:	Globus Toolkit - Globus Gass Transfer Development Files
Group:		Development/Libraries
Requires:	%{name}%{?_isa} = %{version}-%{release}
Requires:	globus-common-devel%{?_isa} >= 14
Requires:	globus-io-devel%{?_isa} >= 8
Requires:	globus-gssapi-gsi-devel%{?_isa} >= 9
Requires:	globus-core%{?_isa} >= 8

%package doc
Summary:	Globus Toolkit - Globus Gass Transfer Documentation Files
Group:		Documentation
%if %{?fedora}%{!?fedora:0} >= 10 || %{?rhel}%{!?rhel:0} >= 6
BuildArch:	noarch
%endif
Requires:	%{name} = %{version}-%{release}

%description
The Globus Toolkit is an open source software toolkit used for building Grid
systems and applications. It is being developed by the Globus Alliance and
many others all over the world. A growing number of projects and companies are
using the Globus Toolkit to unlock the potential of grids for their cause.

The %{name} package contains:
Globus Gass Transfer

%description devel
The Globus Toolkit is an open source software toolkit used for building Grid
systems and applications. It is being developed by the Globus Alliance and
many others all over the world. A growing number of projects and companies are
using the Globus Toolkit to unlock the potential of grids for their cause.

The %{name}-devel package contains:
Globus Gass Transfer Development Files

%description doc
The Globus Toolkit is an open source software toolkit used for building Grid
systems and applications. It is being developed by the Globus Alliance and
many others all over the world. A growing number of projects and companies are
using the Globus Toolkit to unlock the potential of grids for their cause.

The %{name}-doc package contains:
Globus Gass Transfer Documentation Files

%prep
%setup -q -n %{_name}-%{version}

%if "%{rhel}" == "5"
mkdir bin
install %{SOURCE9} bin/epstopdf
%endif

%build
%if "%{rhel}" == "5"
export PATH=$PWD/bin:$PATH
%endif

# Remove files that should be replaced during bootstrap
rm -f doxygen/Doxyfile*
rm -f doxygen/Makefile.am
rm -f pkgdata/Makefile.am
rm -f globus_automake*
rm -rf autom4te.cache
unset GLOBUS_LOCATION
unset GPT_LOCATION

%{_datadir}/globus/globus-bootstrap.sh

%configure --with-flavor=%{flavor} --enable-doxygen \
           --%{docdiroption}=%{_docdir}/%{name}-%{version} \
           --disable-static

make %{?_smp_mflags}

%install
%if "%{rhel}" == "5"
export PATH=$PWD/bin:$PATH
%endif

rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

GLOBUSPACKAGEDIR=$RPM_BUILD_ROOT%{_datadir}/globus/packages

# Remove libtool archives (.la files)
find $RPM_BUILD_ROOT%{_libdir} -name 'lib*.la' -exec rm -v '{}' \;
sed '/lib.*\.la$/d' -i $GLOBUSPACKAGEDIR/%{_name}/%{flavor}_dev.filelist

# Remove unwanted documentation (needed for RHEL4)
rm -f $RPM_BUILD_ROOT%{_mandir}/man3/*_%{_name}-%{version}_*.3
sed -e '/_%{_name}-%{version}_.*\.3/d' \
  -i $GLOBUSPACKAGEDIR/%{_name}/noflavor_doc.filelist

# Generate package filelists
cat $GLOBUSPACKAGEDIR/%{_name}/%{flavor}_rtl.filelist \
  | sed s!^!%{_prefix}! > package.filelist
cat $GLOBUSPACKAGEDIR/%{_name}/%{flavor}_dev.filelist \
  | sed s!^!%{_prefix}! > package-devel.filelist
cat $GLOBUSPACKAGEDIR/%{_name}/noflavor_doc.filelist \
  | grep -v 'GLOBUS_LICENSE' \
  | sed -e 's!/man/.*!&*!' -e 's!^!%doc %{_prefix}!' > package-doc.filelist

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files -f package.filelist
%defattr(-,root,root,-)
%dir %{_datadir}/globus/packages/%{_name}
%dir %{_docdir}/%{name}-%{version}
%doc %{_docdir}/%{name}-%{version}/GLOBUS_LICENSE

%files -f package-devel.filelist devel
%defattr(-,root,root,-)

%files -f package-doc.filelist doc
%defattr(-,root,root,-)
%dir %{_docdir}/%{name}-%{version}/html

%changelog
* Wed Jun 26 2013 Globus Toolkit <support@globus.org> - 7.2-8
- GT-424: New Fedora Packaging Guideline - no %_isa in BuildRequires

* Wed Feb 20 2013 Globus Toolkit <support@globus.org> - 7.2-7
- Workaround missing F18 doxygen/latex dependency

* Mon Nov 26 2012 Globus Toolkit <support@globus.org> - 7.2-6
- 5.2.3

* Mon Jul 16 2012 Joseph Bester <bester@mcs.anl.gov> - 7.2-5
- GT 5.2.2 final

* Fri Jun 29 2012 Joseph Bester <bester@mcs.anl.gov> - 7.2-4
- GT 5.2.2 Release

* Wed May 09 2012 Joseph Bester <bester@mcs.anl.gov> - 7.2-3
- RHEL 4 patches

* Fri May 04 2012 Joseph Bester <bester@mcs.anl.gov> - 7.2-2
- SLES 11 patches

* Tue Feb 14 2012 Joseph Bester <bester@mcs.anl.gov> - 7.2-1
- RIC-226: Some dependencies are missing in GPT metadata

* Mon Dec 05 2011 Joseph Bester <bester@mcs.anl.gov> - 7.1-4
- Update for 5.2.0 release

* Mon Dec 05 2011 Joseph Bester <bester@mcs.anl.gov> - 7.1-3
- Last sync prior to 5.2.0

* Thu Oct 20 2011 Joseph Bester <bester@mcs.anl.gov> - 7.1-2
- Add explicit dependencies on >= 5.2 libraries
- GRAM-220: GASS server may deadlock

* Thu Oct 06 2011 Joseph Bester <bester@mcs.anl.gov> - 7.1-1
- Add backward-compatibility aging

* Thu Sep 01 2011 Joseph Bester <bester@mcs.anl.gov> - 7.0-2
- Update for 5.1.2 release

* Sat Jan 23 2010 Mattias Ellert <mattias.ellert@fysast.uu.se> - 4.3-1
- Update to Globus Toolkit 5.0.0

* Thu Jul 23 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 3.4-3
- Add instruction set architecture (isa) tags
- Make doc subpackage noarch

* Thu Jun 04 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 3.4-2
- Update to official Fedora Globus packaging guidelines

* Thu Apr 16 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 3.4-1
- Make comment about source retrieval more explicit
- Change defines to globals
- Remove explicit requires on library packages
- Put GLOBUS_LICENSE file in extracted source tarball

* Sun Mar 15 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 3.4-0.5
- Adapting to updated globus-core package

* Thu Feb 26 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 3.4-0.4
- Add s390x to the list of 64 bit platforms

* Thu Jan 01 2009 Mattias Ellert <mattias.ellert@fysast.uu.se> - 3.4-0.3
- Adapt to updated GPT package

* Tue Oct 21 2008 Mattias Ellert <mattias.ellert@fysast.uu.se> - 3.4-0.2
- Update to Globus Toolkit 4.2.1

* Tue Jul 15 2008 Mattias Ellert <mattias.ellert@fysast.uu.se> - 3.3-0.1
- Autogenerated
