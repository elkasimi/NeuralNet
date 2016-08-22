#! /bin/sh
#astyle --style=java --convert-tabs *.cc *.h
clang-format -i -style=file *.h *.cpp
rm -f *.orig

