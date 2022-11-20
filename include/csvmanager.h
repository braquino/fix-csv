#ifndef CSVMANAGER_H
#define CSVMANAGER_H

#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stack>
#include <regex>
#include <filesystem>

enum class SimpleType {
    EMPTY, STRING, NUMBER, INTEGER
};

struct Field {
    uint16_t char_count;
    std::string str;

    Field(const std::string& s);
    std::string hex() const;
    SimpleType stype() const;
    std::string stype_str() const;
    bool quote_error(const char& quote = '"') const;
};

struct Row {
    uint32_t char_count;
    uint16_t col_count;
    std::string str;
    std::vector<Field> fields;
    bool error_state;

    Row(const std::string& s, char sep = ',', char quote = '"', char newline = '\n');
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
    Row next_row();
    Row back_row();
    Row curr_row() {return last_row;}
    Row next_error();
    void replace_row(long row_number, const std::string& content);
    long count_rows();
    long long get_position();
    long long get_size() {return size;}
    void save_file(const std::string& out_path);
    void reset();

private:
    int header_count;
    long long size;
    std::map<long, std::string> replaced_rows;
    std::string filename;
    std::stack<long long> last_idx;
    long row;
    std::unique_ptr<std::fstream> fin;
    std::string next_row_str();
    std::string back_row_str();
    Row last_row;
};

#endif // CSVMANAGER_H
