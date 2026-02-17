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

Location &Location::operator=(const Location &other)
{
	if (this != &other)
	{
		path = other.path;
		root = other.root;
		allowed_methods = other.allowed_methods;
		index_files = other.index_files;
		autoindex = other.autoindex;
		///others
	}
	return *this;
}


//no implementantion of modifiers (=, ~, ~*, ^~, '')
void	Location::setPath(TokenLine &tokenLine)
{
    std::vector<std::string> &location = tokenLine.second;
    if (location.size() != 2)
        throw std::runtime_error("Error: invalid location block near token " + ConfToken::catTokens(tokenLine));
    if (location[1] != "{")
        throw std::runtime_error("Error: missing open block '{' location near token " + ConfToken::catTokens(tokenLine));
    
    std::string &path = location[0];
    if (path.front() != '/')
        throw std::runtime_error("Error: invalid location path near token " + ConfToken::catTokens(tokenLine));
    this->path = path;
}

//where is the root in our webserver (where we gonna put our static files)
void	Location::setRoot(TokenLine &tokenLine)
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
        throw std::runtime_error("Error: invalid location path near token " + ConfToken::catTokens(tokenLine));
    this->root = root;
}

void	Location::setAllowed_methods(TokenLine &tokenLine)
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
void	Location::setIndex_files(TokenLine &tokenLine)
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

void Location::setAutoindex(TokenLine &tokenLine)
{
	error_conf status;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);
	std::vector<std::string> &token = tokenLine.second;
	if (token.size() != 1)
        throw std::runtime_error("Error: invalid directive near token " + ConfToken::catTokens(tokenLine));
	if (token[0] == "on")
		this->autoindex = true;
}

std::string Location::getPath() const
{
	return this->path;
}

std::string Location::getRoot() const
{
	return this->root;
}

std::vector<std::string> Location::getAllowed_methods() const
{
	std::vector<std::string> methods;

	for (auto s : allowed_methods)
	{
		if (s == httpMethod::GET)
			methods.push_back("GET");
		else if (s == httpMethod::DELETE)
			methods.push_back("DELETE");
		else if (s == httpMethod::POST)
			methods.push_back("POST");
	}

	return methods;
}

const std::vector<std::string>& Location::getIndex_files() const
{
	return this->index_files;
}

bool Location::getAutoindex() const
{
	return autoindex;
}

void Location::print() const
{
    if (!this->root.empty())
        std::cout << "\t\troot " << this->root << ";" << std::endl;

    if (!this->index_files.empty())
    {
        std::cout << "\t\tindex";
        for (size_t i = 0; i < this->index_files.size(); i++)
            std::cout << " " << this->index_files[i];
        std::cout << ";" << std::endl;
    }

    if (!this->allowed_methods.empty())
    {
		std::vector<std::string> methods = getAllowed_methods();
        std::cout << "\t\tallowed_methods";
        for (size_t i = 0; i < methods.size(); i++)
            std::cout << " " << methods[i];
        std::cout << ";" << std::endl;
    }

    std::cout << "\t\tautoindex " << (this->autoindex ? "on" : "off") << ";" << std::endl;
}