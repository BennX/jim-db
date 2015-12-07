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
#include "../common/error.h"
#include "../bench/bench.h"
#include "../bench/benchmark.h"

namespace jimdb
{
    namespace tasking
    {

RequestTask::RequestTask(const std::shared_ptr<asio::ip::tcp::socket>& sock,
                                 const std::shared_ptr<network::Message> msg): ITask(sock), m_msg(msg) {m_bench = new Bench(m_client->getID());}

void RequestTask::operator()()
{
            delete m_bench;//stop timing

            //check the json
            auto& doc = (*m_msg)();
            if(doc.HasParseError())
            {
                LOG_WARN << "Invalid JSON request.";
                // m_client->send(network::MessageFactory().error(
                //error::ErrorCode::nameOf[error::ErrorCode::ErrorCodes::INVALID_JSON_REQUEST]));
                return;
            }

            if (doc.FindMember("type") == doc.MemberEnd() || doc.FindMember("data") == doc.MemberEnd())
            {
                LOG_WARN << "Invalid JSON request. Missing type or data member";
                // m_client->send(network::MessageFactory().error(
                // error::ErrorCode::nameOf[error::ErrorCode::ErrorCodes::MISSING_TYPE_OR_DATA_REQUEST]));
                return;
            }

            if (!doc["type"].IsString() || !doc["data"].IsObject())
            {
                LOG_WARN << "Invalid JSON request. Missing type isString or data isObject.";
                //  m_client->send(network::MessageFactory().error(
                // error::ErrorCode::nameOf[error::ErrorCode::ErrorCodes::TYPE_OR_DATA_WRONG_TYPE_REQUEST]));
                return;
            }

            if (doc["type"].GetString() == std::string("insert"))
            {
                TaskQueue::getInstance().push_pack(std::make_shared<InsertTask>(m_socket, m_msg));
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
                TaskQueue::getInstance().push_pack(std::make_shared<FindTask>(m_socket, m_msg));
                return;
            }

			if(doc["type"].GetString() ==std::string("bench"))
			{
				LOG_DEBUG << Benchmark::getInstance();
			}
        }
    }
}
