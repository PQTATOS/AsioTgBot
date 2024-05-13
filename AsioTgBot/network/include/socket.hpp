#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <request.hpp>
#include <response.hpp>

namespace utility
{
    class socket
    {
    public:
        socket(boost::asio::io_service& service ,boost::asio::ssl::context& ssl_ctx);
        ~socket();

        socket(socket&& other);
        socket& operator=(socket&& other);

        boost::asio::ip::tcp::socket::lowest_layer_type& get_lowest_layer();
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& get_socket();

        boost::asio::streambuf& get_streambuf();
        void add_bytes(size_t bytes);

        tg::response& get_response();
        tg::response* release_response();
    private:
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket>* base_socket;
        boost::asio::streambuf* stream;
        size_t bytes_transfered;

        tg::response* response;
    };
}