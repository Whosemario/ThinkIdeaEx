/*
*/

#pragma once

#include <asio/detail/noncopyable.hpp>
#include <asio/asio.hpp>
#include <memory>
#include <iostream>

namespace hex_engine {
namespace rpc {

class streambuf_ex : public asio::streambuf {
protected:
	pos_type seekoff(off_type off, std::ios_base::seekdir way, std::ios_base::openmode mode) {
        return _seekoff(off, way, mode);
	}

	pos_type seekpos(pos_type off, std::ios_base::openmode mode) {
        return _seekoff(off, std::ios_base::cur, mode);
	}
private:
    pos_type _seekoff(pos_type off, std::ios_base::seekdir way, std::ios_base::openmode mode) {
        pos_type ret = pos_type(-1);
        if(mode & std::ios_base::in) {
            if(mode & std::ios_base::out) {
                return ret;
            }
            if(pptr() && egptr() < pptr())
                setg(eback(), gptr(), pptr());
            pos_type offset = off;
            if(way == std::ios_base::cur) {
                offset += gptr() - eback();
            } else if(way == std::ios_base::end) {
                offset += egptr() - eback();
            }
            
            if(offset >= 0 && egptr() - eback() >= offset) {
                setg(eback(), gptr() + offset, egptr());
                ret = offset;
            }
        } else if(mode & std::ios_base::out) {
            if(pptr() && egptr() < pptr())
                setg(eback(), gptr(), pptr());
            pos_type offset = off;
            if(way == std::ios_base::cur) {
                offset += pptr() - eback();
            } else if(way == std::ios_base::end) {
                offset += egptr() - eback();
            }
            if(offset >= 0) {
                setp(eback(), epptr());
                pbump(offset);
                ret = offset;
            }
            
        }
        return ret;
    }
};

class rpc_stream : private asio::noncopyable {
public:
	rpc_stream();
public:
	std::istream& read_stream() { return stream_; }
	std::ostream& write_stream() { return stream_; }
	streambuf_ex& buf() { return buf_; }
	void reset();

	char* c_str();

private:
	streambuf_ex buf_;
	std::iostream stream_;
};

typedef std::shared_ptr<rpc_stream> rpc_stream_ptr;

}
}
