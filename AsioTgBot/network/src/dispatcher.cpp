#include "dispatcher.hpp"

dispatcher::dispatcher()
{
    bot = nullptr;
    net_poller = new poller();
    std::cout << "Dispatcher created\n";
};

dispatcher::~dispatcher()
{
    delete net_poller;
};

void dispatcher::add_bot(const tgbot *bot_)
{
    bot = bot_;
};

void dispatcher::start()
{
    while (true)
    {
        if (!this->updates_to_process.empty())
            send_messages();
        else
            get_updates();

        this->net_poller->run();

        process_responces();
        std::cout << this->updates_to_process.size() << '\n';
    }
};

void dispatcher::process_responces()
{
    std::cout << "processing responce \n";
    int x;
    std::cin >> x;
    while(!this->net_poller->is_no_responce())
    {
        tg::response* resp = this->net_poller->get_responce();
        update* upd = convert_str(resp->body);
        this->updates_to_process.push(upd);
    }
}

//fix
update* dispatcher::convert_str(std::string body)
{
    size_t chat_id_pos_start = body.find("\"chat\":{\"id\":")+13,
           chat_id_pos_end = body.find(",", chat_id_pos_start);
    std::string chat_id = body.substr(chat_id_pos_start, chat_id_pos_end -chat_id_pos_start);

    std::cout << "\t\t" << chat_id << '\n';

    size_t text_id_pos_start = body.find("\"text\":\"")+8,
           text_id_pos_end = body.find("\"", text_id_pos_start);
    std::string text = body.substr(text_id_pos_start, text_id_pos_end-text_id_pos_start);

    std::cout << "\t\t" << text << '\n';

    message* msg = new message(chat_id, text);
    
    return msg;
}

void dispatcher::get_updates()
{
    std::cout << "MAKE update req?\n";
        int x = 0;
        std::cin >> x;
        tg::request* req = new tg::request(
            tg::req_types::GET,
            "/bot6329343331:AAER5UjGOmLh_G7wBbEG7VNtBvyYQBD84Z0/getUpdates",
            "api.telegram.org"
        );

        if (x) this->net_poller->add_request(req);
};

void dispatcher::send_messages()
{
    std::cout << "MAKE update send req?\n";
        int x = 0;
        std::cin >> x;
        tg::request* req = new tg::request(
            tg::req_types::POST,
            "/bot6329343331:AAER5UjGOmLh_G7wBbEG7VNtBvyYQBD84Z0/sendMessage",
            "api.telegram.org"
        );
        std::string body = "{\"chat_id\":292732632,\"parse_mode\":\"HTML\",\"text\":\"Hello\n<tg-spoiler>World</tg-spoiler>\"}";
        req->add_body(body);

        if (x) this->net_poller->add_request(req);
};
