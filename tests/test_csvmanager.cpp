#include <gtest/gtest.h>
#include "csvmanager.h"

TEST(CsvManager, Field)
{
    Field s{"field1"};
    ASSERT_EQ(s.char_count, 6);
    ASSERT_EQ(s.str, "field1");
    ASSERT_EQ(s.hex(), "<0x66><0x69><0x65><0x6c><0x64><0x31>");
    ASSERT_EQ(s.stype(), SimpleType::STRING);

    Field n{"456.6"};
    ASSERT_EQ(n.stype(), SimpleType::NUMBER);

    Field i{"4567658"};
    ASSERT_EQ(i.stype(), SimpleType::INTEGER);
}

TEST(CsvManager, Row)
{
    Row r1{"field1,field2,\"fie,ld3\"", ',', '\"'};
    ASSERT_EQ(r1.col_count, 3);
    ASSERT_EQ(r1.char_count, 23);
    ASSERT_EQ(r1.str, "field1,field2,\"fie,ld3\"");
    ASSERT_EQ(r1.fields[0].str, "field1");
    ASSERT_EQ(r1.fields[1].str, "field2");
    ASSERT_EQ(r1.fields[2].str, "\"fie,ld3\"");
}

TEST(CsvManager, getNextRow)
{
    CsvManager csv{};
    csv.open_file("../tests/resources/file_test_1.csv");
    Row r0 = csv.next_row();
    ASSERT_EQ(r0.str, "col1,\"col2\",col3,col4");
    ASSERT_EQ(r0.col_count, 4);
    Row r1 = csv.next_row();
    ASSERT_EQ(r1.str, "56,\"field,comma\",,");
    ASSERT_EQ(r1.col_count, 4);
    Row r0b = csv.back_row();
    ASSERT_EQ(r0b.str, "col1,\"col2\",col3,col4");
    ASSERT_EQ(r0b.col_count, 4);
    Row r1f = csv.next_row();
    ASSERT_EQ(r1f.str, "56,\"field,comma\",,");
    ASSERT_EQ(r1f.col_count, 4);
    Row r2 = csv.next_row();
    ASSERT_EQ(r2.str, "2.4,\"field\"\"quote\",3,ù");
    ASSERT_EQ(r2.col_count, 4);
    Row r3 = csv.next_row();
    ASSERT_EQ(r3.str, "\"904,2\",xxx,,ì");
    ASSERT_EQ(r3.col_count, 4);
    Row r4 = csv.next_row();
    ASSERT_EQ(r4.str, "");
    ASSERT_EQ(r4.col_count, 0);
    ASSERT_TRUE(r4.fields.empty());
    // FIXME: When reach the end, doesn't go back
    //    Row r3b = csv.back_row();
    //    ASSERT_EQ(r3b.str, "\"904,2\",xxx,,ì");
    //    ASSERT_EQ(r3b.col_count, 4);
}
