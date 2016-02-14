//
//  communication_service.hpp
//  libuvcpp
//
//  Created by whosemario on 16-2-8.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#pragma once

#include "proto/message_communication.pb.h"
#include "basic_service.hpp"
#include "tcp_connection.hpp"

namespace lightnet {
    typedef BasicService<communication::CommunicationService, communication::ReplyService_Stub> basic_service;
    class ServerService: public basic_service {
    public:
        ServerService(TcpConnection&);
        
        void send_message(::google::protobuf::RpcController* controller,
                                         const ::communication::Message* msg,
                                         ::communication::Void* rep,
                                         ::google::protobuf::Closure* done);
    };
    
    typedef BasicService<communication::ReplyService, communication::CommunicationService_Stub> client_basic_service;
    class ClientService: public client_basic_service {
    public:
        ClientService(TcpConnection&);
        void SendMessage(const std::string& content);
    };
}