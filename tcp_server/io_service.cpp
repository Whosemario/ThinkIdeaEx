//
//  io_service.cpp
//  libuvcpp
//
//  Created by whosemario on 16-2-6.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#include "io_service.hpp"

namespace lightnet {
    
    IoService::IoService(bool use_default)
    : m_use_default(use_default)
    , m_loop(use_default ? uv_default_loop() : new uv_loop_t, [this](uv_loop_t* loop_t) {
        this->destroy(loop_t);
    }) {
        if(!m_use_default) {
            uv_loop_init(m_loop.get());
        }
    }
    
    void IoService::run() {
        uv_run(m_loop.get(), UV_RUN_DEFAULT);
    }
    
    uv_loop_t* IoService::get() {
        return m_loop.get();
    }
    
    void test_ioservice1() {
        IoService io_service;
        io_service.run();
    }
}