#! /usr/bin/env perl
#
# Copyright 1999-2010 University of Chicago
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Get version of a job manager

use strict;

my $test_exec = 'version-test';

if ($ENV{CONTACT_STRING} eq "")
{
    die "CONTACT_STRING not set";
}

my $valgrind = "";

if (exists $ENV{VALGRIND})
{
    $valgrind = "valgrind --log-file=VALGRIND-version-test.log";
    if (exists $ENV{VALGRIND_OPTIONS})
    {
        $valgrind .= ' ' . $ENV{VALGRIND_OPTIONS};
    }
}

system("$valgrind $test_exec '$ENV{CONTACT_STRING}'");
