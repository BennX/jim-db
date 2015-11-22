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
            LOG_DEBUG << "handshake Successfull";
            TaskQueue::getInstance().push_pack(std::make_shared<RequestTask>(m_client));
        }
    }
}