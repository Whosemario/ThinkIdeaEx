//
//  tcp_server.hpp
//  libuvcpp
//
//  Created by whosemario on 16-2-6.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#pragma once

#include <uv.h>
#include <memory>
#include <string>
#include <functional>

namespace lightnet {
    class IoService;
    class TcpConnection;
    class TcpServer {
    public:
        TcpServer(IoService& io_service,
                  const std::string& ip,
                  int port);
        static void ConnectedCallback(TcpServer*, uv_tcp_t*, int);
    private:
        std::unique_ptr<uv_tcp_t> m_tcp_handler;
    };
    
    void test_tcp_service1();
}
