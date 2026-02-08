#include "../../include/config/WebservConf.hpp"

void WebservConf::pushServer(ServerConf serv)
{
	servers.push_back(serv);
}