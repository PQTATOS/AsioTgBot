#include <poller.hpp>

using boost::asio::ip::tcp;

poller::poller()
{
    this->ssl_ctx = new boost::asio::ssl::context(boost::asio::ssl::context::method::sslv23_client);
    std::cout << "Poller created\n";
};

void poller::handle_connect(const boost::system::error_code &e)
{
    std::cout << "Connect started\n";
    boost::asio::ssl::stream<tcp::socket>* socket = new boost::asio::ssl::stream<tcp::socket>(service, *ssl_ctx);
    tcp::resolver resolver(this->service);
    tcp::resolver::query query("api.telegram.org" , "https");

    boost::asio::async_connect(
        socket->lowest_layer(),
        resolver.resolve(query),
        boost::bind(
            &handle_handshake,
            this,
            boost::asio::placeholders::error,
            socket
            )
        );
};

void poller::handle_handshake(
    const boost::system::error_code &error,
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> *sock
) {
    if (error)
    {
        std::cout << "\t Connect error: " << error.message() << '\n';
        return;
    }
    std::cout << "\tSocket connected\n";

    sock->async_handshake(
        boost::asio::ssl::stream_base::client,
        boost::bind(
            &send,
            this,
            boost::asio::placeholders::error,
            sock
        )
    );
};

void poller::send(
    const boost::system::error_code &error,
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> *sock
) {
    if (error)
    {
        std::cout << "\tHanshake error: " << error.message() << '\n';
        return;
    }

    std::cout << "\tSocket make ssl handshake\n";
    const tg::request* req =  this->requests_to_process.front();
    this->requests_to_process.pop();

    boost::asio::async_write(
        *sock,
        boost::asio::buffer(
            req->to_string()
        ),
        boost::bind(
            &recive_status,
            this,
            boost::asio::placeholders::error,
            new tg::response(),
            sock
            )
        );
};

void poller::recive_status(
    const boost::system::error_code &e,
    tg::response* resp,
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> *sock
) {
    if (e)
    {
        std::cout << "\tSend error: " << e.message() << '\n';
        return;
    }
    std::cout << "\tSocket send msg\n";

    boost::asio::async_read_until(
        *sock,
        resp->raw_responce,
        "\r\n",
        boost::bind (
            &recive_headers,
            this,
            boost::asio::placeholders::error,
            resp,
            sock
        )
    );
};

void poller::recive_headers(
    const boost::system::error_code &e,
    tg::response* resp,
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> *sock
) {
    if (e)
    {
        std::cout << "\tRead status error: " << e.message() << '\n';
        return;
    }
    std::cout << "\tSocket recive status\n";
    std::istream responce_stream(&resp->raw_responce);

    std::string http_ver;
    responce_stream >> http_ver >> resp->status_code;

    std::getline(responce_stream, resp->status_msg);
    std::cout << '\t' << resp->status_code << " " << resp->status_msg << '\n';

    boost::asio::async_read_until(
        *sock,
        resp->raw_responce,
        "\r\n\r\n",
        boost::bind(
            &recive_body,
            this,
            boost::asio::placeholders::error,
            resp,
            sock
        )
    );

};

void poller::recive_body(
    const boost::system::error_code &e,
    tg::response* resp,
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> *sock
) {
    if (e)
    {
        std::cout << "\tRead headers error: " << e.message() << '\n';
        return;
    }
    std::cout << "\tSocket recive headers\n";

    std::string header;
    std::istream responce_stream(&resp->raw_responce);
    while (std::getline(responce_stream, header) && header != "\r")
    {
        size_t delimetr_pos = header.find(": ");
        std::string key = header.substr(0, delimetr_pos),
                    value = header.substr(delimetr_pos+2, header.length());
        resp->headers[key] = value;
        std::cout << "\t--" << header << '\n';
    }

    std::cout << "\t\t" << std::stoi(resp->headers["Content-Length"]) << '\n';
    boost::asio::async_read(
        *sock,
        resp->raw_responce,
        boost::asio::transfer_at_least(1),
        boost::bind(
            &handle_body,
            this,
            boost::asio::placeholders::error,
            resp,
            sock
        )
    );
};

void poller::handle_body(
    const boost::system::error_code &e,
    tg::response* resp,
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> *sock
) {
    if (e && e != boost::asio::error::eof)
    {
        std::cout << "\tRead body error: " << e.message() << '\n';
        return;
    }
    if (e != boost::asio::error::eof)
    {
        std::cout << "\tSocket recive body\n";

        std::istream responce_stream(&resp->raw_responce);
        std::string tmp;
        responce_stream >> tmp;
        resp->body += tmp;

        std::cout << resp->body << '\n';

        boost::asio::async_read(
            *sock,
            resp->raw_responce,
            boost::asio::transfer_at_least(1),
            boost::bind(
                &handle_body,
                this,
                boost::asio::placeholders::error,
                resp,
                sock
            )
        );
    }
    else {
        std::cout << "\tSocket recive body\n";

        std::istream responce_stream(&resp->raw_responce);
        responce_stream >> resp->body;

        std::cout << resp->body << '\n';

        delete sock;
        this->responces_recived.push(resp);
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

    handle_connect(boost::system::error_code());
    this->service.run();
    this->service.reset();

    std::cout << "Poller finished run\n";
};