//
//  main.cpp
//  libuvcpp
//
//  Created by whosemario on 16-1-31.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include "proto/message_communication.pb.h"
#include "io_service.hpp"
#include "tcp_client.hpp"
#include "callback.hpp"
#include "tcp_server.hpp"

int main(int argc, char* argv[])
{
    if(argc > 1 && strcmp(argv[1], "-s") == 0) {
        lightnet::IoService io_service;
        lightnet::TcpServer tcp_server(io_service, "127.0.0.1", 7000);
        io_service.run();
    } else {
        lightnet::IoService io_service;
        lightnet::TcpClient tcp_client(io_service);
        tcp_client.connect("127.0.0.1", 7000);
        io_service.run();
    }
}

