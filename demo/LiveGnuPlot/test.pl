#!/usr/bin/perl -w
for (1..100) 
{
  $i=$_/10;
  print "0:", sin($i), "\n1:", cos($i), "\n";
  system "sleep 0.1"
}

