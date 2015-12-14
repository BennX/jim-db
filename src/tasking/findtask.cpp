#include "findtask.h"
#include "../index/objectindex.h"
#include "../index/pageindex.h"
#include "../network/messagefactory.h"
#include "polltask.h"
#include "taskqueue.h"

namespace jimdb
{
    namespace tasking
    {

        FindTask::FindTask(const std::shared_ptr<network::AsioHandle>& sock,
                           const std::shared_ptr<network::Message>& message): ITask(sock), m_msg(message) {}


        void FindTask::operator()()
        {
            //optain the oid
            auto& l_data = (*m_msg)()["data"];
            if(l_data.FindMember("oid__") == l_data.MemberEnd())
            {
                LOG_WARN << "invalid find task. no oid__";
                return;
            }

            //check if oid id is int
            if(!l_data["oid__"].IsInt64())
            {
                LOG_WARN << "invalid find task. oid__ is no int";
                return;
            }

            //get the ID we are looking for and check if its valid
            auto l_oid = l_data["oid__"].GetInt64();
            if(!index::ObjectIndex::getInstance().contains(l_oid))
            {
                LOG_WARN << "invalid find task. oid not found";
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