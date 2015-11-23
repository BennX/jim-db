# JIM-DB
New:
 - Since it does use ASIO now, it is possible to compile it on different platforms then windows using the right flags. Todo so please create the according buildscript buildenvironment like for example [darwin](https://github.com/BennX/jim-db/blob/master/buildenvironment/darwin.py) (May also send a PR for missing ones)

To build JIM-DB you need:
 - python version < 3.0 
 - scons 2.4.
 - 7zip installed and included to path
 - a C++ 11 Compiler and the according buildenvironment if not defined yet take a look into the defined like the [MSVC](https://github.com/BennX/jim-db/blob/master/buildenvironment/msvc.py) and create a new one for your needs

 
To build JIM-DB call in cmd:
 1. scons library
 2. scons library -c
 3. scons

To use it properly you need a configuration for it. You can generate a configuration by starting JIM-DB with ```-generate configfile```. If you have a configuration start it with the ```-config configuration``` flag.
