#include "request.hpp"
#include <boost/format.hpp>
#include <iostream>

Request::Request(Bot* bot, const std::string& _url)
{
    boost::format frm_str("/bot%1%/%2%");
    frm_str % bot->Token() % _url;
    url = frm_str.str();

    body_size = 0;
}

void Request::AddUrlParams(std::vector<std::pair<std::string, std::string>>& params)
{
    url.push_back('?');
    
    for (auto & param: params)
    {
        url.append(str(boost::format("%1%=%2%") % param.first % param.second));
        url.push_back('&');
    }
    std::cout << url << '\n';
}

bool Request::IsEmptyBody()
{
    return (bool)body_size;
}