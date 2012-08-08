require "mkmf"

#$CFLAGS = "-D_GNU_SOURCE -Wall -std=c++0x"
#$CFLAGS = "-pthread -fno-strict-aliasing -DNDEBUG -g -fwrapv -O2 -Wall -Wstrict-prototypes -fPIC -DImMagick -DLinuxBuild -g -g -O2 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -pthread -I/usr/include/ImageMagick -lMagick++ -lMagickWand -lMagickCore"
$CFLAGS = "-pthread -fno-strict-aliasing -DNDEBUG -g -fwrapv -O2 -Wall -fPIC -DImMagick -DLinuxBuild -g -g -O2 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -pthread -I/usr/include/ImageMagick -lMagick++ -lMagickWand -lMagickCore"

have_header("ruby.h")
have_library("stdc++")
have_library("Magick++")
have_library("MagickWand")
have_library("MagickCore")
create_makefile("imgseek")
