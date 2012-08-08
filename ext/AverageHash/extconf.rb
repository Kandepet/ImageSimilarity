require "mkmf"

$CFLAGS = "-D_GNU_SOURCE -Wall"

have_header("ruby.h")
have_library("stdc++")
create_makefile("AverageHash")
