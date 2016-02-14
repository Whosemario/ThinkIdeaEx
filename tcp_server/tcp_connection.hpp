//
//  TcpConnection.hpp
//  libuvcpp
//
//  Created by whosemario on 16-2-6.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#pragma once

#include <uv.h>
#include <memory>
#include "io_service.hpp"
#include <string>
#include <google/protobuf/service.h>
#include "service_channel.hpp"

namespace lightnet {
    
    void alloc_buffer(uv_handle_t* handle,
                      size_t suggested_size,
                      uv_buf_t* buf);
    
    class TcpConnection {
    public:
        TcpConnection(IoService& io_service)
        : m_client_t(new uv_tcp_t) {
            uv_tcp_init(io_service.get(), m_client_t.get());
        }
        TcpConnection(uv_tcp_t* client)
        :m_client_t(client){}
        
        void connect(const std::string& ip, int port);
        
        uv_tcp_t* GetRawTcp() {
            return m_client_t.get();
        }
        
        void SetChannel(ServiceChannel* channel) {
            this->channel = channel;
        }
        
        void StartToRead();
        
        void WriteToRemove(const char*, int);
        
        static void clientConnectedCallback(TcpConnection* tcpConn, uv_connect_t*, int);
        static void readCallback(TcpConnection* tcpConn, uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);
        
        
    private:
        std::unique_ptr<uv_tcp_t> m_client_t;
        ServiceChannel* channel;
    };
}
