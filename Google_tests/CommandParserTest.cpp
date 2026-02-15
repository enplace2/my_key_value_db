#include <gtest/gtest.h>
#include "../src/CLI/CommandParser.h"

class CommandParserTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CommandParserTest, ParseSetCommandValid) {
    auto result = CommandParser::parse("SET name Alice string");
    EXPECT_EQ(result.type, CommandType::SET);
    EXPECT_EQ(result.key, "name");
    EXPECT_EQ(result.value, "Alice");
    EXPECT_EQ(result.valueType, "string");
    EXPECT_TRUE(result.errorMsg.empty());
}

TEST_F(CommandParserTest, ParseGetCommandValid) {
    auto result = CommandParser::parse("GET name");
    EXPECT_EQ(result.type, CommandType::GET);
    EXPECT_EQ(result.key, "name");
    EXPECT_TRUE(result.errorMsg.empty());
}

TEST_F(CommandParserTest, ParseDeleteCommandValid) {
    auto result = CommandParser::parse("DELETE name");
    EXPECT_EQ(result.type, CommandType::DELETE);
    EXPECT_EQ(result.key, "name");
    EXPECT_TRUE(result.errorMsg.empty());
}

TEST_F(CommandParserTest, ParseListCommandValid) {
    auto result = CommandParser::parse("LIST");
    EXPECT_EQ(result.type, CommandType::LIST);
    EXPECT_TRUE(result.errorMsg.empty());
}

TEST_F(CommandParserTest, ParseExitCommandValid) {
    auto result = CommandParser::parse("EXIT");
    EXPECT_EQ(result.type, CommandType::EXIT);
    EXPECT_TRUE(result.errorMsg.empty());
}

TEST_F(CommandParserTest, ParseSetWithQuotedValue) {
    auto result = CommandParser::parse("SET name \"Alice Smith\" string");
    EXPECT_EQ(result.type, CommandType::SET);
    EXPECT_EQ(result.key, "name");
    EXPECT_EQ(result.value, "Alice Smith");
    EXPECT_EQ(result.valueType, "string");
    EXPECT_TRUE(result.errorMsg.empty());
}

TEST_F(CommandParserTest, ParseSetWithQuotedMessage) {
    auto result = CommandParser::parse("SET msg \"Hello World\" string");
    EXPECT_EQ(result.type, CommandType::SET);
    EXPECT_EQ(result.key, "msg");
    EXPECT_EQ(result.value, "Hello World");
    EXPECT_EQ(result.valueType, "string");
    EXPECT_TRUE(result.errorMsg.empty());
}

TEST_F(CommandParserTest, ParseSetWithUnclosedQuote) {
    auto result = CommandParser::parse("SET bad \"unclosed string");
    EXPECT_EQ(result.type, CommandType::INVALID);
    EXPECT_FALSE(result.errorMsg.empty());
    EXPECT_NE(result.errorMsg.find("Unclosed quote"), std::string::npos);
}

TEST_F(CommandParserTest, ParseInvalidCommand) {
    auto result = CommandParser::parse("INVALID");
    EXPECT_EQ(result.type, CommandType::INVALID);
    EXPECT_FALSE(result.errorMsg.empty());
}

TEST_F(CommandParserTest, ParseSetWithMissingArgs) {
    auto result = CommandParser::parse("SET key");
    EXPECT_EQ(result.type, CommandType::INVALID);
    EXPECT_FALSE(result.errorMsg.empty());
    EXPECT_NE(result.errorMsg.find("Usage"), std::string::npos);
}

TEST_F(CommandParserTest, ParseGetWithMissingArgs) {
    auto result = CommandParser::parse("GET");
    EXPECT_EQ(result.type, CommandType::INVALID);
    EXPECT_FALSE(result.errorMsg.empty());
    EXPECT_NE(result.errorMsg.find("Usage"), std::string::npos);
}

TEST_F(CommandParserTest, ParseSetWithInvalidType) {
    auto result = CommandParser::parse("SET key val badtype");
    EXPECT_EQ(result.type, CommandType::INVALID);
    EXPECT_FALSE(result.errorMsg.empty());
    EXPECT_NE(result.errorMsg.find("Invalid type"), std::string::npos);
}

TEST_F(CommandParserTest, ParseGetCaseInsensitive) {
    auto result1 = CommandParser::parse("get name");
    EXPECT_EQ(result1.type, CommandType::GET);
    EXPECT_EQ(result1.key, "name");

    auto result2 = CommandParser::parse("GeT name");
    EXPECT_EQ(result2.type, CommandType::GET);
    EXPECT_EQ(result2.key, "name");

    auto result3 = CommandParser::parse("GET name");
    EXPECT_EQ(result3.type, CommandType::GET);
    EXPECT_EQ(result3.key, "name");
}
