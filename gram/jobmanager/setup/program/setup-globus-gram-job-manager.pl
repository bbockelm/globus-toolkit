use Getopt::Long;

my $selected_jm_type;
my @all_jm_types = ('condor', 'easymcs', 'fork', 'glunix', 'grd', 'loadleveler', 'lsf',
                    'nqe', 'nswc', 'pbs', 'pexec', 'prun'); 


GetOptions( 'type=s' => \$selected_jm_type,
            'help'   => \$help)
  or pod2usage(1);

pod2usage(0) if $help;

sub pod2usage {
  my $ex = shift;
  print "setup-globus-gram-job-manager [ \\
               -help \\
               -type=[ @all_jm_types ]\\
                     (fork is default)\\
              ]\n";
  exit $ex;
}

if ( $selected_jm_type eq "" )
{
   $selected_jm_type='fork';
}

if ( ! grep {$_ eq $selected_jm_type} @all_jm_types)
{
   die "Invalid Job Manager Type, valid types are: @all_jm_types";
}

my $gpath = $ENV{GPT_LOCATION};

if (!defined($gpath))
{
  $gpath = $ENV{GLOBUS_LOCATION};
}

if (!defined($gpath))
{
   die "GPT_LOCATION or GLOBUS_LOCATION needs to be set before running this script";
}

@INC = (@INC, "$gpath/lib/perl");

require Grid::GPT::Setup;

my $metadata = new Grid::GPT::Setup(package_name => "globus_gram_job_manager_setup");

my $globusdir = $ENV{GLOBUS_LOCATION};
my $setupdir = "$globusdir/setup/globus/";
my $jm_tmp = "$globusdir/tmp";
my $gk_conf = "$globusdir/etc/globus-gatekeeper.conf";
my $jm_conf = "$globusdir/etc/globus-job-manager.conf";
my $port;
my $subject;
my $junk;

print "Setting up $selected_jm_type job manager\n";
print "------------------------------\n";


if ($selected_jm_type eq "fork")
{
   $rdn="jobmanager";
}
else
{
   $rdn="jobmanager-$selected_jm_type";
}

my $jm_service = "$globusdir/etc/grid-services/$rdn";

print "Creating job manager configuration file...\n";

if ( ! open(CONF, ">$jm_conf") )
{
   die "open failed for $jm_conf";
}

if ( ! -f "$gk_conf" )
{
   die "File not found.  $gk_conf required";
}

print "  - Getting gatekeeper subject\n";
my $host_cert_line = `grep x509_user_cert $gk_conf`;
my $host_cert_file;
chomp($host_cert_line);
($junk, $host_cert_file) = split(/x509_user_cert/, $host_cert_line);

$host_cert_file =~ s/^\s+//; #strip leading whitespace
if ( ! -r "$host_cert_file" )
{
   print STDERR "Warning: Host cert file: $host_cert_file not found.\n";
   print STDERR "         rerun setup-globus-gram-job-manager after installing host cert file.\n";
   $subject="unavailable at time of install";
}
else
{
   $subject=`${globusdir}/bin/grid-cert-info -subject -file ${host_cert_file}`;
   if ( $? != 0 )
   {
      die "Failed getting subject from host certificate: ${host_cert_file}.";
   }
   else
   {
      chomp($subject);
      $subject =~ s/^\s+//; #strip leading whitespace
   }
}

print "  - Getting gatekeeper port\n";
my $port_line = `grep port $gk_conf`;
chomp($port_line);
($junk, $port) = split(/port/, $port_line);
$port =~ s/^\s+//; #strip leading whitespace

#get values from system commands/file
my $hostname = `$globusdir/bin/globus-hostname`;
my $cg_results = `$globusdir/sbin/config.guess`;
($junk, my $uname_cmd) = split(/=/, `grep GLOBUS_SH_UNAME $globusdir/libexec/globus-sh-tools.sh`);

chomp($hostname);
chomp($cg_results);
chomp($uname_cmd);

my $os_name=`$uname_cmd -s`;
my $os_version="";

chomp($os_name);

if($os_name eq "AIX")
{
   $os_version=`$uname_cmd -v`;
   chomp($os_version);
   $os_version.=".";
}

$os_version.=`$uname_cmd -r`;

chomp($os_version);


($cpu, $manuf, $junk) = split(/-/, $cg_results);

print CONF "-home \"$globusdir\"\n";
print CONF "-e $globusdir/libexec\n";
print CONF "-globus-gatekeeper-host $hostname\n";
print CONF "-globus-gatekeeper-port $port\n";
print CONF "-globus-gatekeeper-subject \"$subject\"\n";
print CONF "-globus-host-cputype $cpu\n";
print CONF "-globus-host-manufacturer $manuf\n";
print CONF "-globus-host-osname $os_name\n";
print CONF "-globus-host-osversion $os_version\n";
print CONF "-save-logfile on_errors\n";
print CONF "-job-reporting-dir $jm_dir\n";
print CONF "-machine-type unknown\n";
close(CONF);


print "Done\n";

if ( ! -d "$jm_tmp" )
{
   print "Creating directory for job reporting...\n";
   $result = system("mkdir -p $jm_tmp");
   if ($result != 0)
   {
      die "Failed to create job reporting directory - $jm_tmp";
   }

   $result = system("chmod 777 $jm_tmp");
   if ($result != 0)
   {
      die "Failed to set permissions on $jm_tmp";
   }

   print "Done\n";
}


if ( ! -d "$globusdir/etc/grid-services" )
{
   die "grid-services directory does not exist, cannot create jobmanager service file...";
}

print "Creating grid service jobmanager...\n";
  
if ( ! open(SERVICE, ">$jm_service") )
{
   die "open failed for $jm_service";
}

#service arguments must be on the same line
print SERVICE "stderr_log,local_cred - ".
              "$globusdir/libexec/globus-job-manager ".
              "globus-job-manager ".
              "-conf $globusdir/etc/globus-job-manager.conf ".
              "-type $selected_jm_type -rdn $rdn -machine-type unknown ";

close(SERVICE);
print "Done\n";

$metadata->finish();
