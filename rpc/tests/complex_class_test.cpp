/*
 */

#include <iostream>
#include <msgpack.hpp>
#include <string>
#include <vector>
#include <sstream>

struct base {
	int val;
	MSGPACK_DEFINE(val);
};

enum request_type {
	NEW_CONNECTION = 1,
	RE_CONNECTION = 2,
};
MSGPACK_ADD_ENUM(request_type);

struct myclass {
	std::string str;
	std::vector<int> vec;
	base type;
	request_type re;
	MSGPACK_DEFINE(str, type, re);
};


int main(int args, char* argv[])
{
	myclass mc;
	mc.str = "123";
	mc.vec.emplace_back(12);
	mc.vec.emplace_back(102);
	mc.type.val = 3;
	mc.re = RE_CONNECTION;

	std::tuple<myclass> t(mc);

	std::stringstream buffer;
	msgpack::pack(buffer, t);

	buffer.seekg(0);
	std::string s(buffer.str());

	std::tuple<myclass> dest;
	msgpack::unpack(s.c_str(), s.size()).get().convert(dest);

	std::cout << std::get<0>(dest).re << std::endl;

	return 1;
}