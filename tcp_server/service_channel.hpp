//
//  service_channel.hpp
//  libuvcpp
//
//  Created by whosemario on 16-2-7.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#pragma once

#include <string>
#include <memory>
#include <google/protobuf/service.h>

namespace lightnet {
    class TcpConnection;
    class AbstractService;
    class ServiceChannel : public google::protobuf::RpcChannel {
    public:
        enum State {
            data_size = 0,
            data_index = 1,
            data_content = 2,
        };
    public:
        ServiceChannel(TcpConnection&, AbstractService*);
        virtual void CallMethod(const google::protobuf::MethodDescriptor* method,
                                google::protobuf::RpcController* controller,
                                const google::protobuf::Message* request,
                                google::protobuf::Message* response,
                                google::protobuf::Closure* done);
        
        void ProcessInputData(char* data, int size);
        
    private:
        std::shared_ptr<std::string> SerializeMethod(const google::protobuf::MethodDescriptor* method,
                                                     const google::protobuf::Message* request);
        void reset();
    private:
        TcpConnection& tcp_connection;
        AbstractService* service;
    private:
        // 处理数据相关
        State currState;
        char rawDataSize[4];
        int rawDataSizeCnt;
        int dataSize;
        char rawDataIndex[4];
        int rawDataIndexCnt;
        int dataIndex;
        std::string content;
        
    };
}