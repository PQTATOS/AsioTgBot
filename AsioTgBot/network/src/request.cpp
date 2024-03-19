#include "request.hpp"

namespace tg
{

  //FIX
  std::string request::to_string() const
  {
      std::string out_request;

      if (this->type != req_types::GET)
      {
        out_request += "POST /bot6329343331:AAER5UjGOmLh_G7wBbEG7VNtBvyYQBD84Z0/sendMessage HTTP/1.1\r\nHost: api.telegram.org\r\nUser-Agent: custom-client\r\nConnection: keep-alive\r\n";

          out_request += "Content-Type: application/json\r\nContent-Length: 56\r\n\r\n";
          out_request += "{\"chat_id\":292732632,\"parse_mode\":\"HTML\",\"text\":\"Hello\"}";

          std::cout << out_request << '\n';
          return out_request;
      }

      out_request.reserve(72 + this->url.size() + this->host.size());
      out_request += "GET " + (this->url) + " HTTP/1.1\r\n"
                         + "Host: " + (this->host) + "\r\n"
                         + "User-Agent: custom-client\r\n"
                         + "Connection: close\r\n\r\n";

      return out_request;
  }

  void request::add_body(std::string& str)
  {
    this->body = str;
  }

  request::request(const req_types type_, const std::string& url_, const std::string& host_)
    : url(url_), host(host_), type(type_) {};
}