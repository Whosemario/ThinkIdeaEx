//
//  tcp_connection.cpp
//  libuvcpp
//
//  Created by whosemario on 16-2-6.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#include <functional>
#include "tcp_connection.hpp"
#include "callback.hpp"
#include "service_channel.hpp"

#include "communication_service.hpp"

namespace lightnet {
    
    void alloc_buffer(uv_handle_t* handle,
                      size_t suggested_size,
                      uv_buf_t* buf) {
        buf->base = (char*)malloc(suggested_size);
        buf->len = suggested_size;
    }
    
    void TcpConnection::connect(const std::string &ip, int port) {
        
        uv_connect_t* connect_handler = new uv_connect_t;
        struct sockaddr_in dest;
        uv_ip4_addr(ip.c_str(), port, &dest);
        std::function<void(uv_connect_t*, int)> cb = std::bind(&TcpConnection::clientConnectedCallback,
                                                               this,
                                                               std::placeholders::_1,
                                                               std::placeholders::_2);
        connect_handler->data = new Callback<decltype(cb)>(cb);
        uv_tcp_connect(connect_handler, m_client_t.get(), (const struct sockaddr *)&dest, [](uv_connect_t* req, int status) {
            Callback<std::function<void(uv_connect_t*, int)>>* cb = (Callback<std::function<void(uv_connect_t*, int)>>*)(req->data);
            cb->call(req, status);
        });
        
    }
    
    void TcpConnection::WriteToRemove(const char* content, int size) {
        if(size < 0)
            size = strlen(content);
        uv_write_t* w_req = new uv_write_t;
        char* tContent = (char*)malloc(size);
        memcpy(tContent, content, size);
        uv_buf_t buf = uv_buf_init(tContent, size);
        uv_write(w_req, (uv_stream_t*)m_client_t.get(), &buf, 1, [](uv_write_t* req, int status){
            delete req;
        });
    }
    
    void TcpConnection::StartToRead() {
        std::function<void(uv_stream_t*, ssize_t, const uv_buf_t*)> cb = std::bind(&TcpConnection::readCallback,
                                                                                   this,
                                                                                   std::placeholders::_1,
                                                                                   std::placeholders::_2,
                                                                                   std::placeholders::_3);
        m_client_t.get()->data = new Callback<decltype(cb)>(cb);
        uv_read_start((uv_stream_t*)m_client_t.get(), alloc_buffer, [](uv_stream_t* stream,
                                                                       ssize_t nread,
                                                                       const uv_buf_t* buf){
            Callback<std::function<void(uv_stream_t*, ssize_t, const uv_buf_t*)>>* cb =
            (Callback<std::function<void(uv_stream_t*, ssize_t, const uv_buf_t*)>>*)stream->data;
            cb->call(stream, nread, buf);
        });
    }
    
    void TcpConnection::clientConnectedCallback(TcpConnection* tcpConn, uv_connect_t* req, int status) {
        if(status == 0) {
            ClientService* client_service = new ClientService(*tcpConn);
            client_service->SendMessage("hello, i connected.");
            tcpConn->StartToRead();
        }
    }
    
    void TcpConnection::readCallback(TcpConnection* tcpConn, uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
        tcpConn->channel->ProcessInputData(buf->base, nread);
    }

}