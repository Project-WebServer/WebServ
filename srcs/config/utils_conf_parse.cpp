// #include "../../include/config/WebservConf.hpp"
#include "../../include/config/conf_parse.hpp"


std::string	trim(const std::string& s)
{
	const char* whitespace = " \t\n\v";

	size_t	start = s.find_first_not_of(whitespace);

	if (start == std::string::npos)
		return "";
	
	size_t last = s.find_last_not_of(whitespace);

	return s.substr(start, last - start + 1);
}

bool	isDigitOnly(std::string &s)
{
	if (!s.empty() && std::all_of(s.begin(), s.end(), ::isdigit))
		return true;
	return false;
}



error_conf isDirectiveValid(TokenLine& tokenLine)
{
	std::vector<std::string> &v = tokenLine.second;

	if (v.size() < 1)
		return {false, "Error: Invalid directive near " + ConfToken::catTokens(tokenLine)};
	if (v[0] == "=")
		v.erase(v.begin());
	std::string &last = v.back();
	if (last.empty() || last.back() != ';')
		return {false, "Error: missing ';' token near " + ConfToken::catTokens(tokenLine)};
	last.pop_back();
	return {true, 0};
} 


error_conf	isHostValid(std::string &s)
{
	std::string &ip = s;
	if (ip.empty())
		return {false, 0};
	if (!ip.empty() && ip.back() == ';')
        ip.pop_back();
	if (ip == "localhost")
	{
		ip = "127.0.0.1";
        return {true, 0};
	}
	if (ip.length() < 7 || ip.length() > 15)
		return {false, 0}; 

	std::stringstream ss(ip);
	std::string  oct;
	int Octcount = 0;

	while (std::getline(ss,oct, '.'))
	{
		if (oct.empty())
            return {false, 0};
		if (oct.length() > 1 && oct[0] == '0')
            return {false, 0};
		for (char c : oct) {
            if (!std::isdigit(c))
                return {false, 0};
		}
		try
		{
			int n = std::stoi(oct);
			if(n < 0 || n > 255)
				return {false, 0};
			Octcount++;
		}
		catch(const std::exception& e)
		{
			return {false, 0};
		}
	}
	if (Octcount != 4)
		return {false, 0};
	return {true, 0};
}

httpMethod getHttpMethod(std::string token)
{
	if (token == "GET") return httpMethod::GET;
	if (token == "DELETE") return httpMethod::DELETE;
	if (token == "POST") return httpMethod::POST;
	
	return httpMethod::UNKNOWN;
}
