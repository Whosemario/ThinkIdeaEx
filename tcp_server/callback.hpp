//
//  callback.hpp
//  libuvcpp
//
//  Created by whosemario on 16-2-5.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#pragma once

#include <utility>

namespace lightnet {
    template<typename callback_t>
    class Callback {
    public:
        Callback(callback_t& cb)
        :cb(cb){}
        template<typename ... Args>
        void call(Args&& ... args) {
            cb(std::forward<Args>(args)...);
        }
    private:
        callback_t cb;
    };
    
    void test_callback1();
    void test_callback2();
}
