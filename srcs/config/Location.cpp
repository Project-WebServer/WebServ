#include "../../include/config/Location.hpp"
#include "../../include/config/conf_parse.hpp"

//set default values for root. shoult it be srcs?? or what
// matching path = root + uri
Location::Location(const std::string& servRoot): prefix(""), 
    root(servRoot), 
    autoindex(false),
    upload_enable(false),
	upload_store(""),
    has_redirection(false)
{
}

Location &Location::operator=(const Location &other)
{
	if (this != &other)
	{
		prefix = other.prefix;
		root = other.root;
		allowed_methods = other.allowed_methods;
		index_files = other.index_files;
		autoindex = other.autoindex;
		///others
	}
	return *this;
}


void	Location::setPrefix(std::string& prefix)
{
    this->prefix = prefix;
}

//where is the root in our webserver (where we gonna put our static files)
//verify here if the path is valid with stat / access??
// handle the path to ww in our directory 
void	Location::setRoot(std::string& root)
{
	this->root = root;
}

void	Location::setAllowed_methods(std::string method)
{
    this->allowed_methods.insert(getHttpMethod(method));
}

//implement it 
void	Location::setIndex_files(std::string index_file)
{
    this->index_files.push_back(index_file);
}

void Location::setAutoindex_On()
{
	this->autoindex = true;
}

void Location::setUpload(std::string upload_path)
{
	this->upload_enable = true;
	this->upload_store = upload_path;
}
void Location::setRedirection(std::string url, int code)
{
	this->has_redirection = true;
	this->redir_code = code;
	this->redir_url = url;
}

std::string Location::getPrefix() const
{
	return this->prefix;
}

std::string Location::getRoot() const
{
	return this->root;
}

std::vector<httpMethod> Location::getAllowed_methods() const
{
	std::vector<httpMethod> methods;

	for (auto s : allowed_methods)
	{
		if (s == httpMethod::GET)
			methods.push_back(httpMethod::GET);
		else if (s == httpMethod::DELETE)
			methods.push_back(httpMethod::DELETE);
		else if (s == httpMethod::POST)
			methods.push_back(httpMethod::POST);
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

bool Location::hasRedirection() const
{
	return has_redirection;
}
bool Location::is_uploadEnable() const
{
	return upload_enable;
}
std::string Location::getUploadPath() const
{
	return upload_store;
}
int Location::getRedirCode() const
{
	return redir_code;
}
std::string Location::getRedirUrl() const
{
	return redir_url;
}
std::string Location::resolvePath(std::string &uri) const
{
	std::string _realPath = getRoot() + uri;
	return _realPath;
}

void Location::print() const
{

    if (!this->getRoot().empty())
        std::cout << "\t\troot " << this->getRoot() << ";" << std::endl;

    if (!this->getIndex_files().empty())
    {
        std::cout << "\t\tindex";
        const std::vector<std::string>& files = this->getIndex_files();
        for (size_t i = 0; i < files.size(); i++)
            std::cout << " " << files[i];
        std::cout << ";" << std::endl;
    }

    if (!this->getAllowed_methods().empty())
{
    std::vector<httpMethod> methods = this->getAllowed_methods();
    std::cout << "\t\tallowed_methods";
    for (size_t i = 0; i < methods.size(); i++)
    {
        if (methods[i] == httpMethod::GET)
            std::cout << " GET";
        else if (methods[i] == httpMethod::POST)
            std::cout << " POST";
        else if (methods[i] == httpMethod::DELETE)
            std::cout << " DELETE";
    }
    std::cout << ";" << std::endl;
}

    std::cout << "\t\tautoindex " << (this->getAutoindex() ? "on" : "off") << ";" << std::endl;

    if (this->hasRedirection())
    {
        std::cout << "\t\treturn " << this->getRedirCode()
                  << " " << this->getRedirUrl() << ";" << std::endl;
    }

    if (this->is_uploadEnable())
    {
        std::cout << "\t\tupload_enable on;" << std::endl;
        if (!this->getUploadPath().empty())
            std::cout << "\t\tupload_path " << this->getUploadPath() << ";" << std::endl;
    }

}

