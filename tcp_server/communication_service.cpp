//
//  communication_service.cpp
//  libuvcpp
//
//  Created by whosemario on 16-2-9.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#include <iostream>
#include "communication_service.hpp"

namespace lightnet {
    ServerService::ServerService(TcpConnection& tcp_connection)
    :basic_service(tcp_connection) {}
    
    void ServerService::send_message(::google::protobuf::RpcController* controller,
                      const ::communication::Message* msg,
                      ::communication::Void* rep,
                      ::google::protobuf::Closure* done) {
        std::cout << "receive data is " << msg->content() << std::endl;
    }
    
    ClientService::ClientService(TcpConnection& tcp_connection)
    :client_basic_service(tcp_connection) {}
    
    void ClientService::SendMessage(const std::string& content) {
        communication::Message msg;
        msg.set_content(content);
        stub.send_message(nullptr, &msg, nullptr, nullptr);
    }
}