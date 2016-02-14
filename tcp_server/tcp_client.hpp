//
//  tcp_client.hpp
//  libuvcpp
//
//  Created by whosemario on 16-2-10.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#pragma once

#include <string>
#include "tcp_connection.hpp"

namespace lightnet {
    class IoService;
    class TcpClient {
    public:
        TcpClient(IoService&);
        void connect(const std::string& ip, int port);
    private:
        TcpConnection tcp_connection;
    };
}