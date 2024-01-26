#include <string>

enum class ChatType : int
{
    Private,
    Groupe,
    SuperGroupe,
    Channel
}

class Chat
{
private:
    long long id;
    ChatType type;

    std::string title;
    std::string username;
    std::string first_name;
    std::string last_name;

    bool is_forum;
}