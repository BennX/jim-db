#include "findtask.h"
#include "../index/objectindex.h"
#include "../index/pageindex.h"

namespace jimdb
{
    namespace tasking
    {
        FindTask::FindTask(const std::shared_ptr<network::IClient>& client,
                           const std::shared_ptr<network::Message> m) : Task(client), m_msg(m) {}

        void FindTask::operator()()
        {
            LOG_SCOPE_TIME << "Find and create";
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

            //get the meta information of the object
            auto& l_meta = index::ObjectIndex::getInstance()[l_oid];
            //get the page where the object is
            auto l_page = index::PageIndex::getInstance()[l_meta.m_page];
            //get/create the object
            auto l_obj = l_page->getJSONObject(l_meta.m_pos);
			m_client->send(l_obj);
        }
    }
}