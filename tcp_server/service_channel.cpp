//
//  service_channel.cpp
//  libuvcpp
//
//  Created by whosemario on 16-2-7.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#include <sstream>
#include "service_channel.hpp"
#include <google/protobuf/message.h>
#include "tcp_connection.hpp"
#include "basic_service.hpp"

namespace lightnet {
    
    ServiceChannel::ServiceChannel(TcpConnection& tcp_connection,
                                   AbstractService* service)
    :google::protobuf::RpcChannel()
    ,tcp_connection(tcp_connection)
    ,service(service)
    ,currState(data_size){
        reset();
    }
    
    void ServiceChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                                    google::protobuf::RpcController *controller,
                                    const google::protobuf::Message *request,
                                    google::protobuf::Message *response,
                                    google::protobuf::Closure *done) {
        std::shared_ptr<std::string> serialize = SerializeMethod(method, request);
        // write to remote
        tcp_connection.WriteToRemove(serialize->c_str(), serialize->length());
    }
    
    void ServiceChannel::ProcessInputData(char* data, int size) {
        int index = 0;
        while(index < size) {
            int remainSize = size - index;
            switch (currState) {
                case data_size:
                    if(remainSize + rawDataSizeCnt >= 4) {
                        // 字符满足
                        for(; rawDataSizeCnt < 4; ++rawDataSizeCnt) {
                            rawDataSize[rawDataSizeCnt] = data[index];
                            ++index;
                        }
                        dataSize = *(reinterpret_cast<int*>(rawDataSize)) - 4;
                        currState = data_index;
                    } else {
                        for(; index < size; ++index) {
                            rawDataSize[rawDataSizeCnt] = data[index];
                            ++rawDataSizeCnt;
                        }
                    }
                    break;
                case data_index:
                    if(remainSize + rawDataIndexCnt >= 4) {
                        for(; rawDataIndexCnt < 4; ++rawDataIndexCnt) {
                            rawDataIndex[rawDataIndexCnt] = data[index];
                            ++index;
                        }
                        dataIndex = *(reinterpret_cast<int*>(rawDataIndex));
                        currState = data_content;
                    } else {
                        for(; index < size; ++index) {
                            rawDataIndex[rawDataIndexCnt] = data[index];
                            ++rawDataIndexCnt;
                        }
                    }
                    break;
                case data_content:
                    if(remainSize + content.length() >= dataSize) {
                        int addOffset = dataSize - content.length();
                        content += std::string(data+index, data+index+addOffset);
                        index += addOffset;
                        service->CallMethod(dataIndex, content);
                        reset();
                    } else {
                        content += std::string(data+index, data+size);
                        index = size;
                    }
                    break;
                default:
                    return;
            }
        }
    }
    
    
    std::shared_ptr<std::string> ServiceChannel::SerializeMethod(const google::protobuf::MethodDescriptor* method,
                                                                 const google::protobuf::Message* request) {
        if(!request) return nullptr;
        std::ostringstream os;
        int total_len = 0;
        os.write(reinterpret_cast<char*>(&total_len), sizeof(total_len));
        int index = method->index();
        os.write(reinterpret_cast<char*>(&index), sizeof(index));
        if(request->SerializeToOstream(&os)) {
            total_len = os.tellp();
            int data_len = total_len - sizeof(total_len);
            os.seekp(0, std::ios_base::beg);
            os.write(reinterpret_cast<char*>(&data_len), sizeof(total_len));
            os.seekp(total_len);
            std::shared_ptr<std::string> ret(new std::string(os.str()));
            if(ret->empty()) return nullptr;
            return ret;
        }
        return nullptr;
    }
    
    void ServiceChannel::reset() {
        rawDataSizeCnt = 0;
        rawDataIndexCnt = 0;
        dataIndex = 0;
        dataSize = 0;
        content = "";
    }
}
