//
//  basic_service.hpp
//  libuvcpp
//
//  Created by whosemario on 16-2-8.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#pragma once

#include <string>
#include <sstream>
#include "service_channel.hpp"
#include "tcp_connection.hpp"
#include <google/protobuf/descriptor.h>

namespace lightnet {
    
    class AbstractService {
    public:
        virtual void CallMethod(int index, std::string& content) = 0;
    };
    
    template<typename Service, typename ServiceStub>
    class BasicService : public AbstractService, public Service {
    public:
        BasicService(TcpConnection& tcp_connection)
        : channel(tcp_connection, this)
        , stub(&channel) {
            tcp_connection.SetChannel(&channel);
        }
        
        virtual void CallMethod(int index, std::string& content) {
            const google::protobuf::MethodDescriptor *method = Service::descriptor()->method(index);
            google::protobuf::Message* msg = Service::GetRequestPrototype(method).New();
            std::istringstream is(content);
            msg->ParseFromIstream(&is);
            Service::CallMethod(method, nullptr, msg, nullptr, nullptr);
        }
    private:
        ServiceChannel channel;
    protected:
        ServiceStub stub;
    };
}
