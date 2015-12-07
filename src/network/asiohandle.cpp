#include "asiohandle.h"
namespace jimdb
{
    namespace network
    {
        uint64_t AsioHandle::s_counter = 0;
        AsioHandle::AsioHandle(asio::io_service& io_service) : basic_stream_socket<asio::ip::tcp>(io_service),
            m_id(s_counter++) {}

        AsioHandle::AsioHandle(asio::io_service& io_service,
                               const protocol_type& protocol): basic_stream_socket<asio::ip::tcp>(io_service, protocol), m_id(s_counter++) {}

        AsioHandle::AsioHandle(asio::io_service& io_service,
                               const endpoint_type& endpoint): basic_stream_socket<asio::ip::tcp>(io_service, endpoint), m_id(s_counter++) {}

        AsioHandle::AsioHandle(asio::io_service& io_service, const protocol_type& protocol,
                               const native_handle_type& native_socket): basic_stream_socket<asio::ip::tcp>(io_service, protocol, native_socket),
            m_id(s_counter++) {}

        void AsioHandle::operator<<(std::shared_ptr<std::string> s)
        {
            char length[8 + 1];
            sprintf(length, "%8d", static_cast<int>(s->size()));
            auto l_message = std::string(length);
            l_message.append(*s);

            asio::async_write(*this, asio::buffer(l_message.c_str(), l_message.size()), [&](std::error_code ec,
            size_t bytes_read) {});
        }

        uint64_t AsioHandle::ID() const
        {
            return m_id;
        }
    }
}