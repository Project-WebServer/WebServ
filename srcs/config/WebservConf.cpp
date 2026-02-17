#include "../../include/config/WebservConf.hpp"

void WebservConf::pushServer(ServerConf& serv)
{
	servers.push_back(serv);
}

void WebservConf::print() const
{
	std::cout << "test5\n";
	if (!this->servers.empty())
	{
		std::cout << "test6\n";
		for (size_t i = 0; i < this->servers.size(); ++i)
			this->servers[i].print();
	}
	std::cout << "test7\n";
}