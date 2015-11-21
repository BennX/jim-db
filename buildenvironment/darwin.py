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



# Build environment configuration for Mac OS X. The libraries must be stored within
# the global system paths or must be set within the environment variables. The env
# variables are read here and are added to the environment object.
#
# Build is set to C++11 with Clang

Import("*")
import Utilities



conf.env.Replace(CPPDEFINES  = ["JIMDB_DARWIN"])
conf.env.Replace(CPPFLAGS    = ["-std=c++11", "-stdlib=libc++"])
conf.env.Replace(LINKFLAGS   = ["-stdlib=libc++"])

if conf.env["buildtype"] == "release" :
    conf.env.AppendUnique(CPPDEFINES     = ["NDEBUG", "BOOST_UBLAS_NDEBUG", "JIMDB_NDEBUG"])
    conf.env.AppendUnique(CPPFLAGS       = ["-O2", "-finline-functions"])
elif conf.env["buildtype"] == "debug" :
    conf.env.AppendUnique(LINKFLAGS   = ["-g"])
    conf.env.AppendUnique(CPPFLAGS    = ["-g", "-Wall"])



# set library for linking & copying
#libraries = {
#    "lua"            : ["lua"],
#    "boost"          : ["boost_program_options-mt", "boost_filesystem-mt"]
#}

#conf.env.AppendUnique(
#                      LIBS = libraries["lua"] +
#                      libraries["boost"] +
#                      )





# set pathes for compiler & linker
if not conf.env["withlocallibrary"] :

    if conf.env["ENV"].has_key("DYLD_LIBRARY_PATH") :
        conf.env.Replace(LIBPATH = conf.env["ENV"]["DYLD_LIBRARY_PATH"].split(os.pathsep))
        print("Appending custom OSX dynamic library path (DYLD_LIBRARY_PATH)")

    elif conf.env["ENV"].has_key("LD_LIBRARY_PATH") :
        conf.env.Replace(LIBPATH = conf.env["ENV"]["LD_LIBRARY_PATH"].split(os.pathsep))
        print("Appending custom posix dynamic library path (LD_LIBRARY_PATH)")

    elif conf.env["ENV"].has_key("LIBRARY_PATH") :
        conf.env.Replace(LIBPATH = conf.env["ENV"]["LIBRARY_PATH"].split(os.pathsep))
        print("Appending custom posix dynamic library path (LIBRARY_PATH)")

    if conf.env["ENV"].has_key("CPPPATH") :
        conf.env.Replace(CPPPATH = conf.env["ENV"]["CPPPATH"].split(os.pathsep))
        print("Appending custom include path (CPPPATH)")
