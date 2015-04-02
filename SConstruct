import sys
import os

INCLUDES = [
    '.',
    ]

env = Environment(
    ENV = os.environ,
    CC='g++',
    DEBUG= '-ggdb',
    OPTIM= '-O0',
    CPPPATH=INCLUDES,
    CCFLAGS = "-Wall $DEBUG -Wcast-align $OPTIM ",
    LIBS = [],
    LINKCOM = '$LINK -o $TARGET $LINKFLAGS $SOURCES $_LIBDIRFLAGS -Xlinker --start-group $_LIBFLAGS -Xlinker --end-group'
    )

env.ParseConfig("Wand-config --cflags --cppflags --ldflags --libs")

Export('env')

env.SConscript('Image2Code/SConscript')


