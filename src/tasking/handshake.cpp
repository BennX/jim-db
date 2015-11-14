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
            if (!m_client->hasData())
            {
                LOG_WARN << "handshake Failed";
                m_client->close(); //close the soc
                return; //return on failur
            }
            auto l_message = m_client->getData();

            auto& l_doc = (*l_message)();

            if (l_doc.GetParseError() != rapidjson::kParseErrorNone)
            {
                LOG_WARN << "handshake Failed";
                return;
            }

            //check if handshaje is valid
            if (std::string("hi") == l_doc["data"].GetString())
                ;
            else
            {
                LOG_WARN << "handshake Failed";
                m_client->close(); //close the soc
                return; //return on failur
            }
            //if handshake is valid do something
            LOG_DEBUG << "handshake Successfull";
            TaskQueue::getInstance().push_pack(std::make_shared<RequestTask>(m_client));
        }
    }
}