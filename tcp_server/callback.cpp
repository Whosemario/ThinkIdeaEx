//
//  callback.cpp
//  libuvcpp
//
//  Created by whosemario on 16-2-5.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#include "callback.hpp"
#include <iostream>
#include <functional>

namespace lightnet {
    void test_callback1() {
        std::function<void()> cb = [](){
            std::cout << "call 1 in ." << std::endl;
        };
        
        Callback<std::function<void()>> cbObj(cb);
        cbObj.call();
    }
    
    void test_callback2() {
        std::function<void()> cb = std::function<void()>([](){
            std::cout << "call 2 in ." << std::endl;
        });
        
        Callback<std::function<void()>> cbObj(cb);
        cbObj.call();
    }
}


