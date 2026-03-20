
Response handler -> Yuleum 
	function : void	responseHandler(HTTPrequests& request, WebservConf& servConf, std::string& _response);
		take a request and WebservConf : used to select the right virtual server based on request IP and PORT
				string _response : used to save the response

	Use: This function will return already a response in header + body (file html content) format. It will return the content requested or the error page based on the error code.
		for now it just handle GET method and error codes
	Need add the Response.hpp header to use it.