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
#include "handshake.h"
#include "../log/logger.h"
#include "taskqueue.h"
#include "requesttask.h"
#include "../network/messagefactory.h"
#include "../common/error.h"
#include "polltask.h"
class RequestTask;
namespace jimdb
{
    namespace tasking
    {
        HandshakeTask::HandshakeTask(const std::shared_ptr<network::AsioHandle>& sock,
                                     const std::shared_ptr<network::Message> msg): ITask(sock), m_msg(msg) {}

        HandshakeTask::~HandshakeTask() {}

        void HandshakeTask::operator()()
        {
            try
            {
                auto& l_doc = (*m_msg)();
            }
            catch (std::runtime_error& e)
            {
                std::string  error = "parsing error Handshake: ";
                error += e.what();
                LOG_ERROR << error;
                //m_client->send(network::MessageFactory().error(
                //error::ErrorCode::nameOf[error::ErrorCode::ErrorCodes::PARSEERROR_HANDSHAKE]));
                return;
            }

            auto& l_doc = (*m_msg)();
            if (l_doc.GetParseError() != rapidjson::kParseErrorNone)
            {
                LOG_ERROR << "Handshake parsing error.";
                //m_client->send(network::MessageFactory().error(
                //error::ErrorCode::nameOf[error::ErrorCode::ErrorCodes::PARSEERROR_HANDSHAKE]));
                return;
            }

            //check if handshaje is valid
            if(l_doc.FindMember("data") == l_doc.MemberEnd())
            {
                return;
            }
            if (l_doc["data"].FindMember("handshake") == l_doc["data"].MemberEnd())
            {
                return;
            }


            if (std::string("hi") != l_doc["data"]["handshake"].GetString())
            {
                LOG_WARN << "handshake Failed";
                //not needed anymore, socket get closed automatically
                //m_client->close(); //close the soc
                return; //return on failur
            }
            //if handshake is valid do something
//            TaskQueue::getInstance().push_pack(std::make_shared<RequestTask>(m_client));

            //after fully request
            LOG_DEBUG << "Handshake Successfull!";
            TaskQueue::getInstance().push_pack(std::make_shared<PollTask>(m_socket, PollType::RECEIVE));
        }
    }
}