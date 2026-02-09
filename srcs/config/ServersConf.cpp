#include "../../include/config/ServersConf.hpp"
#include "../../include/config/conf_parse.hpp"

void ServerConf::setListen(TokenLine& tokenLine)
{
	error_conf status;
	TokenLine tmp = tokenLine;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);

	std::vector<std::string> &v = tokenLine.second;
	if (v.size() != 1)
		throw std::runtime_error("Error: invalid token near " + ConfToken::catTokens(tmp));
	std::string ip = v[0];

	size_t colonPos = ip.find(':');
	try
	{
		if (colonPos == std::string::npos)
		{
			this->listen.first = "0.0.0.0";
			this->listen.second = stoi(ip.substr(colonPos + 1));
			if (listen.second < 0 || listen.second > 65535)
				throw std::runtime_error("Error: Invalid port value near " + ip);
		}
		else
		{
			std::string host = ip.substr(0, colonPos);
			if (!isHostValid(host).success)
				throw std::runtime_error("Error: Invalid IP address near " + ip);
			this->listen.first = host;
			this->listen.second = stoi(ip.substr(colonPos + 1));
			if (listen.second < 0 || listen.second > 65535)
				throw std::runtime_error("Error: Invalid port value near " + ip);
		}
	}
	catch (std::exception &e)
	{
		throw std::runtime_error("Error:: Invalid token near " + ConfToken::catTokens(tmp));
	}
	return;
}
