#include <gtest/gtest.h>
#include "../src/utils/TypeConverter.h"
#include "../src/structs/ValueWithTypeObject.h"

class TypeConversionTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(TypeConversionTest, toInt64_ValidPositiveNumber) {
    EXPECT_EQ(TypeConverter::toInt64("42"), 42);
    EXPECT_EQ(TypeConverter::toInt64("123456789"), 123456789);
}

TEST_F(TypeConversionTest, toInt64_ValidNegativeNumber) {
    EXPECT_EQ(TypeConverter::toInt64("-42"), -42);
    EXPECT_EQ(TypeConverter::toInt64("-123456789"), -123456789);
}

TEST_F(TypeConversionTest, toInt64_Zero) {
    EXPECT_EQ(TypeConverter::toInt64("0"), 0);
    EXPECT_EQ(TypeConverter::toInt64("-0"), 0);
}

TEST_F(TypeConversionTest, toInt64_LargeNumbers) {
    EXPECT_EQ(TypeConverter::toInt64("9223372036854775807"), 9223372036854775807LL);
    EXPECT_EQ(TypeConverter::toInt64("-9223372036854775808"), -9223372036854775807LL - 1);
}

TEST_F(TypeConversionTest, toInt64_InvalidString) {
    EXPECT_THROW(TypeConverter::toInt64("not_a_number"), std::invalid_argument);
    EXPECT_THROW(TypeConverter::toInt64("12.34"), std::invalid_argument);
    EXPECT_THROW(TypeConverter::toInt64("42abc"), std::invalid_argument);
}

TEST_F(TypeConversionTest, toInt64_EmptyString) {
    EXPECT_THROW(TypeConverter::toInt64(""), std::invalid_argument);
}

TEST_F(TypeConversionTest, toUInt64_ValidPositiveNumber) {
    EXPECT_EQ(TypeConverter::toUInt64("42"), 42ULL);
    EXPECT_EQ(TypeConverter::toUInt64("123456789"), 123456789ULL);
}

TEST_F(TypeConversionTest, toUInt64_Zero) {
    EXPECT_EQ(TypeConverter::toUInt64("0"), 0ULL);
}

TEST_F(TypeConversionTest, toUInt64_LargeNumbers) {
    EXPECT_EQ(TypeConverter::toUInt64("18446744073709551615"), 18446744073709551615ULL);
}

TEST_F(TypeConversionTest, toUInt64_RejectNegative) {
    EXPECT_THROW(TypeConverter::toUInt64("-42"), std::invalid_argument);
    EXPECT_THROW(TypeConverter::toUInt64("-1"), std::invalid_argument);
}

TEST_F(TypeConversionTest, toUInt64_InvalidString) {
    EXPECT_THROW(TypeConverter::toUInt64("not_a_number"), std::invalid_argument);
    EXPECT_THROW(TypeConverter::toUInt64("12.34"), std::invalid_argument);
}

TEST_F(TypeConversionTest, toDouble_ValidPositiveNumber) {
    EXPECT_DOUBLE_EQ(TypeConverter::toDouble("42.5"), 42.5);
    EXPECT_DOUBLE_EQ(TypeConverter::toDouble("123.456"), 123.456);
}

TEST_F(TypeConversionTest, toDouble_ValidNegativeNumber) {
    EXPECT_DOUBLE_EQ(TypeConverter::toDouble("-42.5"), -42.5);
    EXPECT_DOUBLE_EQ(TypeConverter::toDouble("-123.456"), -123.456);
}

TEST_F(TypeConversionTest, toDouble_Zero) {
    EXPECT_DOUBLE_EQ(TypeConverter::toDouble("0"), 0.0);
    EXPECT_DOUBLE_EQ(TypeConverter::toDouble("0.0"), 0.0);
}

TEST_F(TypeConversionTest, toDouble_ScientificNotation) {
    EXPECT_DOUBLE_EQ(TypeConverter::toDouble("1.23e10"), 1.23e10);
    EXPECT_DOUBLE_EQ(TypeConverter::toDouble("4.56E-7"), 4.56E-7);
}

TEST_F(TypeConversionTest, toDouble_InvalidString) {
    EXPECT_THROW(TypeConverter::toDouble("not_a_number"), std::invalid_argument);
    EXPECT_THROW(TypeConverter::toDouble(""), std::invalid_argument);
}

TEST_F(TypeConversionTest, toBool_TrueCaseInsensitive) {
    EXPECT_TRUE(TypeConverter::toBool("true"));
    EXPECT_TRUE(TypeConverter::toBool("TRUE"));
    EXPECT_TRUE(TypeConverter::toBool("True"));
    EXPECT_TRUE(TypeConverter::toBool("TrUe"));
}

TEST_F(TypeConversionTest, toBool_FalseCaseInsensitive) {
    EXPECT_FALSE(TypeConverter::toBool("false"));
    EXPECT_FALSE(TypeConverter::toBool("FALSE"));
    EXPECT_FALSE(TypeConverter::toBool("False"));
    EXPECT_FALSE(TypeConverter::toBool("FaLsE"));
}

TEST_F(TypeConversionTest, toBool_InvalidString) {
    EXPECT_THROW(TypeConverter::toBool("yes"), std::invalid_argument);
    EXPECT_THROW(TypeConverter::toBool("no"), std::invalid_argument);
    EXPECT_THROW(TypeConverter::toBool("1"), std::invalid_argument);
    EXPECT_THROW(TypeConverter::toBool("0"), std::invalid_argument);
    EXPECT_THROW(TypeConverter::toBool(""), std::invalid_argument);
}

TEST_F(TypeConversionTest, variantToString_String) {
    ValueTypeVariant value = std::string("hello world");
    EXPECT_EQ(TypeConverter::variantToString(value), "hello world");
}

TEST_F(TypeConversionTest, variantToString_Int64) {
    ValueTypeVariant value = int64_t(42);
    EXPECT_EQ(TypeConverter::variantToString(value), "42");

    value = int64_t(-123);
    EXPECT_EQ(TypeConverter::variantToString(value), "-123");
}

TEST_F(TypeConversionTest, variantToString_UInt64) {
    ValueTypeVariant value = uint64_t(42);
    EXPECT_EQ(TypeConverter::variantToString(value), "42");
}

TEST_F(TypeConversionTest, variantToString_Double) {
    ValueTypeVariant value = 42.5;
    EXPECT_EQ(TypeConverter::variantToString(value), "42.500000");
}

TEST_F(TypeConversionTest, variantToString_BoolTrue) {
    ValueTypeVariant value = true;
    EXPECT_EQ(TypeConverter::variantToString(value), "true");
}

TEST_F(TypeConversionTest, variantToString_BoolFalse) {
    ValueTypeVariant value = false;
    EXPECT_EQ(TypeConverter::variantToString(value), "false");
}

TEST_F(TypeConversionTest, variantToString_KVMap) {
    KVMap map;
    map["key1"] = {"value1", "string"};
    ValueTypeVariant value = map;
    EXPECT_EQ(TypeConverter::variantToString(value), "{map}");
}

TEST_F(TypeConversionTest, getTypeName_String) {
    ValueTypeVariant value = std::string("test");
    EXPECT_EQ(TypeConverter::getTypeName(value), "string");
}

TEST_F(TypeConversionTest, getTypeName_Int64) {
    ValueTypeVariant value = int64_t(42);
    EXPECT_EQ(TypeConverter::getTypeName(value), "int");
}

TEST_F(TypeConversionTest, getTypeName_UInt64) {
    ValueTypeVariant value = uint64_t(42);
    EXPECT_EQ(TypeConverter::getTypeName(value), "uint");
}

TEST_F(TypeConversionTest, getTypeName_Double) {
    ValueTypeVariant value = 42.5;
    EXPECT_EQ(TypeConverter::getTypeName(value), "double");
}

TEST_F(TypeConversionTest, getTypeName_Bool) {
    ValueTypeVariant value = true;
    EXPECT_EQ(TypeConverter::getTypeName(value), "bool");
}

TEST_F(TypeConversionTest, getTypeName_KVMap) {
    KVMap map;
    ValueTypeVariant value = map;
    EXPECT_EQ(TypeConverter::getTypeName(value), "map");
}
