#ifndef LOCATION_HPP
#define LOCATION_HPP


#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

enum struct httpMethod;
class Location
{
	private:
		std::string 				prefix;
		std::string					root;
		std::set<httpMethod>		allowed_methods;
		std::vector<std::string>	index_files;
		bool						autoindex;

		//remove it 
		bool		upload_enable;
		std::string upload_store;

		// //redirections 
		bool		has_redirection;
		int			redir_code;
		std::string	redir_url;


		bool        has_cgi;
		std::string cgiInterpreter;    // ex: /usr/bin/python3
		std::string cgiExt;     // ex: .py

		public:
			Location(const std::string& servRoot);
			//set copy constructor
			Location& operator=(const Location& other);
			~Location(){};

			//getters
			void	setPrefix(std::string& prefix);
			void	setRoot(std::string& root);
			void	setAllowed_methods(std::string method);
			void	setIndex_files(std::string index_file);
			void	setAutoindex_On();
			void	setUpload(std::string upload_path);
			void	setRedirection(std::string url, int code);
			void	setCgiPass(std::vector<std::string>& pass);

			std::string						getPrefix() const; 
			std::string						getRoot() const; 
			std::vector<httpMethod> 		getAllowed_methods() const; // for debug 
			const std::vector<std::string>&	getIndex_files() const; // 
			bool							getAutoindex() const; //
			bool							hasRedirection() const;
			bool							is_uploadEnable() const;
			std::string						getUploadPath() const;
			int								getRedirCode() const;
			std::string						getRedirUrl() const;
			bool							hasCGI() const;
			std::string						getCGIext() const;
			std::string						getCgiInterpreter() const;

			std::string resolvePath(std::string& uri) const;

			void print() const;
};
 

#endif