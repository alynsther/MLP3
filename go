#! /bin/csh -f
# csh script for running GENESIS
# modified by CBC to require both arguments and to show usage
if (x$1 == x) then
  echo "usage: go [function] [file ext], e.g., go f1 ex1"
  echo "  this will use files f1.c, in.ex1, and template.ex1"
else if (x$2 == x) then
  echo "usage: go [function] [file ext], e.g., go f1 ex1"
  echo "  this will use files f1.c, in.ex1, and template.ex1"
else
  echo "    Note: Genesis files modified for use at Bowdoin"
  echo "    Note2: ga.$1 is your executable (e.g., if you need to use the debugger)"
  echo "    making executables ..."
  make f=$1 ga.$1
  make report
  echo "    running ga.$1 $2 ..."
  ga.$1 $2
  echo "    writing report ..."
  echo rep.$2 for ga.$1 > rep.$2
  date >> rep.$2
  echo ' ' >> rep.$2
  report $2 >> rep.$2
  echo "    done."
endif
