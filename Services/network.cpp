#include <iostream>

#include "network.hpp"
 
using boost::asio::ip::tcp;

void Network::SendGETRequest()
{
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET /bot6329343331:AAER5UjGOmLh_G7wBbEG7VNtBvyYQBD84Z0/getUpdates HTTP/1.1\r\n"
                       << "Host: api.telegram.org\r\n"
                       << "User-Agent: custom-client\r\n"
                       << "Connection: close\r\n"
                       << "Content-Type: application/json\r\n"
                       << "Content-Length: 32\r\n\r\n"
                       << "{\"allowed_updates\":[\"message\"]}\r\n";
    
    SendRequest(request);
}

void Network::SendRequest(boost::asio::streambuf& request)
{
    boost::asio::ssl::stream<tcp::socket> socket(service, ssl_ctx);

    tcp::resolver resolver(service);
    tcp::resolver::query query("api.telegram.org" , "https");
    tcp::resolver::iterator iter = resolver.resolve(query);
    
    boost::asio::connect(socket.lowest_layer(), iter);
    socket.handshake(boost::asio::ssl::stream_base::client);    
    
        // Send the request.
    boost::asio::write(socket, request);
    
        // Read the response status line. The response streambuf will automatically
        // grow to accommodate the entire line. The growth may be limited by passing
        // a maximum size to the streambuf constructor.
    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");
 
    // Check that response is OK.
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
      std::cout << "Invalid response\n";
    }
    if (status_code != 200)
    {
      std::cout << "Response returned with status code " << status_message << "\n";
    }
 
    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n");
 
    // Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r")
      std::cout << header << "\n";
    std::cout << "\n";
 
    // Write whatever content we already have to output.
    if (response.size() > 0)
      std::cout << &response;
 
    // Read until EOF, writing data to output as we go.
    boost::system::error_code error;
    while (boost::asio::read(socket, response,
          boost::asio::transfer_at_least(1), error))
      std::cout << &response;
}

void Network::ConnectBot(const Bot* bot)
{
    bots.push_back(bot);
}

void Network::StartPolling()
{
  SendGETRequest();
  
}