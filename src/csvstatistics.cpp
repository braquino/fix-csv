#include "csvstatistics.h"

CsvStatistics::CsvStatistics(const Row& _header) : fields_count{"fields_count"}, rows_count{1}
{
    for (const Field& f : _header.fields)
    {
        this->header.push_back(f.str);
        this->fields.push_back(std::make_unique<FieldStatistic>(FieldStatistic{f.str}));
    }
}

void CsvStatistics::add_row(const Row& r)
{
    this->fields_count.add_item(r.col_count);
    this->rows_count++;
    for (int i=0; i < std::min(r.fields.size(), this->fields.size()); i++)
    {
        this->fields[i]->add_item(r.fields[i]);
    }
}

FieldReport CsvStatistics::get_field_report(const std::string& field_name) const
{
    int64_t field_idx{0};
    auto it = std::find(this->header.begin(), this->header.end(), field_name);
    if (it != this->header.end())
        field_idx = std::distance(it, this->header.begin());
    else
        throw std::runtime_error("Field: " + field_name + " not found on header");
    return this->fields[field_idx]->get_report();
}

RowsReport CsvStatistics::get_rows_report() const
{
    auto dist = this->fields_count.get_distribution();
    const int f = (dist.size() > 0) ? dist[0].first : 0;
    const double fp = (dist.size() > 0) ? dist[0].second : 0.0;
    const int s = (dist.size() > 1) ? dist[1].first : 0;
    const double sp = (dist.size() > 1) ? dist[1].second : 0.0;
    const int t = (dist.size() > 2) ? dist[2].first : 0;
    const double tp = (dist.size() > 2) ? dist[2].second : 0.0;
    RowsReport result;
    result.header = this->header;
    result.row_count = this->rows_count;
    result.field_count_first = f;
    result.perc_field_count_first = fp;
    result.field_count_second = s;
    result.perc_field_count_second = sp;
    result.field_count_third = t;
    result.perc_field_count_third = tp;
    for (const auto& fi_st : this->fields)
        result.field_statistic.push_back(fi_st->get_report());
    return result;
}

void Counter::add_item(int value)
{
    this->counter[value]++;
    this->_size++;
}

std::vector<std::pair<int, double>> Counter::get_distribution() const
{
    double total_v = 0;
    std::vector<std::pair<int, double>> result;
    for (const auto& [k, v] : this->counter)
    {
        total_v += v;
        result.emplace_back(std::move(std::make_pair(k, v)));
    }
    std::sort(result.begin(), result.end(), [](const std::pair<int, double>& el1, const std::pair<int, double>& el2){return el1.second > el2.second;});
    for (auto& [k, v] : result)
        v = v / total_v;
    return result;
}

double Counter::get_mean() const
{
    int num = 0;
    const auto den = (double)this->size();
    for (const auto& [k, v] : this->counter)
    {
        num += k * v;
    }
    if (den > 0)
        return num / den;
    else
        return 0;
}

int Counter::get_percentile(double percent, std::vector<std::pair<int, double>> dist) const
{
    if (percent < 0 || percent > 1)
        throw std::runtime_error("Percentile must be between 0 and 1, but is: " + std::to_string(percent));
    std::sort(dist.begin(), dist.end(), [](const std::pair<int, double>& el1, const std::pair<int, double>& el2){return el1.first < el2.first;});
    double acc = 0;
    for (const auto& [bin, bin_per] : dist)
    {
        acc += bin_per;
        if (acc > percent)
            return bin;
    }
    throw std::runtime_error("Something went wrong calculating percentile");
}

int Counter::get_percentile(double percent) const
{
    auto dist = this->get_distribution();
    return this->get_percentile(percent, dist);
}

void FieldStatistic::add_item(const Field& f)
{
    this->types.add_item((int)f.stype());
    this->char_count.add_item(f.char_count);
}

FieldReport FieldStatistic::get_report() const
{
    auto dist_char = this->char_count.get_distribution();
    auto dist_type = this->types.get_distribution();
    const SimpleType f = (dist_type.size() > 0) ? (SimpleType)dist_type[0].first : SimpleType::NONE;
    const double fp = (dist_type.size() > 0) ? dist_type[0].second : 0.0;
    const SimpleType s = (dist_type.size() > 1) ? (SimpleType)dist_type[1].first : SimpleType::NONE;
    const double sp = (dist_type.size() > 1) ? dist_type[1].second : 0.0;
    const SimpleType t = (dist_type.size() > 2) ? (SimpleType)dist_type[2].first : SimpleType::NONE;
    const double tp = (dist_type.size() > 2) ? dist_type[2].second : 0.0;
    return FieldReport{
        .name = this->name,
        .char_count_mean = this->char_count.get_mean(),
        .char_count_p_95 = this->char_count.get_percentile(0.95, dist_char),
        .char_count_p_75 = this->char_count.get_percentile(0.75, dist_char),
        .char_count_p_50 = this->char_count.get_percentile(0.50, dist_char),
        .char_count_p_25 = this->char_count.get_percentile(0.25, dist_char),
        .char_count_p_05 = this->char_count.get_percentile(0.05, dist_char),
        .type_first = f,
        .perc_type_first = fp,
        .type_second = s,
        .perc_type_second = sp,
        .type_third = t,
        .perc_type_third = tp,
    };
}
