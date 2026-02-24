#include "../../include/config/WebservConf.hpp"


void WebservConf::pushServer(const ServerConf &serv, const ENDPOINT &endPoint)
{
	this->virtual_servers[endPoint].push_back(serv);
	this->available_ports.push_back(serv.getPort());
}

const std::vector<ServerConf> *WebservConf::matchServer(const uint32_t ipv4, const int port)
{
	ENDPOINT endpoint{ipv4,port};
	std::vector<ServerConf> *ptr = nullptr;

	std::map<ENDPOINT,std::vector<ServerConf>,ENDPOINTCOMP>::iterator it = virtual_servers.find(endpoint);
	if (it != this->virtual_servers.end())
		ptr = &it->second;
	else
	{
		endpoint.ip = 0;
		it = virtual_servers.find(endpoint);
		if (it != this->virtual_servers.end())
			ptr = &it->second;
	}
	return ptr;
}

int WebservConf::getNumberOfServers() const
{
	return (int)this->virtual_servers.size();
}

const std::vector<int> &WebservConf::getAvailablePorts()
{
	return this->available_ports;
}
void WebservConf::print() const
{
    if (virtual_servers.empty())
        return;

    std::map<ENDPOINT, std::vector<ServerConf>, ENDPOINTCOMP>::const_iterator it;
    for (it = virtual_servers.begin(); it != virtual_servers.end(); ++it)
    {
        // std::cout << "Endpoint " << it->first.ip << ":" << it->first.port << "\n";
        for (size_t i = 0; i < it->second.size(); ++i)
            it->second[i].print();
    }
}