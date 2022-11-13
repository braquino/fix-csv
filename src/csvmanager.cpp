#include "csvmanager.h"
#include <sstream>
#include <fmt/core.h>

CsvManager::CsvManager() : last_idx{{0}}, sep{','}, quote{'"'}, newline{'\n'}, row{0}
{

}

void CsvManager::open_file(const std::string& path)
{
    if (this->fin == nullptr)
    {
        this->fin.reset(new std::fstream{path.c_str(), std::ios::in});
    }
    else
    {
        if (this->fin->is_open()) this->fin->close();
        this->fin->open(path.c_str(), std::ios::in);
    }
}

std::string CsvManager::next_row()
{
    if (this->fin != nullptr && this->fin->is_open())
    {
        std::stringstream ss;

        auto idx = this->last_idx.top();

        bool esc{false};
        char c;
        while (this->fin->get(c))
        {
            idx++;
            if (c == quote) esc = !esc;
            if (!esc && c == newline) break;
            ss << c;
        }
        this->last_idx.push(idx);
        this->row++;
        return ss.str();
    }
    return "ERROR: The file is not open";
}

std::string CsvManager::back_row()
{
    if (this->fin != nullptr && this->fin->is_open() and this->row > 1)
    {
        auto idx = this->last_idx.top();
        this->last_idx.pop();
        this->row--;
        this->last_idx.pop();
        this->row--;
        this->fin->seekg(this->last_idx.top() - idx, this->fin->cur);
        return this->next_row();
    }
    return "ERROR: The file is not open";
}

Field::Field(const std::string& s)
{
    str = s;
    char_count = s.size();
}

std::string Field::hex() const
{
    std::stringstream ss;
    for (int i=0; i < str.size(); i++)
    {
        ss << fmt::format("<{:#x}>", (unsigned char)str[i]);
    }
    return ss.str();
}
