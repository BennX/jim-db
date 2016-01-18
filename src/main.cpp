/** \mainpage Wellcome to JIM-DB Doku

\section Introduction

JIM-DB stands for JSON-In-Memory database. It is a System to store JSON objects inside
of memory and retrive them by their ID. The DB can handle multiple requests at the same time
depending on the configuration.

<br>
Here is an example configuration for JIM-DB. Some comments with //. They need to be deleted if 
you want to use this configuration.

\code
{
    "log_level": 5, // if lower the DB get less chaty
    "log_file": "default.log", 
    "thread": 0, //how much working threads do you like? 0 = takes the Hardware thread count
	"ip" : "192.168.2.101", //which ip to bind to
    "port": 6060,
    "max_tasks": 16000, //maximum tasks in the queue to prevent from ddos
    "page_header": 4096, //page header size in byte
    "page_body": 16384, //page body size in byte
    "page_fragmentation_clean" : 0.125, // a value to determine when a page need to be cleaned. Cleaning is not implemented yet.
	"page_buckets": [ //Buckets to categorize the used pages. The last bucket need to be the highest.
		256, //if a page has at least 256 Byte free if not it is not in the "search free page" list
		512, //if a page has at least 512 Byte
		1024 //if a page has at least 1024 Byte
	]
}
\endcode

\section License

 #GPL License

 This file is part of the JIM-DB.                                         <br>
 Copyright (c) 2015, Benjamin Meyer, <benjamin.meyer@tu-clausthal.de>     <br>
 This program is free software: you can redistribute it and/or modify     <br>
 it under the terms of the GNU General Public License as                  <br>
 published by the Free Software Foundation, either version 3 of the       <br>
 License, or (at your option) any later version.                          <br>
                                                                          <br>
 This program is distributed in the hope that it will be useful,          <br>
 but WITHOUT ANY WARRANTY; without even the implied warranty of           <br>
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            <br>
 GNU General Public License for more details.                             <br>
                                                                          <br>
 You should have received a copy of the GNU General Public License        <br>
 along with this program. If not, see <http://www.gnu.org/licenses/>.     <br>

\author Benjamin Meyer
\date 08.01.2016 
**/


#include "log/logger.h"
#include <thread>
#include "common/configuration.h"
#include "tasking/taskqueue.h"
#include "common/cmdargs.h"
#include <vector>
#include "network/asioserver.h"
#include "index/pageindex.h"

/** \brief terminate function, for suppress exspecially on Windows the abnormal termination
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

	//init the page index
	jimdb::index::PageIndex::getInstance().init();

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
// increase the thread priority of the maint hread
// so worker do not starve it
#ifdef JIMDB_WINDOWS
    if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST))//set it to 2 which is one above normal
        LOG_WARN << "couldn't increase thread priority.";
#endif

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