#include <gtest/gtest.h>
#include <csvstatistics.h>

TEST(CsvStatistics, Counter)
{
    Counter c{"test"};
    std::vector<int> v{1,2,3,4,4,5,5,5,6,6,7,8,9,10,11};
    for (auto& el : v)
        c.add_item(el);
    ASSERT_NEAR(c.get_mean(), 5.7333333, 0.0001);
    auto dist = c.get_distribution();
    ASSERT_EQ(dist[0].first, 5);
    ASSERT_DOUBLE_EQ(dist[0].second, 0.2);
    ASSERT_EQ(dist[1].first, 4);
    ASSERT_DOUBLE_EQ(dist[1].second, 2.0 / 15.0);
    ASSERT_EQ(dist[10].first, 11);
    ASSERT_DOUBLE_EQ(dist[10].second, 1.0 / 15.0);
    ASSERT_EQ(c.get_percentile(0.5), 5);
    ASSERT_EQ(c.get_percentile(0.1), 2);
    ASSERT_EQ(c.get_percentile(0.8), 9);
}

TEST(CsvStatistics, FieldStatistic)
{
    FieldStatistic fs{"t_field"};
    std::vector<std::string> v{"foo", "4567", "0.566", "lonnnng field", "", "test", "9", "ssssssss", "bar", "450", "r", ""};
    for (auto& el : v)
        fs.add_item(Field(el));
    FieldReport rep = fs.get_report();
    ASSERT_EQ(rep.name, "t_field");
    ASSERT_DOUBLE_EQ(rep.char_count_mean, 3.75);
    ASSERT_EQ(rep.char_count_p_95, 13);
    ASSERT_EQ(rep.char_count_p_75, 5);
    ASSERT_EQ(rep.char_count_p_50, 3);
    ASSERT_EQ(rep.char_count_p_25, 1);
    ASSERT_EQ(rep.char_count_p_05, 0);
    ASSERT_EQ(rep.type_first, SimpleType::STRING);
    ASSERT_DOUBLE_EQ(rep.perc_type_first, 6.0 / 12.0);
    ASSERT_EQ(rep.type_second, SimpleType::INTEGER);
    ASSERT_DOUBLE_EQ(rep.perc_type_second, 3.0 / 12.0);
    ASSERT_EQ(rep.type_third, SimpleType::EMPTY);
    ASSERT_DOUBLE_EQ(rep.perc_type_third, 2.0 / 12.0);
}

TEST(CsvStatistics, CsvStatistics)
{
    CsvManager csv;
    csv.open_file("../tests/resources/file_test_1.csv");
    CsvStatistics stats{csv.next_row()};
    while (true)
    {
        auto r = csv.next_row();
        if (!csv.eof())
            stats.add_row(r);
        else
            break;
    }
    RowsReport rep = stats.get_rows_report();
    ASSERT_EQ(rep.row_count, 4);
    ASSERT_EQ(rep.field_count_first, 4);
    ASSERT_EQ(rep.perc_field_count_second, 0.0);
    ASSERT_EQ(rep.perc_field_count_first, 1.0);
    ASSERT_EQ(stats.get_field_report("col1").type_first, SimpleType::NUMBER);
    ASSERT_EQ(stats.get_field_report("col1").perc_type_first, 2.0 / 3.0);
    ASSERT_EQ(rep.field_statistic[2].type_first, SimpleType::EMPTY);
    ASSERT_EQ(rep.field_statistic[2].type_second, SimpleType::INTEGER);
    ASSERT_EQ(rep.field_statistic[2].perc_type_second, 1.0 / 3.0);
}
