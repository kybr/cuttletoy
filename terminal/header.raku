#!/usr/bin/env raku
# zcat /usr/share/consolefonts/Unifont-APL8x16-16.0.03.psf.gz | psfgettable - > coverage.txt
#
# Files look like this...
#
# Character table extracted from font
#
# 0x000	U+00a9
# 0x001	U+00cb U+0401
# 0x002	U+0404
# 0x003	U+00cf U+0407
# 0x004	U+2666 U+25c8 U+fffd
# 0x005	U+0411
# 0x006	U+0414
# 0x007	U+2022 U+25cf
# 0x008	U+0416
# ...

sub MAIN($filename) {
  my @str = $filename.IO
  .lines
  .grep(/0x/)
  .map({ .words[1] })
  .grep(*.so) # some glyphs do not map to any unicode points
  .map({ .split('+')[1] })
  .map({ "0x$_".Int })
  .Set.keys.sort # only give us the unique codes
  .map({
      given .chr {
        when '\\' { '"\\\"' }
        when '"' { '"\""' }
        default { '"' ~ $_ ~ '"' }
      }
  });

  my $guts = @str.join(",");

  say "const char* glyph[]=\{$guts\};";
}
