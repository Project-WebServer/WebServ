#include "../../include/config/WebservConf.hpp"

void WebservConf::pushServer(ServerConf& serv)
{
	servers.push_back(serv);
}

void WebservConf::print() const
{
	if (!this->servers.empty())
	{
		for (size_t i = 0; i < this->servers.size(); ++i)
			this->servers[i].print();
	}
}