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

    ASSERT_EQ(Field::string_to_stype("string"), SimpleType::STRING);
    ASSERT_EQ(Field::string_to_stype("strIng"), SimpleType::STRING);
    ASSERT_EQ(Field::string_to_stype("NONE"), SimpleType::NONE);
    ASSERT_EQ(Field::string_to_stype("INteger"), SimpleType::INTEGER);
}

TEST(CsvManager, FieldErrors)
{
    Field s{"fiel\"d1"};
    ASSERT_TRUE(s.quote_error());
    ASSERT_TRUE(s.non_print_char().empty());

    Field f{"fiel\x12" "dx"};
    EXPECT_EQ(f.non_print_char().size(), 1);
    ASSERT_EQ(f.non_print_char()[0], 0x12);

    Field f2{"\x01" "fiel" "\x12" "dx"};
    EXPECT_EQ(f2.non_print_char().size(), 2);
    ASSERT_EQ(f2.non_print_char()[0], 0x01);
    ASSERT_EQ(f2.non_print_char()[1], 0x12);
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

TEST(CsvManager, RowError)
{
    Row r1{"fi" "\x09" "eld1,f\"ie\"ld2,\"fie,l" "\x02" "d3\"", ',', '\"', '\n'};
    ASSERT_TRUE(r1.field_count_error(4));
    ASSERT_TRUE(r1.non_print_char_error());
    ASSERT_TRUE(r1.quote_error());
    EXPECT_EQ(r1.error_state.size(), 4);
    ASSERT_EQ(r1.error_state[0], "FIELD_COUNT_ERROR: this row has 3 fields and header 4 fields");
    ASSERT_EQ(r1.error_state[1], "NON_PRINTABLE_CHAR_ERROR: on field 0 the following chars are non printable: 0x09");
    ASSERT_EQ(r1.error_state[2], "NON_PRINTABLE_CHAR_ERROR: on field 2 the following chars are non printable: 0x02");
    ASSERT_EQ(r1.error_state[3], "QUOTE_ERROR: field 1, string: f\"ie\"ld2");
    ASSERT_FALSE(r1.check_field_type(1, SimpleType::INTEGER));
    ASSERT_TRUE(r1.check_field_type(1, SimpleType::STRING));

    Row r2{"abd,,456.4"};
    ASSERT_TRUE(r2.check_field_type(1, SimpleType::EMPTY));
    ASSERT_FALSE(r2.check_field_type(1, SimpleType::INTEGER));
    EXPECT_EQ(r2.error_state.size(), 1);
    ASSERT_EQ(r2.error_state[0], "TYPE_FOUND: Empty on field index 1");

    Row r3{"abd,,456.4"};
    ASSERT_TRUE(r3.check_field_type(2, SimpleType::NUMBER));
    ASSERT_FALSE(r3.check_field_type(2, SimpleType::INTEGER));
    EXPECT_EQ(r3.error_state.size(), 1);
    ASSERT_EQ(r3.error_state[0], "TYPE_FOUND: Number on field index 2");
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
