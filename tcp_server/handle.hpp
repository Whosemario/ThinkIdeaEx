//
//  handle.hpp
//  libuvcpp
//
//  Created by whosemario on 16-2-4.
//  Copyright (c) 2016年 whosemario. All rights reserved.
//

#pragma once

namespace uvpp {
    template<typename HANDLE_T>
    class Handle {
    protected:
        Handle()
        : m_uv_handle(new HANDLE_T) {
            assert(m_uv_handle);
        }
    private:
        HANDLE_T* m_uv_handle;
    };
}
