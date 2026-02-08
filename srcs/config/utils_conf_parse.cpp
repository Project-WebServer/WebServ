#include "../../include/config/WebservConf.hpp"
#include "../../include/config/conf_parse.hpp"

std::string	trim(const std::string& s)
{
	const char* whitespace = " \t\n\v";

	size_t	start = s.find_first_of(whitespace);

	if (start == std::string::npos)
		return "";
	
	size_t last = s.find_last_not_of(whitespace);

	return s.substr(start, last - start + 1);
}

bool	isHostValid(std::vector<std::string> &v)
{
	if (v.size() != 1)
		return false;

	std::string ip(v[0]);
	if (ip.empty())
		return false;
	if (!ip.empty() && ip.back() == ';')
        ip.pop_back();
	if (ip == "localhost")
        return true;
	if (ip.length() < 7 || ip.length() > 15)
		return false; 

	std::stringstream ss(ip);
	std::string  oct;
	int Octcount = 0;

	while (std::getline(ss,oct, '.'))
	{
		if (oct.empty())
            return false;
		if (oct.length() > 1 && oct[0] == '0')
            return false;
		for (char c : oct) {
            if (!std::isdigit(c))
                return false;
		}
		try
		{
			int n = std::stoi(oct);
			if(n < 0 || n > 255)
				return false;
			Octcount++;
		}
		catch(const std::exception& e)
		{
			return false;
		}
	}
	if (Octcount != 4)
		return false;
	return true;
}
