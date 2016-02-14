//
//  tcp_server.cpp
//  libuvcpp
//
//  Created by whosemario on 16-2-6.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#include <iostream>
#include <functional>
#include "tcp_server.hpp"
#include "io_service.hpp"
#include "tcp_connection.hpp"
#include "assert.h"
#include "callback.hpp"
#include "communication_service.hpp"

namespace lightnet {
    TcpServer::TcpServer(IoService& io_service,
                         const std::string& ip,
                         int port)
    : m_tcp_handler(new uv_tcp_t){
        struct sockaddr_in* addr = new struct sockaddr_in;
        uv_tcp_init(io_service.get(), m_tcp_handler.get());
        uv_ip4_addr(ip.c_str(), port, addr);
        uv_tcp_bind(m_tcp_handler.get(), (const struct sockaddr*)addr, 0);
        std::function<void(uv_tcp_t*, int)> cb = std::bind(&TcpServer::ConnectedCallback, this, std::placeholders::_1, std::placeholders::_2);
        m_tcp_handler.get()->data = new Callback<decltype(cb)>(cb);
        int r = uv_listen((uv_stream_t*)(m_tcp_handler.get()), 255, [](uv_stream_t* server, int status) {
            std::cout << "tcp connect" << std::endl;
            Callback<std::function<void(uv_tcp_t*, int)>>* cb = (Callback<std::function<void(uv_tcp_t*, int)>>*)(server->data);
            cb->call((uv_tcp_t*)server, status);
        });
        
        assert(r == 0);
    }
    
    void TcpServer::ConnectedCallback(TcpServer* server, uv_tcp_t* server_handler, int status) {
        std::cout << "call create conn." << status << std::endl;
        uv_tcp_t* client = new uv_tcp_t;
        uv_tcp_init(server_handler->loop, client);
        if(uv_accept((uv_stream_t*)server_handler, (uv_stream_t*)client) == 0) {
            TcpConnection* tcpConnPtr = new TcpConnection(client);
            ServerService* serverService = new ServerService(*tcpConnPtr);
            tcpConnPtr->StartToRead();
        }
    }
    
    void test_tcp_service1() {
        IoService io_service;
        TcpServer tcp_server(io_service, "127.0.0.1", 7000);
        io_service.run();
    }
}
