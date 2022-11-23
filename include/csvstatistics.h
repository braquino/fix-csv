#ifndef CSVSTATISTICS_H
#define CSVSTATISTICS_H

#include <string>
#include <map>
#include <utility>
#include "csvmanager.h"


struct FieldReport
{
    std::string name;
    double char_count_mean;
    int char_count_p_95;
    int char_count_p_75;
    int char_count_p_50;
    int char_count_p_25;
    int char_count_p_05;
    SimpleType type_first;
    double perc_type_first;
    SimpleType type_second;
    double perc_type_second;
    SimpleType type_third;
    double perc_type_third;
};

struct RowsReport
{
    std::vector<std::string> header;
    long row_count;
    std::vector<FieldReport> field_statistic;
    int field_count_first;
    double perc_field_count_first;
    int field_count_second;
    double perc_field_count_second;
    int field_count_third;
    double perc_field_count_third;
};

class Counter
{
public:
    Counter(const std::string& name) : name{name}, _size{0} {}
    const std::string name;

    void add_item(int value);
    int get_percentile(double percent) const;
    int get_percentile(double percent, std::vector<std::pair<int, double>> dist) const;
    double get_mean() const;
    long size() const {return _size;}
    std::vector<std::pair<int, double>> get_distribution() const;
private:
    std::map<int, int> counter;
    long _size;
};

class FieldStatistic
{
public:
    FieldStatistic(const std::string& name) : name{name}, types{"type"}, char_count{"char_count"} {}
    const std::string name;

    void add_item(const Field& f);
    FieldReport get_report() const;
private:
    Counter types;
    Counter char_count;
};


class CsvStatistics
{
public:
    CsvStatistics(const Row& header);
    void add_row(const Row& r);
    FieldReport get_field_report(const std::string& field_name) const;
    RowsReport get_rows_report() const;
private:
    std::vector<std::unique_ptr<FieldStatistic>> fields;
    Counter fields_count;
    std::vector<std::string> header;
    long rows_count;
};

#endif // CSVSTATISTICS_H
