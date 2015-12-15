/**
############################################################################
# GPL License                                                              #
#                                                                          #
# This file is part of the JIM-DB.                                         #
# Copyright (c) 2015, Benjamin Meyer, <benjamin.meyer@tu-clausthal.de>     #
# This program is free software: you can redistribute it and/or modify     #
# it under the terms of the GNU General Public License as                  #
# published by the Free Software Foundation, either version 3 of the       #
# License, or (at your option) any later version.                          #
#                                                                          #
# This program is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with this program. If not, see <http://www.gnu.org/licenses/>.     #
############################################################################
**/

/** \mainpage Wellcome to JIM-DB

\section Introduction

JIM-DB stands for JSON-In-Memory database. It is a System to store objects inside
of memory and allow to querry them.
\author Benjamin Meyer
\date DATE
*/
#include "log/logger.h"
#include <thread>
#include "common/configuration.h"
#include "tasking/taskqueue.h"
#include "common/cmdargs.h"
#include <vector>
#include "network/asioserver.h"
#include "bench/benchmark.h"


/** terminate function, for suppress exspecially on Windows the abnormal termination
 * message, that is thrown on uncaught exceptions. The handler need not call the
 * "abort()" function, because this creates the message
 **/
void program_terminate()
{
    std::cerr << "error detected which is not handled by the main program" << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    std::set_terminate(program_terminate);

    //logger can be at init using the startup log
    auto& args = jimdb::common::CmdArgs::getInstance();
    args.init(argc, argv);
    LOG_DEBUG << args;

    //set the flag to benchmark find instead of insert
    if(args.contains("-bench"))
    {
        if(args["-bench"] == "find")
            Benchmark::getInstance().setType(Benchmark::FIND);
    }

    if (args.contains("-h"))
    {
        LOG_INFO << "todo print some help if help is needed!";
        return EXIT_SUCCESS;
    }

    if (args.contains("-generate"))
    {
        std::ofstream file;

        try
        {
            //if second param is missing catch it.
            file.open(args["-generate"]);
        }
        catch (const std::runtime_error& e)
        {
            LOG_EXCAPT << e.what();
            return EXIT_FAILURE;
        }

        if (!file || !file.is_open())
        {
            LOG_ERROR << "couldn't generate config";
            return EXIT_FAILURE;
        }

        file << jimdb::common::Configuration::getInstance().generate();
        file.flush();
        LOG_INFO << "generated example configuration";
        return EXIT_SUCCESS;
    }

    if (!args.contains("-config"))
    {
        LOG_ERROR << "missing argument -config CONFIGFILE";
        return EXIT_FAILURE;
    }

    //get the config instance
    auto& cfg = jimdb::common::Configuration::getInstance();

    try
    {
        cfg.init(args["-config"]);
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << e.what();
        return EXIT_FAILURE;
    }

    //set the loglevel of the config or the default log level
    jimdb::common::Logger::getInstance().setLogLevel(cfg[jimdb::common::LOG_LEVEL].GetInt());
    //set the "real logfile" before this we used a "default to log excaptions"
    jimdb::common::Logger::setLogFile(cfg[jimdb::common::LOG_FILE].GetString());

    LOG_INFO << cfg; //print out the config
    //after this the logger can be used as regular!

    auto& tasks = jimdb::tasking::TaskQueue::getInstance();
    //set up the max number of tasks
    tasks.setMaxSize(cfg[jimdb::common::MAX_TASKS].GetInt());

    //start the workers
    auto threads = cfg[jimdb::common::THREADS].GetInt();
    //if the config value is 0 take hardware conc.
    if (threads == 0)
    {
        threads = std::thread::hardware_concurrency() - 1; //since the mainthread
    }

    LOG_INFO << "Starting: " << threads << " Workers";

    //spawn the working threads
    std::vector<std::thread> l_workers;
    for (auto i = 0; i < threads; ++i)
    {
        l_workers.push_back(std::thread([]()
        {
            auto& l_task = jimdb::tasking::TaskQueue::getInstance();
            while (true)
            {
                auto task = l_task.pop_front();
                if (task)
                    (*task)(); //execute the task
                if (task->continuous())
                    l_task.push_pack(task);
            }
        }));
    }

    jimdb::network::ASIOServer l_server;

    l_server.accept(false);
    //go into the asio service loop
    l_server.start();


    //kill the workers
    std::for_each(l_workers.begin(), l_workers.end(), [](std::thread & t)
    {
        t.join();
    });

    LOG_ERROR << "This is bad we lost the io service and lost our loop.";
}