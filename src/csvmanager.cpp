#include "csvmanager.h"
#include <fmt/core.h>
#include <memory>
#include <sstream>

CsvManager::CsvManager() : last_idx{{0}}, sep{','}, quote{'"'}, newline{'\n'}, 
    row{0}, header_count{0}, size{0}, last_row{""}, _eof{false}, header{""}
{

}

void CsvManager::open_file(const std::string& path)
{
    this->filename = path;
    size = std::filesystem::file_size(path);
    if (this->fin == nullptr)
    {
        this->fin = std::make_unique<std::fstream>(path.c_str(), std::ios::in);
    }
    else
    {
        if (this->fin->is_open()) this->fin->close();
        this->fin->open(path.c_str(), std::ios::in);
    }
}

int64_t CsvManager::get_position()
{
    if (this->fin != nullptr && this->fin->is_open())
        return this->fin->tellg();
    return 0;
}

std::string CsvManager::next_row_str()
{
    if (this->fin != nullptr && this->fin->is_open())
    {
        std::stringstream ss;

        auto idx = this->last_idx.top();

        bool esc{false};
        char c = 0;
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
    const std::string r_str = next_row_str();
    if (!r_str.empty())
    {
        auto r = Row{r_str, sep, quote, newline};
        last_row = r;
        if (this->row == 1) 
        {
            this->header = r;
            this->header_count = r.col_count;
        }
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

Row CsvManager::next_error(bool field_count, bool bad_quote, bool unprint_char, bool check_type, int fld_t_idx, SimpleType fld_t)
{
    if (this->row == 0) this->next_row();
    int64_t lastrow = this->row;
    while(true)
    {
        Row r = this->next_row();
        if (this->row == lastrow)
            return r;
        if (field_count)
            r.field_count_error(this->header_count);
        if (bad_quote)
            r.quote_error();
        if (unprint_char)
            r.non_print_char_error();
        if (check_type && fld_t_idx > -1)
            r.check_field_type(fld_t_idx, fld_t);
        if (!r.error_state.empty())
            return r;

        lastrow = this->row;
    }
}

int64_t CsvManager::count_rows()
{
    int64_t count = 0;
    while(this->fin)
    {
        if (this->next_row_str().empty()) break;
        count++;
    }
    return count;
}

void CsvManager::replace_row(int64_t row_number, const std::string& content)
{
    this->replaced_rows[row_number] = content;
}

void CsvManager::reset()
{
    this->row = 0;
    this->last_idx = std::stack<int64_t>{{0}};
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
            if (!rep_row.empty() && rep_row[0] != newline)
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
    for (const char& c : str)
    {
        ss << fmt::format("{:02x} ", (unsigned char)c);
    }
    return ss.str();
}

std::vector<unsigned char> Field::non_print_char() const
{
    std::vector<unsigned char> result;
    for (const unsigned char c : str)
    {
        if (c < 0x20 && c != 0x10 && c != 0x0d)
            result.push_back(c);
    }
    return result;
}

SimpleType is_number(const std::string& s, char quote)
{
    const int skip_quote = (s[0] == quote && s[s.size() - 1] == quote) ? 1 : 0;
    int dec = 0;
    const int sign = (int)(s[skip_quote] == 0x2b || s[skip_quote] == 0x2d);
    for (int i = sign + skip_quote; i < s.size() - skip_quote; i++)
    {
        const uint8_t c = s[i];
        if (c == 0x2c || c == 0x2e) dec++;
        if (c > 0x39 || c < 0x2c || dec > 1 || c == 0x2d || c == 0x2f) return SimpleType::NONE;
    }
    if (dec == 1) return SimpleType::NUMBER;
    else return SimpleType::INTEGER;
}

SimpleType Field::stype() const
{
    if (str.empty()) return SimpleType::EMPTY;
    const SimpleType test_num = is_number(this->str, this->quote);
    if (test_num != SimpleType::NONE) return test_num;
    return SimpleType::STRING;
}

std::string Field::stype_to_string(SimpleType t)
{
    if (t == SimpleType::INTEGER) return "Integer";
    if (t == SimpleType::NUMBER) return "Number";
    if (t == SimpleType::EMPTY) return "Empty";
    if (t == SimpleType::STRING) return "String";
    else return "None";
}

SimpleType Field::string_to_stype(const std::string& type)
{
    std::stringstream ss;
    for (auto c : type)
        ss << (char)std::tolower(c);
    const std::string t = ss.str();
    if (t == "integer") return SimpleType::INTEGER;
    if (t == "number") return SimpleType::NUMBER;
    if (t == "empty") return SimpleType::EMPTY;
    if (t == "string") return SimpleType::STRING;
    else return SimpleType::NONE;
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
        const bool open_close = ((i == 0 || i == this->str.size() - 1) && c == quote);
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
    std::stringstream ss_f;
    bool esc = false;
    char c = 0;
    for (int i=0; i < s.size(); i++)
    {
        c = s[i];
        if (c == quote) esc = !esc;
        if ((c == sep || c == newline) && !esc)
        {
            fields.emplace_back(std::move(Field{ss_f.str()}));
            ss_f.str(std::string());
            ss_f.clear();
        }
        else
        {
            ss_f << c;
            if (i + 1 == s.size()) fields.emplace_back(std::move(Field{ss_f.str(), quote}));
        }
    }
    col_count = fields.size();
}

bool Row::quote_error()
{
    int i = 0;
    for (const Field& f : this->fields)
    {
        if (f.quote_error())
        {
            this->error_state.push_back(fmt::format("QUOTE_ERROR: field {}, string: {}", i, f.str));
        }
        i++;
    }
    return (!this->error_state.empty());
}

bool Row::field_count_error(uint16_t header_count)
{
    if (this->col_count != header_count)
    {
        this->error_state.push_back(fmt::format("FIELD_COUNT_ERROR: this row has {} fields and header {} fields", this->col_count, header_count));
        return true;
    }
    return (!this->error_state.empty());
}

bool Row::non_print_char_error()
{
    int i = 0;
    for (const Field& f : this->fields)
    {
        auto unprint_chars = f.non_print_char();
        if (!unprint_chars.empty())
        {
            std::ostringstream ss;
            ss << "NON_PRINTABLE_CHAR_ERROR: on field "<< i << " the following chars are non printable:";
            for (auto c : unprint_chars)
                ss << fmt::format(" 0x{:02x}", c);
            this->error_state.push_back(ss.str());
        }
        i++;
    }
    return (!this->error_state.empty());
}

bool Row::check_field_type(int field_idx, SimpleType t)
{
    if (this->fields.size() > field_idx && this->fields[field_idx].stype() == t)
    {
        this->error_state.push_back(fmt::format("TYPE_FOUND: {} on field index {}", Field::stype_to_string(t), field_idx));
        return true;
    }
    else
        return false;
}
