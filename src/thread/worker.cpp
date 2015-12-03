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

#include "worker.h"
namespace jimdb
{
    namespace tasking
    {
        Worker::Worker(TaskQueue& t) : m_tasks(t), m_thread(&Worker::doTask, this), m_running(true) {}

        Worker::~Worker()
        {
            m_running = false;
            m_thread.join();
        }

        void Worker::stop()
        {
            m_running = false;
        }

        void Worker::doTask()
        {
            while (m_running)
            {
                auto task = m_tasks.pop_front();
                if(task)
                    (*task)(); //execute the task
				if (task->continuous())
					m_tasks.push_pack(task);
            }
        }
    }
}