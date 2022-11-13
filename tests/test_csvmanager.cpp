#include <gtest/gtest.h>
#include "csvmanager.h"

TEST(CsvManager, Field)
{
    Field f{"field1"};
    ASSERT_EQ(f.char_count, 6);
    ASSERT_EQ(f.str, "field1");
    ASSERT_EQ(f.hex(), "<0x66><0x69><0x65><0x6c><0x64><0x31>");
}
