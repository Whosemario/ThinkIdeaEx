//
//  tcp_client.cpp
//  libuvcpp
//
//  Created by whosemario on 16-2-10.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#include "tcp_client.hpp"
#include "io_service.hpp"
#include "tcp_connection.hpp"

namespace lightnet {
    TcpClient::TcpClient(IoService& io_service)
    : tcp_connection(io_service){}
    
    void TcpClient::connect(const std::string& ip, int port) {
        tcp_connection.connect(ip, port);
    }
}