#!/usr/bin/env raku
# zcat /usr/share/consolefonts/Unifont-APL8x16-16.0.03.psf.gz | psfgettable - > coverage.txt
#
sub MAIN($filename) {
  $filename.IO
  .lines
  .grep(/0x/)
  .map({ .words[1].split('+')[1] })
  .map({ "0x$_".Int })
  .map({ .chr })
  .map({ .base(16) ~ ':' ~ .chr })
  .join("\n")
  .say
}
