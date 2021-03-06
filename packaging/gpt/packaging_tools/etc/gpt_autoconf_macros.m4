dnl GPT_INIT()
AC_DEFUN([GPT_INIT], [
        GPT_LOCATION="${GPT_LOCATION:-${GLOBUS_LOCATION:-/usr}}"

	dnl --with-docdir for older autoconf (<2.60)
	AC_ARG_WITH([docdir],
	AC_HELP_STRING([--with-docdir=DIR], [Install documentation in DIR [[DATADIR/doc/PACKAGE]]]),
        [case $withval in 
	  yes|no)
		AC_MSG_ERROR([Invalid DIR])
		;;
	  *)
		docdir="$withval"
		;;
	esac
	],
	[
                if test -z "$docdir"; then
                        docdir='${datadir}/doc/${PACKAGE}'
                fi
	])
	AC_SUBST(docdir)


        AC_SUBST(GPT_LOCATION)
        AC_SUBST(GPT_PKGCONFIG_DEPENDENCIES)

        # bootstrap extracts the the name and version of the package from the
        # src metadata into gptdata.sh for easier processing
        . ${srcdir}/gptdata.sh
	GPT_VERSION="$GPT_MAJOR_VERSION.$GPT_MINOR_VERSION"

        # Determine if GPT is version 2.x

        GPT_IS_2="no"

        if test -f "$GPT_LOCATION/sbin/gpt-build"; then
                GPT_IS_2="yes"
        fi

        AC_SUBST(GPT_IS_2)

	#Default to shared, before checking static-only
	GPT_LINKTYPE="shared"
	# We have to figure out if we're linking only static before build_config
	AC_ARG_ENABLE(static-only,
        AC_HELP_STRING([--enable-static-only], [Don't do any dynamic linking]),

	[
	case $enableval in
	no)
		GPT_LINKTYPE="shared"
		;;
	yes)
		GPT_LINKTYPE="static"
dnl		if test "$STATIC_LDFLAGS" = ""; then
dnl		GPT_LDFLAGS=" -all-static $GPT_LDFLAGS"
dnl		else
dnl		GPT_LDFLAGS=" $STATIC_LDFLAGS $GPT_LDFLAGS"
dnl		fi
		;;
	*)
		echo "--enable-static-only has no arguments" >&2
		exit 1
		;;
	esac
	]
	)

	#extract the cumulative build environment from the installed development tree
	[
	if $GPT_LOCATION/sbin/gpt_build_config -src $srcdir/pkgdata/pkg_data_src.gpt.in -f $GLOBUS_FLAVOR_NAME -link $GPT_LINKTYPE; then 
		echo "Dependencies Complete";
	else 
		exit 1;
	fi
	]

# The following variables are used to manage the build enviroment
# GPT_CFLAGS, GPT_INCLUDES, GPT_PGM_LINKS, GPT_LIB_LINKS, and GPT_LDFLAGS
# are the variables used in the Makefile.am's
# GPT_PKG_CFLAGS, GPT_EXTERNAL_INCLUDES and GPT_EXTERNAL_LIBS are stored 
# as build data in the packaging metadata file.
# GPT_CONFIG_FLAGS, GPT_CONFIG_INCLUDES, GPT_CONFIG_PGM_LINKS, and 
# GPT_CONFIG_LIB_LINKS are returned by gpt_build_config and contain build
# environment data from the dependent packages.



	. ./gpt_build_temp.sh  
	rm ./gpt_build_temp.sh
	GPT_CFLAGS="$GPT_CONFIG_CFLAGS"
	GPT_INCLUDES="$GPT_CONFIG_INCLUDES"
	GPT_LIBS="$GPT_CONFIG_PKG_LIBS $GPT_CONFIG_LIBS"
	GPT_LDFLAGS="$GPT_CONFIG_STATIC_LINKLINE $GPT_LDFLAGS"
	GPT_PGM_LINKS="$GPT_CONFIG_PGM_LINKS $GPT_CONFIG_LIBS"
	GPT_LIB_LINKS="-version-info $GPT_MAJOR_VERSION:$GPT_MINOR_VERSION:$GPT_AGE_VERSION $GPT_CONFIG_LIB_LINKS $GPT_CONFIG_LIBS"



	AC_SUBST(GPT_CFLAGS)
	AC_SUBST(GPT_PKG_CFLAGS)
	AC_SUBST(GPT_INCLUDES)
	AC_SUBST(GPT_EXTERNAL_INCLUDES)
	AC_SUBST(GPT_EXTERNAL_LIBS)
	AC_SUBST(GPT_LIBS)
	AC_SUBST(GPT_LDFLAGS)
	AC_SUBST(GPT_CONFIG_CFLAGS)
	AC_SUBST(GPT_CONFIG_INCLUDES)
	AC_SUBST(GPT_CONFIG_LIBS)
	AC_SUBST(GPT_CONFIG_PKG_LIBS)
	AC_SUBST(GPT_PGM_LINKS)
	AC_SUBST(GPT_LIB_LINKS)
	AC_SUBST(GPT_LINKTYPE)
	builddir=`pwd`
	AC_SUBST(builddir)

        # Export pkg-config information about this package
        pkgconfdir='${libdir}/pkgconfig'
        pkgconffile=`echo "${GPT_NAME}.pc" | sed -e 's!_!-!g'`
        pkgconffile_in="pkg_data_src.pc.in"

        AC_SUBST(pkgconfdir)
        AC_SUBST(pkgconffile)
        AC_SUBST(pkgconffile_in)

        AC_CONFIG_FILES(pkgdata/$pkgconffile:pkgdata/pkg_data_src.pc.in)
])

AC_DEFUN([GPT_SET_CFLAGS], [
	GPT_CFLAGS_TMP=$1
	GPT_CFLAGS="$GPT_CFLAGS_TMP $GPT_CFLAGS"
	GPT_PKG_CFLAGS="$GPT_CFLAGS_TMP $GPT_PKG_CFLAGS"
])

AC_DEFUN([GPT_SET_INCLUDES], [

	GPT_INCLUDES_TMP=$1
	GPT_EXTERNAL_INCLUDES="$GPT_EXTERNAL_INCLUDES $GPT_INCLUDES_TMP"
	GPT_INCLUDES="$GPT_INCLUDES_TMP $GPT_INCLUDES"
])

AC_DEFUN([GPT_SET_LIBS], [

	GPT_LIBS_TMP=$1
	GPT_EXTERNAL_LIBS="$GPT_EXTERNAL_LIBS $GPT_LIBS_TMP"
	GPT_LIB_LINKS=" $GPT_LIB_LINKS $GPT_LIBS_TMP"
	GPT_PGM_LINKS=" $GPT_PGM_LINKS $GPT_LIBS_TMP"
])

AC_DEFUN([GPT_SET_LDFLAGS], [
 
         GPT_LDFLAGS_TMP=$1
	 GPT_EXTERNAL_LDFLAGS="$GPT_EXTERNAL_LDFLAGS $GPT_LDFLAGS_TMP"
	 GPT_LDFLAGS=" $GPT_LDFLAGS_TMP $GPT_LDFLAGS"
])
