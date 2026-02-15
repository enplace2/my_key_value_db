#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <filesystem>
#include <string>
#include "../src/KVDatabase/KVDatabase.h"
#include "../src/FSManager/FSManager.h"
#include "../src/CLI/KVCli.h"

namespace fs = std::filesystem;
using ::testing::HasSubstr;
using ::testing::ContainsRegex;

class CLIFeatureTest : public ::testing::Test {
protected:
    std::vector<std::string> testDatabases;

    void TearDown() override {
        for (const auto& dbNameToDelete : testDatabases) {
            std::string dbPath = FSManager::getDbPath(const_cast<std::string&>(dbNameToDelete));
            if (fs::exists(dbPath)) {
                fs::remove_all(dbPath);
            }
        }
    }

    KVDatabase createTestDb() {
        std::string dbName = "cli_feature_test_db_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
        testDatabases.push_back(dbName);
        return KVDatabase::createEmptyDb(dbName);
    }
};

TEST_F(CLIFeatureTest, BasicSetGetWorkflow) {
    KVDatabase db = createTestDb();
    std::istringstream input("SET name alice string\nGET name\nEXIT\n");
    std::ostringstream output;

    KVCli::runInteractiveMode(db, input, output);

    std::string result = output.str();
    EXPECT_THAT(result, HasSubstr("Stored: name = alice (string)"));
    EXPECT_THAT(result, HasSubstr("alice"));
}

TEST_F(CLIFeatureTest, MultiWordValueWorkflow) {
    KVDatabase db = createTestDb();
    std::istringstream input("SET greeting \"hello world\" string\nGET greeting\nEXIT\n");
    std::ostringstream output;

    KVCli::runInteractiveMode(db, input, output);

    std::string result = output.str();
    EXPECT_THAT(result, HasSubstr("Stored: greeting = hello world (string)"));
    EXPECT_THAT(result, HasSubstr("hello world"));
}

TEST_F(CLIFeatureTest, AllTypesWorkflow) {
    KVDatabase db = createTestDb();
    std::istringstream input(
        "SET str_key test_value string\n"
        "SET int_key 42 int\n"
        "SET bool_key true bool\n"
        "SET double_key 3.14 double\n"
        "SET uint_key 999 uint\n"
        "GET str_key\n"
        "GET int_key\n"
        "GET bool_key\n"
        "GET double_key\n"
        "GET uint_key\n"
        "EXIT\n"
    );
    std::ostringstream output;

    KVCli::runInteractiveMode(db, input, output);

    std::string result = output.str();
    EXPECT_THAT(result, HasSubstr("Stored: str_key = test_value (string)"));
    EXPECT_THAT(result, HasSubstr("Stored: int_key = 42 (int)"));
    EXPECT_THAT(result, HasSubstr("Stored: bool_key = true (bool)"));
    EXPECT_THAT(result, HasSubstr("Stored: double_key = 3.14 (double)"));
    EXPECT_THAT(result, HasSubstr("Stored: uint_key = 999 (uint)"));
    EXPECT_THAT(result, HasSubstr("test_value"));
    EXPECT_THAT(result, HasSubstr("42"));
    EXPECT_THAT(result, HasSubstr("true"));
    EXPECT_THAT(result, HasSubstr("3.14"));
    EXPECT_THAT(result, HasSubstr("999"));
}

TEST_F(CLIFeatureTest, DeleteWorkflow) {
    KVDatabase db = createTestDb();
    std::istringstream input(
        "SET key1 value1 string\n"
        "DELETE key1\n"
        "GET key1\n"
        "EXIT\n"
    );
    std::ostringstream output;

    KVCli::runInteractiveMode(db, input, output);

    std::string result = output.str();
    EXPECT_THAT(result, HasSubstr("Stored: key1 = value1 (string)"));
    EXPECT_THAT(result, HasSubstr("Deleted: key1"));
    EXPECT_THAT(result, HasSubstr("Error:"));
}

TEST_F(CLIFeatureTest, ListWorkflow) {
    KVDatabase db = createTestDb();
    std::istringstream input(
        "SET apple red string\n"
        "SET banana yellow string\n"
        "SET count 5 int\n"
        "LIST\n"
        "EXIT\n"
    );
    std::ostringstream output;

    KVCli::runInteractiveMode(db, input, output);

    std::string result = output.str();
    EXPECT_THAT(result, HasSubstr("apple:"));
    EXPECT_THAT(result, HasSubstr("red"));
    EXPECT_THAT(result, HasSubstr("banana:"));
    EXPECT_THAT(result, HasSubstr("yellow"));
    EXPECT_THAT(result, HasSubstr("count:"));
    EXPECT_THAT(result, HasSubstr("5"));
}

TEST_F(CLIFeatureTest, ErrorRecovery) {
    KVDatabase db = createTestDb();
    std::istringstream input(
        "INVALID_COMMAND\n"
        "SET key value string\n"
        "GET key\n"
        "EXIT\n"
    );
    std::ostringstream output;

    KVCli::runInteractiveMode(db, input, output);

    std::string result = output.str();
    EXPECT_THAT(result, HasSubstr("Unknown command"));
    EXPECT_THAT(result, HasSubstr("Stored: key = value (string)"));
    EXPECT_THAT(result, HasSubstr("value"));
}

TEST_F(CLIFeatureTest, InvalidTypeConversion) {
    KVDatabase db = createTestDb();
    std::istringstream input(
        "SET key not_a_number int\n"
        "SET key2 valid_value string\n"
        "GET key2\n"
        "EXIT\n"
    );
    std::ostringstream output;

    KVCli::runInteractiveMode(db, input, output);

    std::string result = output.str();
    EXPECT_THAT(result, HasSubstr("Error:"));
    EXPECT_THAT(result, HasSubstr("Stored: key2 = valid_value (string)"));
    EXPECT_THAT(result, HasSubstr("valid_value"));
}

TEST_F(CLIFeatureTest, MissingKeyError) {
    KVDatabase db = createTestDb();
    std::istringstream input(
        "GET nonexistent_key\n"
        "EXIT\n"
    );
    std::ostringstream output;

    KVCli::runInteractiveMode(db, input, output);

    std::string result = output.str();
    EXPECT_THAT(result, HasSubstr("Error:"));
}

TEST_F(CLIFeatureTest, UnclosedQuoteError) {
    KVDatabase db = createTestDb();
    std::istringstream input(
        "SET key \"unclosed string\n"
        "SET key2 valid string\n"
        "GET key2\n"
        "EXIT\n"
    );
    std::ostringstream output;

    KVCli::runInteractiveMode(db, input, output);

    std::string result = output.str();
    EXPECT_THAT(result, HasSubstr("Error:"));
    EXPECT_THAT(result, HasSubstr("Stored: key2 = valid (string)"));
    EXPECT_THAT(result, HasSubstr("valid"));
}

TEST_F(CLIFeatureTest, HelpCommand) {
    KVDatabase db = createTestDb();
    std::istringstream input("HELP\nEXIT\n");
    std::ostringstream output;

    KVCli::runInteractiveMode(db, input, output);

    std::string result = output.str();
    EXPECT_THAT(result, HasSubstr("Available commands:"));
    EXPECT_THAT(result, HasSubstr("SET"));
    EXPECT_THAT(result, HasSubstr("GET"));
    EXPECT_THAT(result, HasSubstr("DELETE"));
    EXPECT_THAT(result, HasSubstr("LIST"));
    EXPECT_THAT(result, HasSubstr("EXIT"));
    EXPECT_THAT(result, HasSubstr("HELP"));
}

TEST_F(CLIFeatureTest, MultiCommandSequence) {
    KVDatabase db = createTestDb();
    std::istringstream input(
        "SET name Alice string\n"
        "SET age 30 int\n"
        "SET active true bool\n"
        "LIST\n"
        "GET name\n"
        "DELETE age\n"
        "LIST\n"
        "SET name Bob string\n"
        "GET name\n"
        "EXIT\n"
    );
    std::ostringstream output;

    KVCli::runInteractiveMode(db, input, output);

    std::string result = output.str();
    EXPECT_THAT(result, HasSubstr("Stored: name = Alice (string)"));
    EXPECT_THAT(result, HasSubstr("Stored: age = 30 (int)"));
    EXPECT_THAT(result, HasSubstr("Stored: active = true (bool)"));
    EXPECT_THAT(result, HasSubstr("Alice"));
    EXPECT_THAT(result, HasSubstr("Deleted: age"));
    EXPECT_THAT(result, HasSubstr("Stored: name = Bob (string)"));
    EXPECT_THAT(result, HasSubstr("Bob"));
}

TEST_F(CLIFeatureTest, PersistenceWorkflow) {
    std::string persistDbName = "cli_persist_test_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    testDatabases.push_back(persistDbName);

    {
        KVDatabase persistDb = KVDatabase::createEmptyDb(persistDbName);
        std::istringstream input(
            "SET persistent_key persistent_value string\n"
            "SET number 123 int\n"
            "EXIT\n"
        );
        std::ostringstream output;

        KVCli::runInteractiveMode(persistDb, input, output);

        std::string result = output.str();
        EXPECT_THAT(result, HasSubstr("Stored: persistent_key = persistent_value (string)"));
        EXPECT_THAT(result, HasSubstr("Stored: number = 123 (int)"));
    }

    {
        KVDatabase reloadedDb = KVDatabase::load(persistDbName);
        std::istringstream input(
            "GET persistent_key\n"
            "GET number\n"
            "LIST\n"
            "EXIT\n"
        );
        std::ostringstream output;

        KVCli::runInteractiveMode(reloadedDb, input, output);

        std::string result = output.str();
        EXPECT_THAT(result, HasSubstr("persistent_value"));
        EXPECT_THAT(result, HasSubstr("123"));
        EXPECT_THAT(result, HasSubstr("persistent_key:"));
        EXPECT_THAT(result, HasSubstr("number:"));
    }
}
