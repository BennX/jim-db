/**
############################################################################
# GPL License                                                              #
#                                                                          #
# This file is part of the JIM - DB.                                         #
# Copyright(c) 2015, Benjamin Meyer, <benjamin.meyer@tu-clausthal.de>     #
# This program is free software : you can redistribute it and / or modify     #
# it under the terms of the GNU General Public License as                  #
# published by the Free Software Foundation, either version 3 of the       #
# License, or (at your option) any later version.                          #
#                                                                          #
# This program is distributed in the hope that it will be useful, #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with this program.If not, see <http://www.gnu.org/licenses/>.     #
############################################################################
**/

#include "requesttask.h"
#include "../log/logger.h"
#include "taskqueue.h"
#include "inserttask.h"
#include "findtask.h"
#include "../network/messagefactory.h"

namespace jimdb
{
    namespace tasking
    {
        RequestTask::RequestTask(const std::shared_ptr<network::IClient> client) : Task(client) { }

        void RequestTask::operator()()
        {
            auto l_message = m_client->getData();

            if(l_message == nullptr)
            {
                LOG_WARN << "failed recv after handshake.";
                //no need to answer
                return;
            }

            //check the json
            auto& doc = (*l_message)();
            if(doc.HasParseError())
            {
                LOG_WARN << "Invalid JSON request.";
                m_client->send(network::MessageFactory().error("Invalid JSON request."));
                return;
            }

            if (doc.FindMember("type") == doc.MemberEnd() || doc.FindMember("data") == doc.MemberEnd())
            {
                LOG_WARN << "Invalid JSON request. Missing type or data member";
                m_client->send(network::MessageFactory().error("Invalid JSON request. Missing type or data member"));
                return;
            }

            if (!doc["type"].IsString() || !doc["data"].IsObject())
            {
                LOG_WARN << "Invalid JSON request. Missing type isString or data isObject.";
                m_client->send(network::MessageFactory().error("Invalid JSON request. Missing type isString or data isObject."));
                return;
            }

            if (doc["type"].GetString() == std::string("insert"))
            {
                TaskQueue::getInstance().push_pack(std::make_shared<InsertTask>(m_client, l_message));
                return;
            }

            if (doc["type"].GetString() == std::string("delete"))
            {
                return;
            }

            if (doc["type"].GetString() == std::string("query"))
            {
                return;
            }

            if (doc["type"].GetString() == std::string("find"))
            {
                TaskQueue::getInstance().push_pack(std::make_shared<FindTask>(m_client, l_message));
                return;
            }
        }
    }
}
