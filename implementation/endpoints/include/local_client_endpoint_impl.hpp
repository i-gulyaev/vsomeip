// Copyright (C) 2014-2016 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef VSOMEIP_LOCAL_CLIENT_ENDPOINT_IMPL_HPP
#define VSOMEIP_LOCAL_CLIENT_ENDPOINT_IMPL_HPP

#include <boost/asio/io_service.hpp>
#include <boost/asio/local/stream_protocol.hpp>

#ifdef WIN32
#include <boost/asio/ip/tcp.hpp>
#endif

#include <vsomeip/defines.hpp>

#include "client_endpoint_impl.hpp"

namespace vsomeip {

#ifdef WIN32
typedef client_endpoint_impl<
            boost::asio::ip::tcp
        > local_client_endpoint_base_impl;
#else
typedef client_endpoint_impl<
            boost::asio::local::stream_protocol
        > local_client_endpoint_base_impl;
#endif

class local_client_endpoint_impl: public local_client_endpoint_base_impl {
public:
    typedef std::function<void()> error_handler_t;

    local_client_endpoint_impl(std::shared_ptr<endpoint_host> _host,
                               endpoint_type _remote,
                               boost::asio::io_service &_io,
                               std::uint32_t _max_message_size);

    virtual ~local_client_endpoint_impl();

    void start();

    bool is_local() const;

    bool get_remote_address(boost::asio::ip::address &_address) const;
    unsigned short get_remote_port() const;

    void register_error_handler(error_handler_t _error_handler);

private:
    void send_queued();

    void send_magic_cookie();

    void connect();
    void receive();
    void receive_cbk(boost::system::error_code const &_error,
                     std::size_t _bytes);

    message_buffer_t recv_buffer_;

    error_handler_t error_handler_;
};

} // namespace vsomeip

#endif // VSOMEIP_LOCAL_CLIENT_ENDPOINT_IMPL_HPP
