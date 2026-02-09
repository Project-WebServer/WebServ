// #include "../../include/config/WebservConf.hpp"
#include "../../include/config/conf_parse.hpp"

int main()
{
	ConfToken parser;

	parser.getFile("exemple.conf");

	TokenLine line;

	line = parser.getNextToken();

	while (line.first != tokenType::ENDOF)
	{
		std::cout << parser.catTokens(line) + "\n";
		line = parser.getNextToken();
	}
	
	return 0;
}