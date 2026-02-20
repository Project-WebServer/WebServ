#ifndef LOCATION_HPP
#define LOCATION_HPP


#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

enum struct httpMethod;
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
			void	setPath(std::string& path);
			void	setRoot(std::string& root);
			void	setAllowed_methods(std::string method);
			void	setIndex_files(std::string index_file);
			void	setAutoindex_On();

			std::string						getPath() const; 
			std::string						getRoot() const; 
			std::vector<std::string> 		getAllowed_methods() const; // for debug 
			const std::vector<std::string>&	getIndex_files() const; // 
			bool							getAutoindex() const; //

			void print() const;
};
 

#endif