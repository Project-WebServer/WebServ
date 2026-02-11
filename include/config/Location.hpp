#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "conf_parse.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>


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
			void	setpath(TokenLine &tokenLine); //ok
			void	root(TokenLine &tokenLine); //ok
			void	allowed_methods(TokenLine &tokenLine); //ok
			void	index_files(TokenLine &tokenLine); //
			void	autoindex(TokenLine &tokenLine);

};
 

#endif