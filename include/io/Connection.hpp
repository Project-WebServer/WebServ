#ifndef CONNECTION_HPP
#define CONNECTION_HPP

# include <string>

struct Connection
		{
			std::string in_buf;
			std::string out_buf;
			bool want_write;
			bool should_close;
			//timestamps
			//last_activity

			Connection() : want_write(false), should_close(false) {}
		};

#endif