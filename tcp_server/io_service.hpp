//
//  io_service.hpp
//  libuvcpp
//
//  Created by whosemario on 16-2-6.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#pragma once

#include <uv.h>
#include <memory>
#include <functional>

namespace lightnet {
    class IoService {
    public:
        IoService(bool use_default = true);
        void run();
        uv_loop_t* get();
    private:
        typedef std::function<void(uv_loop_t*)> Deleter;
        void destroy(uv_loop_t* loop_t) {
            if(!m_use_default)
                delete loop_t;
        }
        bool m_use_default;
        std::unique_ptr<uv_loop_t, Deleter> m_loop;
    };
    
    void test_ioservice1();
}
