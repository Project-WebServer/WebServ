#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "conf_parse.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

enum struct httpMethod
{
	GET,
	POST,
	DELETE
};

//store the servers 
class Location
{
	private:
		std::string 				path;
		std::string					root;
		std::set<httpMethod>		allowed_methods;
		std::vector<std::string>	index_files;
		bool						autoindex;

		//redirections
		bool		has_redirection;
		std::string	redir_code;
		std::string	redir_url;

		//upload
		bool		upload_enable;
		std::string upload_path;
		size_t		client_max_body_size;



		public:
			Location();
			~Location();

			//getters
			//setters
};
 

#endif