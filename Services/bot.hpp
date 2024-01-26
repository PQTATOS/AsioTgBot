#include <string>

enum class ParseMode: int
{
    MarkdownV2,
    Markdown,
    HTML
}

class Bot
{
private:
    std::string bot_token;
    ParseMode parse_mode;
public:
    Bot(std::string token, ParseMode parse_m);
    void StartPolling();
    void getUpdates(int offset, int limit, int timeout);
};
