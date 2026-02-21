#include "../../include/config/WebservConf.hpp"

void WebservConf::pushServer(const ServerConf& serv, const ENDPOINT& endPoint)
{
	server_pool[endPoint].push_back(serv);
}

int WebservConf::getNumberOfServers() const
{
	return (int)this->server_pool.size();
}

void WebservConf::print() const
{
    if (server_pool.empty())
        return;

    std::map<ENDPOINT, std::vector<ServerConf>, ENDPOINTCOMP>::const_iterator it;
    for (it = server_pool.begin(); it != server_pool.end(); ++it)
    {
        // std::cout << "Endpoint " << it->first.ip << ":" << it->first.port << "\n";
        for (size_t i = 0; i < it->second.size(); ++i)
            it->second[i].print();
    }
}