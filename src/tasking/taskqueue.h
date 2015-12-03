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

#pragma once
#include <memory>
#include <deque>
#include "itask.h"
#include <mutex>
#include "../log/logger.h"
namespace jimdb
{
    namespace tasking
    {
        /**
        \brief Thread Safe Queue for Tasks

        \author Benjamin Meyer
        \date 15.09.2015 14:25
        */
        class TaskQueue
        {
        public:
            static TaskQueue& getInstance();

            /**
            set the max number of Tasks in the queue. If set to 0
            it is "unlimited".
            @param[in] size max size
            @author Benjamin Meyer
            @date 07.10.2015 10:39
            */
            void setMaxSize(const size_t& size);

            /**\brief Add a new task
            *
            * Pushs a new task to the task queues back. If the list is full
            * the task isn't added to the queue. It rejects.
            *@param[in] std::unique_ptr<Task> t the task to add
            *@return true if added false if list is full to prevent from ddos
            *@author Benjamin Meyer
            *@date Mittwoch, 22. Juli 2015
            */
            inline bool push_pack(std::shared_ptr<ITask> t);

            /**\brief returns a task without copying it
            *
            * get the next task of the queues front
            *@return unique_ptr<Task> from the front
            *@author Benjamin Meyer
            *@date Mittwoch, 22. Juli 2015
            */

            inline std::shared_ptr<ITask> pop_front();
            /**\brief Returns the current size of the taskqueue
            *
            * If its 0 do not execute a task of it!
            *@author Benjamin Meyer
            *@date Mittwoch, 22. Juli 2015
            */
            size_t size();

        private:
            TaskQueue();
            ~TaskQueue();

            static TaskQueue m_instance;
            std::deque<std::shared_ptr<ITask>> m_tasks;
            std::condition_variable m_cond;
            std::mutex m_mutex;
            size_t m_maxSize;
        };
#include "taskqueue.hpp"
    }
}