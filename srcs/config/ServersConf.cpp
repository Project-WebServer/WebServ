#include "../../include/config/ServersConf.hpp"
#include "../../include/config/conf_parse.hpp"

void ServerConf::setHost(std::vector<std::string> &v)
{
	if (!isHostValid(v))
		//throw implement error here
	this->host = v[0]; 
}