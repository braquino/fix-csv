#include "csvmanager.h"
#include <sstream>
#include <fmt/core.h>

CsvManager::CsvManager() : last_idx{{0}}, sep{','}, quote{'"'}, newline{'\n'}, row{0}, header_count{0}, size{0}
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
        while (this->fin->peek() != EOF)
        {
            this->fin->get(c);
            idx++;
            if (c == quote) esc = !esc;
            if (!esc && c == newline) break;
            ss << c;
        }
        std::string str_row = ss.str();
        if (str_row.size() > 1)
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
    auto r = Row{next_row_str(), sep, quote};
    if (this->row == 1) this->header_count = r.col_count;
    return r;
}

Row CsvManager::back_row()
{
    return Row{back_row_str(), sep, quote};
}

Row CsvManager::next_error()
{
    if (this->row == 0) this->next_row();
    while(true)
    {
        Row r = this->next_row();
        if (r.col_count != this->header_count || r.error_state) return r;
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
    std::fstream out_f{out_path, std::ios::out};
    while(this->fin)
    {
        std::string r = this->next_row_str();
        if (r.empty()) break;
        if (this->replaced_rows.find(this->row) != this->replaced_rows.end())
        {
            std::string& rep_row = this->replaced_rows[this->row];
            if (!rep_row.empty()) out_f << rep_row << std::endl;
        }
        else out_f << r << std::endl;
    }
    out_f.close();
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
        ss << fmt::format("{:x} ", (unsigned char)str[i]);
    }
    return ss.str();
}

SimpleType Field::stype() const
{
    if (str.size() == 0) return SimpleType::EMPTY;
    if (std::regex_match(str, std::regex("^[0-9]*$"))) return SimpleType::INTEGER;
    if (std::regex_match(str, std::regex("^([0-9]|[.]|,)*$"))) return SimpleType::NUMBER;
    else return SimpleType::STRING;
}

std::string Field::stype_str() const
{
    SimpleType t = this->stype();
    if (t == SimpleType::INTEGER) return "Integer";
    if (t == SimpleType::NUMBER) return "Number";
    if (t == SimpleType::EMPTY) return "Empty";
    else return "String";
}

Row::Row(const std::string& s, char sep, char quote) : str{s}, char_count{(uint32_t)s.size()}
{
    error_state = false;
    // TODO: Add error state true when an escape char is found oddly in the midle of the string
    std::stringstream ss_f;
    bool esc = false;
    for (const char& c : s)
    {
        if (c == quote) esc = !esc;
        if (c == sep && !esc)
        {
            fields.push_back(Field{ss_f.str()});
            ss_f.str(std::string());
            ss_f.clear();
        }
        else
        {
            ss_f << c;
        }
    }
    if (s.size() > 0) fields.push_back(Field{ss_f.str()});
    col_count = fields.size();
}
