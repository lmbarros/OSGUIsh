#
# SConstruct
# OSGUIsh
# By Leandro Motta Barros
#


# ------------------------------------------------------------------------------
#  The "base" building environment
#  This is used as a template to create the more specific building environments
#  that are actually used to build something.
# ------------------------------------------------------------------------------

# Inherit the environment from, well, from the environment.
# TODO: I suppose that's not the best way to create a 'SConstruct'. But for a
#       rootless GoboLinux user (with installed libraries in his own $HOME),
#       this simplifies the things quite a lot.
import os
envBase = Environment (ENV = os.environ,
                       CPPPATH = "#/include",
                       LIBPATH = "#/lib")

# Add some flags manually...
if envBase["CXX"] == "g++":
    envBase["CXXFLAGS"] += " -Wall"

    buildMode = ARGUMENTS.get ("mode", "no-opt")

    if buildMode == "opt":
        envBase["CXXFLAGS"] += " -O3"
    elif buildMode == "debug":
        envBase["CXXFLAGS"] += " -g"
        envBase["LINKFLAGS"] += " -g"
    elif buildMode == "profile":
        envBase["CXXFLAGS"] += " -g -pg"
        envBase["LINKFLAGS"] += " -g -pg"

    prefix = ARGUMENTS.get ("prefix", "/usr")

# A friendly help message...
envBase.Help("""
OSGUIsh build system

Just type 'scons' to build everything. To install, you can use
'scons install'.  The following options are available:

   mode=<MODE>
   Selects the compilation mode. <MODE> must be one the following:
      o 'profile', for compiling with profiling ('gprof') support
      o 'debug', for compiling with debug support
      o 'no-opt' (the default), for compiling without optimizations or
        debug support or anything else
      o 'opt', for compiling with optimizations enabled

   prefix=<DIR>
   Selects the prefix directory for installation. Default is '/usr'.
""")


# ------------------------------------------------------------------------------
#  The library building environment
#  This is one is used to build the OSGUIsh library. Currently, it is no
#  different than the base building environment.
# ------------------------------------------------------------------------------
envLib = envBase.Copy()


# # ------------------------------------------------------------------------------
# #  The tests building environment
# #  This one is used to build the OSGUIsh unit tests.
# # ------------------------------------------------------------------------------
# envTests = envBase.Copy (LIBS = [ "OSGUIsh", "lua", "dl",
#                                   "boost_unit_test_framework-gcc-mt" ])

# ------------------------------------------------------------------------------
#  The demos building environment
#  This one is used to build the OSGUIsh demos.
# ------------------------------------------------------------------------------
envDemos = envBase.Copy (LIBS = [ "OSGUIsh", "osg", "osgProducer",
                                  "boost_signals-gcc-mt" ])



# ------------------------------------------------------------------------------
#  The build targets
#  The things that are actually built.
# ------------------------------------------------------------------------------
theStaticLib = envLib.Library ("lib/OSGUIsh", [ "Sources/EventHandler.cpp" ])

envDemos.Program ("Demos/Demo", "Demos/Demo.cpp")


# --------------------------------------------------------------------
#  The 'install' target
# --------------------------------------------------------------------
import glob
headerFiles = glob.glob ("include/OSGUIsh/*.hpp")
shareFiles = [ "AUTHORS.txt" ]

envBase.Alias ("install", envBase.Install (os.path.join (prefix, "lib"), theStaticLib))
envBase.Alias ("install", envBase.Install (os.path.join (prefix, "include", "OSGUIsh"), headerFiles))
envBase.Alias ("install", envBase.Install (os.path.join (prefix, "share", "OSGUIsh"), shareFiles))
