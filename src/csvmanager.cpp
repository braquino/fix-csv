#include "csvmanager.h"
#include <sstream>
#include <fmt/core.h>

CsvManager::CsvManager() : last_idx{{0}}, sep{','}, quote{'"'}, newline{'\n'}, row{0}, header_count{0}, size{0}, last_row{""}, _eof{false}
{

}

void CsvManager::open_file(const std::string& path)
{
    this->filename = path;
    size = std::filesystem::file_size(path);
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

long long CsvManager::get_position()
{
    if (this->fin != nullptr && this->fin->is_open())
        return this->fin->tellg();
    else
        return 0;
}

std::string CsvManager::next_row_str()
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
            ss << c;
            if (!esc && c == newline) break;
        }
        std::string str_row = ss.str();
        if (!str_row.empty())
        {
            this->last_idx.push(idx);
            this->row++;
            return str_row;
        }

    }
    return "";
}

std::string CsvManager::back_row_str()
{
    if (this->fin != nullptr && this->fin->is_open() && this->row > 1)
    {
        auto idx = this->last_idx.top();
        this->last_idx.pop();
        this->row--;
        this->last_idx.pop();
        this->row--;
        this->fin->seekg(this->last_idx.top() - idx, this->fin->cur);
        return this->next_row_str();
    }
    return "";
}

Row CsvManager::next_row()
{
    std::string r_str = next_row_str();
    if (!r_str.empty())
    {
        auto r = Row{r_str, sep, quote, newline};
        last_row = r;
        if (this->row == 1) this->header_count = r.col_count;
        return r;
    }
    else
    {
        this->_eof = true;
        return this->curr_row();
    }
}

Row CsvManager::back_row()
{
    this->_eof = false;
    return Row{back_row_str(), sep, quote, newline};
}

Row CsvManager::next_error()
{
    if (this->row == 0) this->next_row();
    long _lrow = this->row;
    while(true)
    {
        Row r = this->next_row();
        if (this->row == _lrow) return r;
        if (r.col_count != this->header_count || r.error_state) return r;
        _lrow = this->row;
    }
}

long CsvManager::count_rows()
{
    long count = 0;
    while(this->fin)
    {
        if (this->next_row_str().empty()) break;
        count++;
    }
    return count;
}

void CsvManager::replace_row(long row_number, const std::string& content)
{
    this->replaced_rows[row_number] = content;
}

void CsvManager::reset()
{
    this->row = 0;
    this->last_idx = std::stack<long long>{{0}};
    if (this->fin != nullptr && this->fin->is_open()) this->fin->close();
    if (!this->filename.empty()) this->fin->open(this->filename);
}

void CsvManager::save_file(const std::string& out_path)
{
    this->reset();
    std::fstream out_f;
    out_f.open(out_path, std::ios::out);
    if(!out_f.is_open())
        throw std::runtime_error("Error trying to save file: " + out_path);

    while(!this->fin->eof())
    {
        std::string r = this->next_row_str();
        if (r.empty()) break;
        if (this->replaced_rows.find(this->row) != this->replaced_rows.end())
        {
            std::string& rep_row = this->replaced_rows[this->row];
            if (rep_row != "" && rep_row[0] != newline)
            {
                out_f << rep_row;
                if (rep_row[rep_row.size() - 1] != newline)
                    out_f << newline;
            }
        }
        else
        {
            out_f << r;
            if (r[r.size() - 1] != newline)
                out_f << newline;
        }
    }
    out_f.close();
}

Field::Field(const std::string& s, const char& quote) : char_count{(uint16_t)s.size()}, str{s}, quote{quote}
{

}

std::string Field::hex() const
{
    std::stringstream ss;
    for (int i=0; i < str.size(); i++)
    {
        ss << fmt::format("{:x} ", (unsigned char)str[i]);
    }
    return ss.str();
}

SimpleType is_number(const std::string& s, char quote)
{
    int skip_quote = (s[0] == quote && s[s.size() - 1] == quote) ? 1 : 0;
    int dec = 0;
    for (int i = skip_quote; i < s.size() - skip_quote; i++)
    {
        uint8_t c = s[i];
        if (c == 0x2c || c == 0x2e) dec++;
        if (c > 0x39 || c < 0x2c || dec > 1 || c == 0x2d || c == 0x2f) return SimpleType::NONE;
    }
    if (dec == 1) return SimpleType::NUMBER;
    else return SimpleType::INTEGER;
}

SimpleType Field::stype() const
{
    if (str.size() == 0) return SimpleType::EMPTY;
    SimpleType test_num = is_number(this->str, this->quote);
    if (test_num != SimpleType::NONE) return test_num;
    return SimpleType::STRING;
}

std::string Field::stype_to_string(SimpleType t)
{
    if (t == SimpleType::INTEGER) return "Integer";
    if (t == SimpleType::NUMBER) return "Number";
    if (t == SimpleType::EMPTY) return "Empty";
    else return "String";
}

std::string Field::stype_str() const
{
    return this->stype_to_string(this->stype());
}

bool Field::quote_error() const
{
    bool err = false;
    int count_quote = 0;
    for (int i=0; i < this->str.size(); i++)
    {
        const char& c{this->str[i]};
        bool open_close = ((i == 0 || i == this->str.size() - 1) && c == quote);
        if (!open_close)
        {
            if (c == quote)
                count_quote++;
            else
            {
                err = err || (count_quote % 2 != 0);
                count_quote = 0;
            }
        }
    }
    return err;
}

Row::Row(const std::string& s, char sep, char quote, char newline) : str{s}, char_count{(uint32_t)s.size()}
{
    error_state = false;
    // TODO: Add error state true when an escape char is found oddly in the midle of the string
    std::stringstream ss_f;
    bool esc = false;
    char c;
    for (int i=0; i < s.size(); i++)
    {
        c = s[i];
        if (c == quote) esc = !esc;
        if ((c == sep || c == newline) && !esc)
        {
            fields.push_back(Field{ss_f.str()});
            ss_f.str(std::string());
            ss_f.clear();
        }
        else
        {
            ss_f << c;
            if (i + 1 == s.size()) fields.push_back(Field{ss_f.str(), quote});
        }
    }
    col_count = fields.size();
}
