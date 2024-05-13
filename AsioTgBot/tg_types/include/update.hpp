#pragma once

enum class update_types
{
    Message
};

class update
{
public:
    virtual ~update() {};
    virtual update_types get_type() const {};
};