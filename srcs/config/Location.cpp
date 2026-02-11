#include "../../include/config/Location.hpp"
#include "../../include/config/conf_parse.hpp"

//set default values for root. shoult it be srcs?? or what
// matching path = root + path + uri
Location::Location(): path(""), 
    root(""), 
    autoindex(false), 
    has_redirection(false),
    upload_enable(false)
{
}

//no implementantion of modifiers (=, ~, ~*, ^~, '')
void	Location::setpath(TokenLine &tokenLine)
{
    std::vector<std::string> &location = tokenLine.second;
    if (location.size() != 2)
        throw std::runtime_error("Error: invalid location block near token " + ConfToken::catTokens(tokenLine));
    if (location[1] != "{")
        throw std::runtime_error("Error: missing open block '{' location near token " ConfToken::catTokens(tokenLine));
    
    std::string &path = location[0];
    if (path.front() != '/')
        throw std::runtime_error("Error: invalid location path near token " ConfToken::catTokens(tokenLine));
    this->path = path;
}

//where is the root in our webserver (where we gonna put our static files)
void	Location::root(TokenLine &tokenLine)
{
    error_conf status;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);
    std::vector<std::string> &token = tokenLine.second;
    if (token.size() != 1)
        throw std::runtime_error("Error: invalid root directive near token " + ConfToken::catTokens(tokenLine));
    std::string &root = token[0];
    if (root.front() != '/')
        throw std::runtime_error("Error: invalid location path near token " ConfToken::catTokens(tokenLine));
    this->root = root;
}

void	Location::allowed_methods(TokenLine &tokenLine)
{
    error_conf status;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);
    std::vector<std::string> &token = tokenLine.second;
    if (token.size() < 1)
        throw std::runtime_error("Error: invalid allowed_method directive near token " + ConfToken::catTokens(tokenLine));
    for (auto method : token)
        this->allowed_methods.insert(getHttpMethod(method));
}

//implement it 
void	Location::index_files(TokenLine &tokenLine)
{
     error_conf status;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);
    std::vector<std::string> &token = tokenLine.second;
    if (token.size() < 1)
        throw std::runtime_error("Error: invalid allowed_method directive near token " + ConfToken::catTokens(tokenLine));
    for (auto index : token)
        this->index_files.push_back(index);
}