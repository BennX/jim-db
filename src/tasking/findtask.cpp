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

#include "findtask.h"
#include "../index/objectindex.h"
#include "../index/pageindex.h"
#include "../network/messagefactory.h"
#include "polltask.h"
#include "taskqueue.h"
#include "../common/error.h"

namespace jimdb
{
    namespace tasking
    {

        FindTask::FindTask(const std::shared_ptr<network::AsioHandle>& sock,
                           const std::shared_ptr<network::Message>& message): ITask(sock), m_bench(nullptr),
            m_msg(message) {}

        FindTask::FindTask(const std::shared_ptr<network::AsioHandle>& sock, const std::shared_ptr<network::Message>& message,
                           std::shared_ptr<Bench> bench) : ITask(sock),
            m_bench(bench), m_msg(message) {}

        void FindTask::operator()()
        {
            if (m_bench != nullptr)
                m_bench->setType(Benchmark::FIND);

            //optain the oid
            auto& l_data = (*m_msg)()["data"];
            if(l_data.FindMember("oid__") == l_data.MemberEnd())
            {
                //LOG_WARN << "invalid find task. no oid__";
                *m_socket << network::MessageFactory().error(error::ErrorCode::nameOf[error::ErrorCode::MISSING_OID_FIND]);
                return;
            }

            //check if oid id is int
            if(!l_data["oid__"].IsInt64())
            {
                //LOG_WARN << "invalid find task. oid__ is no int";
                *m_socket << network::MessageFactory().error(error::ErrorCode::nameOf[error::ErrorCode::INVALID_OID_FIND]);
                return;
            }

            //get the ID we are looking for and check if its valid
            auto l_oid = l_data["oid__"].GetInt64();
            if(!index::ObjectIndex::getInstance().contains(l_oid))
            {
                //LOG_WARN << "invalid find task. oid not found: " << l_oid;
                *m_socket << network::MessageFactory().error(error::ErrorCode::nameOf[error::ErrorCode::OID_NOT_FOUND_FIND]);
                TaskQueue::getInstance().push_pack(std::make_shared<PollTask>(m_socket, RECEIVE));
                return;
            }

            auto& l_meta = index::ObjectIndex::getInstance()[l_oid];

            //get the page where the object is
            auto l_page = index::PageIndex::getInstance()[l_meta.m_page];

            //get/create the object
            auto l_obj = l_page->getJSONObject(l_meta.m_pos);

            *m_socket << network::MessageFactory().generate(network::RESULT, *l_obj);
            TaskQueue::getInstance().push_pack(std::make_shared<PollTask>(m_socket, RECEIVE));
        }
    }
}