#include "deletetask.h"
#include "../index/objectindex.h"
#include "../index/pageindex.h"
#include "../network/messagefactory.h"
#include "../common/error.h"

namespace jimdb
{
    namespace tasking
    {
        DeleteTask::DeleteTask(const std::shared_ptr<jimdb::network::IClient>& client,
                               const std::shared_ptr<jimdb::network::Message> m) : Task(client), m_msg(m)
        {

        }

        void DeleteTask::operator()()
        {
            //we know everything is valid here so just get the oid to delete

            auto& l_data = (*m_msg)()["data"];
            if (l_data.FindMember("oid__") == l_data.MemberEnd())
            {
                LOG_WARN << "invalid delete task. no oid__";
                m_client->send(network::MessageFactory().error(error::ErrorCode::nameOf[error::ErrorCode::MISSING_OID_DELETE]));
                return;
            }

            //check if oid id is int
            if (!l_data["oid__"].IsInt64())
            {
                LOG_WARN << "invalid delete task. oid__ is no int";
                m_client->send(network::MessageFactory().error(error::ErrorCode::nameOf[error::ErrorCode::INVALID_OID_DELETE]));
                return;
            }

            //get the ID we are looking for and check if its valid
            auto l_oid = l_data["oid__"].GetInt64();
            if (!index::ObjectIndex::getInstance().contains(l_oid))
            {
                LOG_WARN << "invalid delete task. oid not found";
                m_client->send(network::MessageFactory().error(error::ErrorCode::nameOf[error::ErrorCode::OID_NOT_FOUND_DELETE]));
                return;
            }

            //get the meta information of the object
            auto& l_meta = index::ObjectIndex::getInstance()[l_oid];
            //get the page where the object is
            auto l_page = index::PageIndex::getInstance()[l_meta.m_page];
            l_page->deleteObj(l_meta.m_pos);

            //inform the client
        }
    }
}