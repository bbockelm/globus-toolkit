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

Name:		globus-gridmap-eppn-callout
%global _name %(tr - _ <<< %{name})
Version:	0.6
Release:	1%{?dist}
Summary:	Globus Toolkit - Globus gridmap eppn callout.

Group:		System Environment/Libraries
License:	ASL 2.0
URL:		http://www.globus.org/
Source:		http://www.globus.org/ftppub/gt5/5.2/testing/packages/src/%{_name}-%{version}.tar.gz
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

Requires:	globus-common%{?_isa} >= 14
BuildRequires:	grid-packaging-tools >= 3.6
BuildRequires:	globus-core >= 8
BuildRequires:	globus-gsi-sysconfig-devel >= 1
BuildRequires:	globus-gss-assist-devel >= 3
BuildRequires:	globus-gridmap-callout-error-devel
BuildRequires:	globus-gssapi-gsi-devel >= 4
BuildRequires:	globus-gsi-credential-devel >= 6

%description
The Globus Toolkit is an open source software toolkit used for building Grid
systems and applications. It is being developed by the Globus Alliance and
many others all over the world. A growing number of projects and companies are
using the Globus Toolkit to unlock the potential of grids for their cause.

The %{name} package contains:
Globus gridmap eppn callout.

%prep
%setup -q -n %{_name}-%{version}

%build
# Remove files that should be replaced during bootstrap
rm -f doxygen/Doxyfile*
rm -f doxygen/Makefile.am
rm -f pkgdata/Makefile.am
rm -f globus_automake*
rm -rf autom4te.cache

unset GLOBUS_LOCATION
unset GPT_LOCATION
%{_datadir}/globus/globus-bootstrap.sh

%configure --disable-static --with-flavor=%{flavor} \
	   --%{docdiroption}=%{_docdir}/%{name}-%{version}

make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

GLOBUSPACKAGEDIR=$RPM_BUILD_ROOT%{_datadir}/globus/packages

# Remove libtool archives (.la files)
find $RPM_BUILD_ROOT%{_libdir} -name 'lib*.la' -exec rm -v '{}' \;
sed '/lib.*\.la$/d' -i $GLOBUSPACKAGEDIR/%{_name}/%{flavor}_dev.filelist

# Remove pkgconf file
find $RPM_BUILD_ROOT%{_libdir} -name '*.pc' -exec rm -v '{}' \;
sed '/lib.*\.pc$/d' -i $GLOBUSPACKAGEDIR/%{_name}/%{flavor}_dev.filelist

# Generate package filelists
cat $GLOBUSPACKAGEDIR/%{_name}/%{flavor}_dev.filelist \
    $GLOBUSPACKAGEDIR/%{_name}/%{flavor}_rtl.filelist \
    $GLOBUSPACKAGEDIR/%{_name}/noflavor_doc.filelist \
    $GLOBUSPACKAGEDIR/%{_name}/noflavor_data.filelist \
  | grep -v '^/etc' \
  | sed s!^!%{_prefix}! > package.filelist
grep '^/etc' \
    $GLOBUSPACKAGEDIR/%{_name}/noflavor_data.filelist \
  >> package.filelist

%clean
rm -rf $RPM_BUILD_ROOT

%post
/sbin/ldconfig

%postun -p /sbin/ldconfig

%files -f package.filelist
%defattr(-,root,root,-)
%dir %{_datadir}/globus/packages/%{_name}
%dir %{_docdir}/%{name}-%{version}
%config(noreplace) %{_sysconfdir}/gridmap_eppn_callout-gsi_authz.conf


%changelog
* Mon Oct 28 2013 Globus Toolkit <support@globus.org> - 0.6-1
- Update dependencies for new credential functions

* Mon Oct 28 2013 Globus Toolkit <support@globus.org> - 0.5-2
- Update dependencies for new credential functions

* Fri Oct 25 2013 Globus Toolkit <support@globus.org> - 0.5-1
- Missing configure dependency on OpenSSL

* Wed Jun 26 2013 Globus Toolkit <support@globus.org> - 0.4-2
- GT-424: New Fedora Packaging Guideline - no %_isa in BuildRequires

* Mon Apr 15 2013 Globus Toolkit <support@globus.org> - 0.4-1
- verify sharing cert chain

* Tue Mar 19 2013 Globus Toolkit <support@globus.org> - 0.3-2
- Update sharing to support a full cert chain at logon

* Tue Mar 12 2013 Globus Toolkit <support@globus.org> - 0.3-1
- Improve error message handling

* Tue Mar 05 2013 Globus Toolkit <support@globus.org> - 0.2-1
- Initial version
