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
			//set copy constructor
			Location& operator=(const Location& other);
			~Location(){};

			//getters
			void	setPath(TokenLine &tokenLine); //ok
			void	setRoot(TokenLine &tokenLine); //ok
			void	setAllowed_methods(TokenLine &tokenLine); //ok
			void	setIndex_files(TokenLine &tokenLine); // ok
			void	setAutoindex(TokenLine &tokenLine); //ok

			std::string						getPath() const; 
			std::string						getRoot() const; 
			std::vector<std::string> 		getAllowed_methods() const; // for debug 
			const std::vector<std::string>&	getIndex_files() const; // 
			bool							getAutoindex() const; //

			void print() const;
};
 

#endif