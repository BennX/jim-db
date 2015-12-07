#include "polltask.h"
#include "../log/logger.h"
#include "taskqueue.h"
#include "requesttask.h"
#include "handshake.h"

namespace jimdb
{
    namespace tasking
    {
#define MESSAGE_SIZE 8

        PollTask::~PollTask() {}

        bool PollTask::continuous()
        {
            return m_active;
        }

        PollTask::PollTask(std::shared_ptr<network::AsioHandle> sock, const PollType& p,
                           const int& timeout) : ITask(sock),
            m_timeout(timeout), m_buffer(new char[MESSAGE_SIZE + 1]), m_active(true), m_type(p)
        {
            m_last = std::chrono::high_resolution_clock::now();
            m_buffer[MESSAGE_SIZE] = '\0';
            m_socket->async_read_some(asio::buffer(m_buffer, 8),
                                      [&](asio::error_code ec, size_t bytes_read)
            {
                if (ec)
                {
                    // cancle or close while in a operation of the socket..
					// nice try watson!
                    //m_socket->cancel();
                    //m_socket->shutdown(asio::ip::tcp::socket::shutdown_both);
                    //m_socket->close();

                    LOG_DEBUG << ec.message();
                    m_active = false;
                    return;
                }

                //reset timer
                m_last = std::chrono::high_resolution_clock::now();

                //else we got some data!!
                int l_size = 0;
                std::stringstream ss;
                ss << m_buffer;
                ss >> l_size;

                delete[] m_buffer;

                //create local buffer for a message
                auto l_buffer = new char[l_size + 1];
                l_buffer[l_size] = '\0';
                try
                {
                    m_socket->read_some(asio::buffer(l_buffer, l_size));
                }
                catch (std::runtime_error& e)
                {
                    LOG_DEBUG << e.what();
                }

                //LOG_DEBUG << l_buffer;

                switch (m_type)
                {
                    case HANDSHAKE:
                        TaskQueue::getInstance().push_pack(std::make_shared<HandshakeTask>(m_socket,
                                                           std::make_shared<network::Message>(l_buffer)));
                        break;
                    case RECEIVE:
                        TaskQueue::getInstance().push_pack(std::make_shared<RequestTask>(m_socket,
                                                           std::make_shared<network::Message>(l_buffer)));
                        break;
                    default:
                        break;
                }
                m_active = false;
            });
        }

        void PollTask::operator()()
        {
            if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_last).count() >
                    m_timeout)
            {
                LOG_INFO << "client timedout";
                m_active = false;
            }
        }
    }
}