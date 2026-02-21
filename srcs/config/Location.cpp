#include "../../include/config/Location.hpp"
#include "../../include/config/conf_parse.hpp"


//set default values for root. shoult it be srcs?? or what
// matching path = root + path + uri
Location::Location(): path(""), 
    root(""), 
    autoindex(false) 
    // has_redirection(false),
    // upload_enable(false)
	
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


void	Location::setPath(std::string& path)
{
    this->path = path;
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