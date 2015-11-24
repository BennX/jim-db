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
#include "thread/worker.h"
#include <vector>
#include "network/asioserver.h"

//forward declare
//class ASIOServer;



/** terminate function, for suppress exspecially on Windows the abnormal termination
 * message, that is thrown on uncaught exceptions. The handler need not call the
 * "abort()" function, because this creates the message
 **/
void program_terminate() {
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

    auto& tasks = jimdb::tasking::TaskQueue::getInstance();
    //set up the max number of tasks
    tasks.setMaxSize(cfg[jimdb::common::MAX_TASKS].GetInt());

    std::shared_ptr<jimdb::network::IServer> tcpServer = std::make_shared<jimdb::network::ASIOServer>();
    //std::shared_ptr<jimdb::network::IServer> tcpServer = std::make_shared<jimdb::network::TCPServer>(tasks);
    //tcpServer->start();
    //start the workers
    auto threads = cfg[jimdb::common::THREADS].GetInt();
    //if the config value is 0 take hardware conc.
    if (threads == 0)
    {
        threads = std::thread::hardware_concurrency() - 1; //since the mainthread
    }

    LOG_INFO << "Starting: " << threads + 1 << " Workers";
    std::vector<std::unique_ptr<jimdb::tasking::Worker>> m_workers;
    for (unsigned int i = 0; i < std::thread::hardware_concurrency(); ++i)
    {
        m_workers.push_back(std::make_unique<jimdb::tasking::Worker>(tasks));
    }
    //use this as acceptor and handshaker
    //do nothing else here!
    while (true)
    {
        tcpServer->accept(true); //call accept blocking
    }
}