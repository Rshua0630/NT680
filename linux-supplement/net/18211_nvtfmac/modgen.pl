#!/usr/bin/perl -w

use strict;
use warnings;
#use Time::localtime;
#use File::stat;

our $progrma_ver = "0.1";
our $output;


if (!defined($ARGV[0])) {
        &show_usage;
        exit;
} else {
        if ($ARGV[0] eq "help") {
                &show_usage;
                exit;
        } elsif ($ARGV[0] eq "ver") {
                print "\nversion:".$progrma_ver."\n";
                exit;
        } elsif ($ARGV[0] eq "ins") {

                # Remove old module first
                print "INFO: Removing original module...";
                $output = qx(make uninstall 2>&1);
                print "DONE\n";

                # Build module
                print "INFO: Building module...";
                $output = qx(make clean 2>&1);
                if ($output =~ /\[.*\] Error/) {
                        print "\n".$output;
                        print "\nERROR: Make clean error\n";
                        exit;
                }

                $output = qx(make 2>&1);
                if ($output =~ /\[.*\] Error/) {
                        print "\n".$output;
                        print "\nERROR: Make error\n";
                        exit;
                } else {
                        print "DONE\n";
                }

                print "INFO: Installing the module to system...";
                $output = qx(sudo make install 2>&1);
                if ($output =~ /\[.*\] Error/) {
                        print "\n".$output;
                        print "\nERROR: Make install error\n";
                        exit;
                } else {
                        print "\n".$output;
                        print "DONE\n"
                }
        } elsif ($ARGV[0] eq "unins") {
                print "INFO: Uninstalling the module to system...";
                $output = qx(sudo make uninstall 2>&1);
                if ($output =~ /\[.*\] Error/) {
                        print "\n".$output;
                        print "\nERROR: Make uninstall error\n";
                        exit;
                } else {
                        print "DONE\n"
                }

        } else {
                print "ERROR: <type> error!!\n";
                &show_usage;
                exit;
        }
}



sub show_usage
{
        print "usage: $0 <type>\n";
        print "[type]:\n";
        print "\tins\tbuild and install module to system\n";
        print "\tunins\tuninstall the module from system\n";
        print "\thelp\tfor help\n";
        print "\tver\tshow program version\n";
}

