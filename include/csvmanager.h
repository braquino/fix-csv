#ifndef CSVMANAGER_H
#define CSVMANAGER_H

#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stack>
#include <regex>
#include <filesystem>
#include <map>

enum class SimpleType {
    EMPTY, STRING, NUMBER, INTEGER, NONE
};

struct Field {
    uint16_t char_count;
    std::string str;

    Field(const std::string& s, const char& quote = '"');
    std::string hex() const;
    SimpleType stype() const;
    std::string stype_str() const;
    static std::string stype_to_string(SimpleType t);
    static SimpleType string_to_stype(const std::string& type);
    bool quote_error() const;
    std::vector<unsigned char> non_print_char() const;
    char quote;
};

struct Row {
    uint32_t char_count;
    uint16_t col_count;
    std::string str;
    std::vector<Field> fields;
    std::vector<std::string> error_state;

    Row(const std::string& s = "", char sep = ',', char quote = '"', char newline = '\n');
    bool quote_error();
    bool field_count_error(uint16_t header_count);
    bool non_print_char_error();
    bool check_field_type(int field_idx, SimpleType t);
};

class CsvManager
{
public:
    char sep;
    char quote;
    char newline;
    CsvManager();
    void open_file(const std::string& path);
    long curr_row_num() {return row;}
    int get_header_count() {return header_count;}
    Row next_row();
    Row back_row();
    Row curr_row() {return last_row;}
    Row get_header() {return header;}
    Row next_error(
        bool field_count = true, bool bad_quote = false, 
        bool unprint_char = false, bool check_type = false,
        int fld_t_idx = -1, SimpleType fld_t = SimpleType::NONE
    );
    void replace_row(int64_t row_number, const std::string& content);
    int64_t count_rows();
    int64_t get_position();
    int64_t get_size() {return size;}
    void save_file(const std::string& out_path);
    void reset();
    bool eof() { return _eof; }

private:
    int header_count;
    int64_t size;
    std::map<long, std::string> replaced_rows;
    std::string filename;
    std::stack<int64_t> last_idx;
    long row;
    std::unique_ptr<std::fstream> fin;
    std::string next_row_str();
    std::string back_row_str();
    Row last_row;
    Row header;
    bool _eof;
};

#endif // CSVMANAGER_H
