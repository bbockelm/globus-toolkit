AC_REVISION($Revision$)
AC_INIT(Makefile.am)

GLOBUS_INIT

AM_PROG_LIBTOOL

dnl config header goes here

dnl Initialize the automake rules the last argument
AM_INIT_AUTOMAKE($GPT_NAME, $GPT_VERSION)

GLOBUS_FINALIZE

dnl Don't create a pkg-config file for program-only packages
pkgconffile=""


AC_ARG_WITH(slurm-path,
    AC_HELP_STRING(
            [--with-slurm-path=SLURMPATH],
            [Specify the path of the SLURM installation]),
	    [if test "$withval" != yes; then
	         SLURM_PATH=$withval
		 export PATH=$SLURM_PATH/bin:$PATH
		 export LD_LIBRARY_PATH=$SLURM_PATH/lib:$LD_LIBRARY_PATH
	     else
	         SLURM_PATH=''
	     fi],
	    [SLURM_PATH=no])

DEFAULT_MPI_TYPE=""
MPI_TYPES=""

AC_ARG_WITH(default-mpi,
    AS_HELP_STRING(
        [--with-default-mpi=TYPE]
        [Choose default MPI type, either mpich2 or openmpi]),
        [
            case x"$withval" in
                xmpich2)
                    DEFAULT_MPI_TYPE=mpich2
                    ;;
                xopenmpi)
                    DEFAULT_MPI_TYPE=openmpi
                    ;;
                *)
                    AC_MSG_WARN([Unknown mpi type $withval])
                    ;;
            esac])
            
AC_ARG_WITH(openmpi,
    AS_HELP_STRING(
            [--with-openmpi=OPENMPIPATH],
            [Use OpenMPI located in OPENMPIPATH]),
	    [if test x"$withval" != xno; then
		 USEOPENMPI="yes"
                 if test x"$withval" != xyes; then
                     OPENMPI_PATH=$withval
                     export PATH=$OPENMPI_PATH/bin:$PATH
                 fi
                 if test x"$DEFAULT_MPI_TYPE" = x; then
                     DEFAULT_MPI_TYPE="openmpi"
                 fi
	     fi],
	    [USEOPENMPI="no"])

AC_ARG_WITH(mpich2,
    AC_HELP_STRING(
            [--with-mpich2=MPICH2PATH],
            [Use MPICH2 located in MPICH2PATH]),
	    [if test x"$withval" != xno; then
		 USEMPICH2="yes"
                 if test x"$withval" != xyes; then
                     MPICH2_PATH=$withval
                     export PATH=$MPICH2_PATH/bin:$PATH
                 fi
                 if test x"$DEFAULT_MPI_TYPE" = x; then
                     DEFAULT_MPI_TYPE="mpich2"
                 fi
	     fi],
	    [USEMPICH2="no"])

AC_PATH_PROGS(IBRUN, ibrun, no)
if test "$IBRUN" = "no" ; then
    AC_MSG_WARN([Cannot locate ibrun])
fi

AC_PATH_PROGS(SRUN, srun, no)
if test "$SRUN" = "no" ; then
    AC_MSG_WARN([Cannot locate srun])
fi

AC_PATH_PROGS(SBATCH, sbatch, no)
if test "$SBATCH" = "no"; then
    AC_MSG_WARN([Cannot locate sbatch])
fi

AC_PATH_PROGS(SALLOC, salloc, no)
if test "$SALLOC" = "no"; then
    AC_MSG_WARN([Cannot locate salloc])
fi

AC_PATH_PROGS(SCANCEL, scancel, no)
if test "$SCANCEL" = "no"; then
    AC_MSG_WARN([Cannot locate scancel])
fi

AC_PATH_PROGS(SCONTROL, scontrol, no)
if test "$SCONTROL" = "no"; then
    AC_MSG_WARN([Cannot locate scontrol])
fi

if test x"${DEFAULT_MPI_TYPE}" != x; then
    case $DEFAULT_MPI_TYPE in
        openmpi)
            mpisearchpath="$OPENMPI_PATH:$PATH"
            ;;
        mpich2)
            mpisearchpath="$MPICH2_PATH:$PATH"
            ;;
    esac
else
    mpisearchpath="$PATH"
fi

AC_PATH_PROGS(MPIRUN, mpirun, no, $mpisearchpath)
if test "$MPIRUN" = "no"; then
    AC_MSG_WARN([Cannot locate mpirun])
fi

AC_ARG_WITH(slurm-config,
    AC_HELP_STRING(
            [--with-slurm-config=SLURMCONFIG],
            [Specify the path of the SLURM configuration file]),
	    [if test "$withval" != yes; then
	         SLURM_CONFIG=$withval
	     else
	         SLURM_CONFIG='/etc/slurm/slurm.conf'
	     fi],
	    [SLURM_CONFIG='/etc/slurm/slurm.conf'])

if test "$SRUN" = "no" -o "$SBATCH" = "no" -o "$SALLOC" = "no" -o "$SCANCEL" = "no" -o "$SCONTROL" = "no" ; then
    AC_MSG_WARN([Some SLURM commands were not found.])
else #Try to guess slurm path
    SLURM_PATH=$(dirname $SRUN | sed 's/\/bin//')
fi

dnl LOOK FOR MPI TO USE IF NONE SPECIFIED
if test x"$DEFAULT_MPI_TYPE" = x; then
	if test "$MPIRUN" = "no" ; then 
		AC_MSG_WARN([No MPI found])
	else
		if $MPIRUN --version 2>&1 | grep "Open.*MPI" > /dev/null ; then 
			USEOPENMPI="yes"
			OPENMPI_PATH=$(dirname $MPIRUN | sed 's/\/bin//')
                        DEFAULT_MPI_TYPE="openmpi"
		else
			USEMPICH2="yes"
			MPICH2_PATH=$(dirname $MPIRUN | sed 's/\/bin//')
                        DEFAULT_MPI_TYPE="mpich2"
		fi
	fi
fi

AC_SUBST(SRUN)
AC_SUBST(SBATCH)
AC_SUBST(SALLOC)
AC_SUBST(SCANCEL)
AC_SUBST(SCONTROL)
AC_SUBST(SLURM_PATH)
AC_SUBST(DEFAULT_MPI_TYPE)
AC_SUBST(OPENMPI_PATH)
AC_SUBST(MPICH2_PATH)
if test x$USEOPENMPI = xyes; then
    MPI_TYPES="openmpi"
fi
if test x$USEMPICH2 = xyes; then
    MPI_TYPES="${MPI_TYPES:+$MPI_TYPES }mpich2"
fi

AC_SUBST(MPI_TYPES)
mpi_type_rvf="mpi_type.rvf"
if test "x$MPI_TYPES" != x; then
    cat > "$mpi_type_rvf" <<EOF
Values: $MPI_TYPES
Default: $DEFAULT_MPI_TYPE
DefaultWhen: GLOBUS_GRAM_JOB_SUBMIT"
EOF
else
    rm -f "$mpi_type_rvf" 
    touch "$mpi_type_rvf" 
fi
AC_SUBST_FILE(mpi_type_rvf)

AC_OUTPUT(
	Makefile
        jobmanager-slurm
        globus-slurm.conf
	slurm.rvf
	pkgdata/Makefile
	pkgdata/pkg_data_src.gpt
)
