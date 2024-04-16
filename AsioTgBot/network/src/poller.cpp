#include <poller.hpp>

using boost::asio::ip::tcp;

poller::poller()
{
    this->ssl_ctx = new boost::asio::ssl::context(boost::asio::ssl::context::method::sslv23_client);
    std::cout << "Poller created\n";
};

void poller::handle_connect(
    const boost::system::error_code &e,
    size_t sock_id
) {
    std::cout << "Connect started\n";

    tcp::resolver resolver(this->service);
    tcp::resolver::query query("api.telegram.org" , "https");

    utility::socket& sock = this->opened_sockets[sock_id];

    boost::asio::async_connect(
        sock.get_lowest_layer(),
        resolver.resolve(query),
        boost::bind(
            &handle_handshake,
            this,
            boost::asio::placeholders::error,
            sock_id
        )
    );
};

void poller::handle_handshake(
    const boost::system::error_code &error,
    size_t sock_id
) {
    if (error)
    {
        std::cout << "\t Connect error: " << error.message() << '\n';
        return;
    }
    std::cout << "\tSocket connected\n";

    utility::socket& sock = this->opened_sockets[sock_id];

    sock.get_socket().async_handshake(
        boost::asio::ssl::stream_base::client,
        boost::bind(
            &send,
            this,
            boost::asio::placeholders::error,
            sock_id
        )
    );
};

void poller::send(
    const boost::system::error_code &error,
    size_t sock_id
) {
    if (error)
    {
        std::cout << "\tHanshake error: " << error.message() << '\n';
        return;
    }
    std::cout << "\tSocket make ssl handshake\n";

    utility::socket& sock = this->opened_sockets[sock_id];
    const tg::request* req =  this->requests_to_process.front();
    this->requests_to_process.pop();

    boost::asio::async_write(
        sock.get_socket(),
        boost::asio::buffer(
            req->to_string()
        ),
        boost::bind(
            &recive_status,
            this,
            boost::asio::placeholders::error,
            sock_id
        )
    );
};

void poller::recive_status(
    const boost::system::error_code &e,
    size_t sock_id
) {
    if (e)
    {
        std::cout << "\tSend error: " << e.message() << '\n';
        return;
    }
    std::cout << "\tSocket send msg\n";

    utility::socket& sock = this->opened_sockets[sock_id];

    boost::asio::async_read_until(
        sock.get_socket(),
        sock.get_streambuf(),
        "\r\n",
        boost::bind (
            &recive_headers,
            this,
            boost::asio::placeholders::error,
            sock_id
        )
    );
};

void poller::recive_headers(
    const boost::system::error_code &e,
    size_t sock_id
) {
    if (e)
    {
        std::cout << "\tRead status error: " << e.message() << '\n';
        return;
    }
    std::cout << "\tSocket recive status\n";

    utility::socket& sock = this->opened_sockets[sock_id];
    sock.get_response();
    std::istream responce_stream(&sock.get_streambuf());

    std::string http_ver;
    responce_stream >> http_ver >> sock.get_response().status_code;

    std::getline(responce_stream, sock.get_response().status_msg);
    std::cout << '\t' << sock.get_response().status_code << " " << sock.get_response().status_msg << '\n';
    boost::asio::async_read_until(
        sock.get_socket(),
        sock.get_streambuf(),
        "\r\n\r\n",
        boost::bind(
            &recive_body,
            this,
            boost::asio::placeholders::error,
            sock_id
        )
    );

};

void poller::recive_body(
    const boost::system::error_code &e,
    size_t sock_id
) {
    if (e)
    {
        std::cout << "\tRead headers error: " << e.message() << '\n';
        return;
    }
    std::cout << "\tSocket recive headers\n";

    utility::socket& sock = this->opened_sockets[sock_id];

    std::string header;
    std::istream responce_stream(&sock.get_streambuf());
    while (std::getline(responce_stream, header) && header != "\r")
    {
        size_t delimetr_pos = header.find(": ");
        std::string key = header.substr(0, delimetr_pos),
                    value = header.substr(delimetr_pos+2, header.length());
        sock.get_response().headers[key] = value;
        std::cout << "\t--" << header << '\n';
    }

    boost::asio::async_read(
        sock.get_socket(),
        sock.get_streambuf(),
        boost::asio::transfer_at_least(1),
        boost::bind(
            &handle_body,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred,
            sock_id
        )
    );
};

void poller::handle_body(
    const boost::system::error_code &e,
    std::size_t bytes_transferred,
    size_t sock_id
) {
    if (e && e != boost::asio::error::eof)
    {
        std::cout << "\tRead body error: " << e.message() << '\n';
        return;
    }

    utility::socket& sock = this->opened_sockets[sock_id];
    if (e != boost::asio::error::eof)
    {
        std::cout << "\t[Poller] socket " << sock_id << " recive body part\n\t stream size is " << sock.get_streambuf().size()  << '\n';

        std::istream responce_stream(&sock.get_streambuf());
        std::string body_part;
        responce_stream >> body_part;
        sock.get_response().body += body_part;
        std::cout << body_part;

        boost::asio::async_read(
            sock.get_socket(),
            sock.get_streambuf(),
            boost::asio::transfer_at_least(1),
            boost::bind(
                &handle_body,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred,
                sock_id
            )
        );
    }
    else {
        std::cout << "\t[Poller] socket " << sock_id << " recive full body\n\t stream size is " 
        << sock.get_streambuf().size() << '\n';

        std::istream responce_stream(&sock.get_streambuf());
        std::string body_part;
        responce_stream >> body_part;
        sock.get_response().body += body_part;

        std::cout << sock.get_response().body << '\n';

        this->responces_recived.push(sock.release_response());
    }
};

tg::response* poller::get_responce()
{
    tg::response* resp = this->responces_recived.front();
    this->responces_recived.pop();
    return resp;
};

void poller::add_request(const tg::request* req) {
    std::cout << "added to queue\n";
    this->requests_to_process.push(req); 
};

bool poller::is_no_requests() {
    return this->requests_to_process.empty(); 
};

bool poller::is_no_responce()
{
    return this->responces_recived.empty();
}

poller::~poller() 
{
    delete this->ssl_ctx;
};

void poller::run() {
    std::cout << "Poller started with queue size: " << this->requests_to_process.size() << '\n';

    if (this->requests_to_process.empty())
        return;

    for (size_t i = 0; i < this->requests_to_process.size(); ++i)
    {
        this->opened_sockets.emplace_back(this->service, *this->ssl_ctx);
        handle_connect(boost::system::error_code(), i);
    }
    this->service.run();

    std::cout << "Poller finished run\n";
    this->opened_sockets.clear();
    this->service.reset();
};