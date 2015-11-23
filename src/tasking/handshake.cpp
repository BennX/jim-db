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

class RequestTask;
namespace jimdb
{
    namespace tasking
    {
        HandshakeTask::HandshakeTask(const std::shared_ptr<network::IClient> client) : Task(client) { }

        void HandshakeTask::operator()()
        {
            //sending a handshake HI and wait 1s to return a hi as shake
            m_client->send(network::MessageFactory().generate(network::HANDSHAKE));
            std::shared_ptr<network::Message> l_message = m_client->getData();
            if (l_message == nullptr)
                return;

            try
            {
                auto& l_doc = (*l_message)();
            }
            catch (std::runtime_error& e)
            {
                LOG_ERROR << "Parsing throwed: " << e.what();
                return;
            }

            auto& l_doc = (*l_message)();
            if (l_doc.GetParseError() != rapidjson::kParseErrorNone)
            {
                return;
            }

            //check if handshaje is valid
            if (std::string("hi") != l_doc["data"].GetString())
            {
                LOG_WARN << "handshake Failed";
                //not needed anymore, socket get closed automatically
                //m_client->close(); //close the soc
                return; //return on failur
            }
            //if handshake is valid do something
            //LOG_DEBUG << "handshake Successfull";
            TaskQueue::getInstance().push_pack(std::make_shared<RequestTask>(m_client));
        }
    }
}