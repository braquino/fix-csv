#include <gtest/gtest.h>
#include "csvmanager.h"

TEST(CsvManager, Field)
{
    Field s{"field1"};
    ASSERT_EQ(s.char_count, 6);
    ASSERT_EQ(s.str, "field1");
    ASSERT_EQ(s.hex(), "66 69 65 6c 64 31 ");
    ASSERT_EQ(s.stype(), SimpleType::STRING);

    Field n{"456.6"};
    ASSERT_EQ(n.stype(), SimpleType::NUMBER);

    Field i{"4567658"};
    ASSERT_EQ(i.stype(), SimpleType::INTEGER);

    Field i2{"\"3\""};
    ASSERT_EQ(i2.stype(), SimpleType::INTEGER);

    Field e{""};
    ASSERT_EQ(e.stype(), SimpleType::EMPTY);

    Field n2{"\"245,34\""};
    ASSERT_EQ(n2.stype(), SimpleType::NUMBER);
}

TEST(CsvManager, Row)
{
    Row r1{"field1,f\"ie\"ld2,\"fie,ld3\"", ',', '\"', '\n'};
    ASSERT_EQ(r1.col_count, 3);
    ASSERT_EQ(r1.char_count, 25);
    ASSERT_EQ(r1.str, "field1,f\"ie\"ld2,\"fie,ld3\"");
    ASSERT_EQ(r1.fields[0].str, "field1");
    ASSERT_EQ(r1.fields[1].str, "f\"ie\"ld2");
    ASSERT_TRUE(r1.fields[1].quote_error());
    ASSERT_EQ(r1.fields[2].str, "\"fie,ld3\"");
    ASSERT_FALSE(r1.fields[2].quote_error());
}

TEST(CsvManager, getNextRow)
{
    CsvManager csv{};
    csv.open_file("../tests/resources/file_test_1.csv");
    Row r0 = csv.next_row();
    ASSERT_EQ(r0.str, "col1,\"col2\",col3,col4\n");
    ASSERT_EQ(r0.col_count, 4);
    Row r1 = csv.next_row();
    ASSERT_EQ(r1.str, "56,\"field,comma\",,\n");
    ASSERT_EQ(r1.col_count, 4);
    Row r0b = csv.back_row();
    ASSERT_EQ(r0b.str, "col1,\"col2\",col3,col4\n");
    ASSERT_EQ(r0b.col_count, 4);
    Row r1f = csv.next_row();
    ASSERT_EQ(r1f.str, "56,\"field,comma\",,\n");
    ASSERT_EQ(r1f.col_count, 4);
    Row r2 = csv.next_row();
    ASSERT_EQ(r2.str, "2.4,\"field\"\"quote\",3,ù\n");
    ASSERT_EQ(r2.col_count, 4);
    Row r3 = csv.next_row();
    ASSERT_EQ(r3.str, "\"904,2\",xxx,,ì\n");
    ASSERT_EQ(r3.col_count, 4);
    Row r4 = csv.next_row();
    ASSERT_EQ(r4.str, "\"904,2\",xxx,,ì\n");
    ASSERT_EQ(r4.col_count, 4);
    // FIXME: When reach the end, doesn't go back
    //    Row r3b = csv.back_row();
    //    ASSERT_EQ(r3b.str, "\"904,2\",xxx,,ì");
    //    ASSERT_EQ(r3b.col_count, 4);
}

TEST(CsvManager, getNextError)
{
    CsvManager csv{};
    csv.open_file("../tests/resources/file_test_2.csv");
    Row re0 = csv.next_error();
    ASSERT_EQ(re0.str, "56,\"field,comma\",\n");
    ASSERT_EQ(re0.col_count, 3);
    Row re1 = csv.next_error();
    ASSERT_EQ(re1.str, "\"904,2\",xx,x,,ì\n");
    ASSERT_EQ(re1.col_count, 5);
}

TEST(CsvManager, getRowsCount)
{
    CsvManager csv1{};
    csv1.open_file("../tests/resources/file_test_1.csv");
    ASSERT_EQ(csv1.count_rows(), 4);
    CsvManager csv2{};
    csv2.open_file("../tests/resources/file_test_2.csv");
    ASSERT_EQ(csv2.count_rows(), 5);
}

TEST(CsvManager, countRows)
{
    CsvManager csv;
    csv.open_file("../tests/resources/file_test_3.csv");
    ASSERT_EQ(csv.count_rows(), 7);
}

TEST(CsvManager, saveReplacedreplaceRow)
{
    CsvManager csv;
    csv.open_file("../tests/resources/file_test_4.csv");
    csv.replace_row(3, "replaced row 1");
    csv.replace_row(2, "replaced row 2\n");
    csv.replace_row(4, "\n");
    csv.replace_row(5, "");
    csv.save_file("../tests/resources/file_test_4.csv.out");
    std::fstream f{"../tests/resources/file_test_4.csv.out"};
    std::stringstream ss;
    ss << f.rdbuf();
    f.close();
    remove("../tests/resources/file_test_4.csv.out");
    ASSERT_EQ(ss.str(),"col1,\"col2\",col3,col4\nreplaced row 2\nreplaced row 1\n332,454,ddw,fe\n\"904,2\",xx,x,,ì\n");
}
