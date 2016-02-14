//
//  timer.hpp
//  libuvcpp
//
//  Created by whosemario on 16-2-4.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#pragma once

#include <uv.h>
#include <iostream>
#include <memory>
#include <functional>
#include <assert.h>
#include "callback.hpp"

namespace lightnet {
    class IoService;
    class Timer {
    public:
        typedef Callback<std::function<void()>> TimerCallback;
        
        Timer(IoService& loop)
        :m_pTimer(new uv_timer_t) {
            assert(m_pTimer);
            uv_timer_init(loop.get(), m_pTimer.get());
        }
        
        int start(std::function<void()> callback, uint64_t timeout, uint64_t repeat) {
            m_pTimer.get()->data = new TimerCallback(callback);
            return uv_timer_start(m_pTimer.get(),
                           [](uv_timer_t* handler){
                               TimerCallback* cb = (TimerCallback*)(handler->data);
                               cb->call();
                           },
                           timeout,
                           repeat);
        }
    private:
        std::unique_ptr<uv_timer_t> m_pTimer;
    };
}
