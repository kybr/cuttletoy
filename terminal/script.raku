#!/usr/bin/env raku
my $fontfile =
  @*ARGV[1] // "/usr/share/consolefonts/Unifont-APL8x16-16.0.03.psf.gz";
my $file = qqx{zcat $fontfile | psfgettable -};
#say $file;
my @points = $file
  .lines
  .grep(/0x/)
  .map({
    .words[1].split('+')[1]
  })
  .map({
    "0x$_".Int.chr
  });
my $guts = @points.map({ '"' ~ $_ ~ '"'}).join(",");
my $n = @points.elems;
say "const char* glyph[$n]=\{$guts\};";

use Net::OSC;

my Net::OSC::Server::UDP $server .= new(
  :listening-address<0.0.0.0>
  :listening-port(7771)
  :send-to-address<224.0.7.23>
  :send-to-port(7770)
  :actions(
    action(
      "/hello",
      sub ($msg, $match) {
        if $msg.type-string eq 's' {
          say "Hello { $msg.args[0] }!";
        }
        else {
          say "Hello?";
        }
      }
    ),
  )
);

$server.send: '/print', :args(15, 15, 'world', );

sleep 0.5;

$server.close;

