#!/usr/bin/zsh
(perl -e '
for (1..500) {
  $i=$_/10;
  print "0:", sin($i), "\n1:", cos($i), "\n";
  if ($_%2) {
    print "2:", log(2*$_), "\n"
  }
  system "sleep 0.1"
}' ; read) | \
perl ./driveGnuPlotStreams.pl 3 2 50 50 -1 1 -2 7 500x300+0+0 500x300+500+0 'sin' 'cos' 'log' 0 0 1
