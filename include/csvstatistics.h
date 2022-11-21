#ifndef CSVSTATISTICS_H
#define CSVSTATISTICS_H

#include <string>
#include <map>
#include <tuple>
#include "csvmanager.h"


struct FieldReport
{

};

struct RowsReport
{

};

class DiscreteCounter
{
public:
    DiscreteCounter(const std::string& name) : name{name} {}
    const std::string name;

    void add_item(int value);
    double get_percetile(double percent) const;
    double get_mean() const;
    std::tuple<int, double> get_mode() const;
private:
    std::map<int, int> counter;
};

class FieldStatistic
{
public:
    FieldStatistic(const std::string& name) : name{name}, types{"type"}, char_count{"char_count"} {}
    const std::string name;

    void add_item(Field f);
    FieldReport get_report() const;
private:
    DiscreteCounter types;
    DiscreteCounter char_count;
};


class CsvStatistics
{
public:
    CsvStatistics(Row& header) : fields_count{"fields_count"} {} //TODO: move to cpp and initialize the header
    void add_row(const Row& r);
    FieldReport get_field_report(const std::string& field_name) const;
    RowsReport get_rows_report() const;
private:
    std::map<std::string, FieldStatistic> fields;
    DiscreteCounter fields_count;
    const std::vector<std::string> header;
};

#endif // CSVSTATISTICS_H
