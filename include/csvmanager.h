#ifndef CSVMANAGER_H
#define CSVMANAGER_H

#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stack>

enum class SimpleType {
    STRING, NUMBER
};

struct Field {
    int char_count;
    std::string str;
    SimpleType stype;

    Field(const std::string& s);
    std::string hex() const;
};

struct Row {
    long char_count;
    int col_count;
    std::string str;
    std::vector<Field> fields;

    Row(const std::string& s);
};

class CsvManager
{
public:
    char sep;
    char quote;
    char newline;
    CsvManager();
    void open_file(const std::string& path);
    std::string next_row();
    std::string back_row();
    long get_row() {return row;}

private:
    std::string filename;
    std::stack<long long> last_idx;
    long row;
    std::unique_ptr<std::fstream> fin;
    std::unique_ptr<std::fstream> ftmp;
};

#endif // CSVMANAGER_H
