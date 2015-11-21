# -*- coding: utf-8 -*-

############################################################################
# LGPL License                                                             #
#                                                                          #
# Copyright (c) 2015, Philipp Kraus, <philipp.kraus@tu-clausthal.de>       #
# This program is free software: you can redistribute it and/or modify     #
# it under the terms of the GNU Lesser General Public License as           #
# published by the Free Software Foundation, either version 3 of the       #
# License, or (at your option) any later version.                          #
#                                                                          #
# This program is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU Lesser General Public License for more details.                      #
#                                                                          #
# You should have received a copy of the GNU Lesser General Public License #
# along with this program. If not, see <http://www.gnu.org/licenses/>.     #
############################################################################



# main build script, that sets the script parameters, detects the build toolkit,
# sets the builders / emitters and collect all sub build scripts
# add support for http://parts.tigris.org/doc/PartsUserGuide.pdf

import os, platform, multiprocessing
import SCons.Node.Alias, Utilities


#=== build environment =================================================================================================================

# create the environment
# @param notparallelbuild targets that are not build in parallel job
# @return env environment
def CreateEnvironment( notparallelbuild = [] ) :
    vars = Variables()

    vars.Add(EnumVariable("buildtype", "value of the buildtype", "release", allowed_values=("debug", "release"), ignorecase=True))
    vars.Add(EnumVariable("buildtoolkit", "value for the build toolkit", "default", allowed_values=("default", "mingw", "msvc"), ignorecase=True))

    vars.Add(BoolVariable("withlocallibrary", "use the library in the local directory only", True))
    vars.Add(ListVariable("withoutlibrary", "skipping library builds / downloads", "", ["json", "bstar"]))

    if platform.system().lower() == "windows" :
        vars.Add(EnumVariable("winversion", "value of the Windows version", "win7", allowed_values=("win7", "srv2008", "vista", "srv2003sp1", "xpsp2", "srv2003", "xp", "w2000"), ignorecase=True))

    # parallel build options
    SetOption( "num_jobs", multiprocessing.cpu_count() )
    if any([i in COMMAND_LINE_TARGETS for i in notparallelbuild]) :
        SetOption( "num_jobs", 1 )



    # we set the environment, but the toolkit is set manually
    env = Environment( tools = ["URLDownload", "Unpack", "Repository", "InstallInto"], ENV = os.environ, variables=vars )
    env.VariantDir("build", ".", duplicate=0)
    Help(vars.GenerateHelpText(env))
    if not any([(i in env["ENV"] and env["ENV"][i]) for i in ["CIRCLECI", "JENKINS_HOME"]]) :
        showlicence()

    # we detect the build toolkit, but on Windows we need to set it hard, because MinGW can not be detect automatically
    env["TOOLKIT"], env["TOOLKIT_ARCH"] = Utilities.DetectToolkit()
    if env["buildtoolkit"] == "msvc" :
        env.Tool("msvc")
        env.Tool("mslink")
        env.Tool("msvs")
        env.Tool("mssdk")
        env["TOOLKIT"] = "msvc"

    elif env["buildtoolkit"] == "mingw" :
        env.Tool("mingw")
        env["TOOLKIT"] = "mingw"

    else :
        env.Tool("default")


    # XCode settings to use the buildscript within XCode
    env.SetOption("clean", "clean" in COMMAND_LINE_TARGETS )
    if os.environ.has_key("CONFIGURATION") and os.environ["CONFIGURATION"] :
        env["buildtype"] = os.environ["CONFIGURATION"].lower()


    # set all enum values to lower-case
    env["buildtype"]    = env["buildtype"].lower()
    env["buildtoolkit"] = env["buildtoolkit"].lower()
    if platform.system().lower() == "windows" :
            env["winversion"]   = env["winversion"].lower()


    print "\n\n"
    print "Build environment is ["+env["TOOLKIT"]+"] and architecture ["+env["TOOLKIT_ARCH"]+"]"
    print "Current buildtype: " + env["buildtype"]
    print "Build with [" + str(GetOption("num_jobs")) + "] parallel jobs"
    print "\n"
    return env


# modifying function for the local library path values
# @warn disable temporary builds
# @param env environment object
# @param list with path values
def LocalLibraryPathConfiguration(env, path) :
    includes = []
    libs     = []
    for i in path :
        includes.append( os.path.abspath(os.path.join(i, "include")) )
        libs.append( os.path.abspath(os.path.join(i, "lib")) )

    env.Replace(CPPPATH = includes)
    env.Replace(LIBPATH = libs)




#=== licence ===========================================================================================================================

# licence text
def showlicence() :
    print "############################################################################"
    print "# GPL License                                                              #"
    print "#                                                                          #"
    print "# This file is part of the JIM-DB.                                         #"
    print "# Copyright (c) 2015, Benjamin Meyer, <benjamin.meyer@tu-clausthal.de>     #"
    print "# This program is free software: you can redistribute it and/or modify     #"
    print "# it under the terms of the GNU General Public License as                  #"
    print "# published by the Free Software Foundation, either version 3 of the       #"
    print "# License, or (at your option) any later version.                          #"
    print "#                                                                          #"
    print "# This program is distributed in the hope that it will be useful,          #"
    print "# but WITHOUT ANY WARRANTY; without even the implied warranty of           #"
    print "# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #"
    print "# GNU General Public License for more details.                             #"
    print "#                                                                          #"
    print "# You should have received a copy of the GNU General Public License        #"
    print "# along with this program. If not, see <http://www.gnu.org/licenses/>.     #"
    print "############################################################################"




#=== create environment and compiling ==================================================================================================
# base information with base configuration

# targets which are ignored by default from library checking and parallel build
ignoretargets = ["documentation", "library"]
env = CreateEnvironment( ignoretargets )

# directories within the library build directory which should be checked for a library
librarydirs   = [
                 "rapidjson",
                 "btree",
                 "asio"
]
# directories which are checked for build.py files
builddirs   = [
                 "documentation",
                 "library",
]



#=== build process =====================================================================================================================
if env["withlocallibrary"] :
    print "use local project libraries"
    env = Utilities.Configuration( env, os.path.join("buildenvironment", env["TOOLKIT"]+".py"), ignoretargets, os.path.join("library", "build"), librarydirs, LocalLibraryPathConfiguration )
else :
    print "use system libraries"
    env = Utilities.Configuration( env, os.path.join("buildenvironment", env["TOOLKIT"]+".py"), ignoretargets )

# we create the includes in a fixed order to avoid non-existing errors
for i in builddirs :
    Utilities.IncludeConstruct( env, i, "build.py", "env" )



# === default build =====================================================================================================================
default = env.Program(
    target = os.path.join("#build", env["buildtype"], "jimdb"),
    source = Utilities.GlobRekursiv( os.path.join(os.curdir, "src" ), ".cpp" )
)

env.Default( default )
env.Clean( default, [
    Utilities.GlobRekursiv( os.curdir, [env["OBJSUFFIX"], env["SHOBJSUFFIX"], env["LIBSUFFIX"], ".pyc"] ),
    os.path.join("#", "documentation", "html"),
    os.path.join("#", "documentation", "xml"),
    os.path.join("#", "build"),
    "config.log",
    ".sconf_temp"
])
