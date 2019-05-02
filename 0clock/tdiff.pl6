my $base = DateTime.new( year => 2019, month => 4, day => 12, hour => 10, minute => 58, second => 30);

my $real = DateTime.new( year => 2019, month => 5, day => 2, hour => 19, minute => 41, second => 32);
my $meas = DateTime.new( year => 2019, month => 5, day => 2, hour => 19, minute => 41, second => 0);

my $loss-per-day = ($real - $meas)/($real - $base) * 86400 ; # secs in a day
say "Loss per day: $loss-per-day";
