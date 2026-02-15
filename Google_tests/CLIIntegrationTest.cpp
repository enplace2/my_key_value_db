#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include "../src/KVDatabase/KVDatabase.h"
#include "../src/FSManager/FSManager.h"
#include "../src/utils/TypeConverter.h"

namespace fs = std::filesystem;

class CLIIntegrationTest : public ::testing::Test {
protected:
    std::vector<std::string> testDatabases;

    void TearDown() override {
        for (const auto& dbName : testDatabases) {
            std::string dbPath = FSManager::getDbPath(const_cast<std::string&>(dbName));
            if (fs::exists(dbPath)) {
                fs::remove_all(dbPath);
            }
        }
    }
};

TEST_F(CLIIntegrationTest, SetThenGet) {
    std::string dbName = "set_get_db";
    testDatabases.push_back(dbName);

    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    std::string key = "test_key";
    std::string value = "test_value";
    std::string type = "string";
    db.store(key, value, type);

    ValueTypeVariant retrieved = db.get(key);
    EXPECT_EQ(std::get<std::string>(retrieved), value);

    db.destroy();
}

TEST_F(CLIIntegrationTest, SetThenGetMultipleTypes) {
    std::string dbName = "multiple_types_db";
    testDatabases.push_back(dbName);

    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    std::string stringKey = "string_key";
    std::string stringValue = "hello";
    std::string stringType = "string";
    db.store(stringKey, stringValue, stringType);

    std::string intKey = "int_key";
    int64_t intValue = -42;
    std::string intType = "int";
    db.store(intKey, intValue, intType);

    std::string boolKey = "bool_key";
    bool boolValue = true;
    std::string boolType = "bool";
    db.store(boolKey, boolValue, boolType);

    std::string doubleKey = "double_key";
    double doubleValue = 3.14159;
    std::string doubleType = "double";
    db.store(doubleKey, doubleValue, doubleType);

    std::string uintKey = "uint_key";
    uint64_t uintValue = 12345;
    std::string uintType = "uint";
    db.store(uintKey, uintValue, uintType);

    EXPECT_EQ(std::get<std::string>(db.get(stringKey)), stringValue);
    EXPECT_EQ(std::get<int64_t>(db.get(intKey)), intValue);
    EXPECT_EQ(std::get<bool>(db.get(boolKey)), boolValue);
    EXPECT_EQ(std::get<double>(db.get(doubleKey)), doubleValue);
    EXPECT_EQ(std::get<uint64_t>(db.get(uintKey)), uintValue);

    db.destroy();
}

TEST_F(CLIIntegrationTest, SetThenDelete) {
    std::string dbName = "set_delete_db";
    testDatabases.push_back(dbName);

    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    std::string key = "delete_me";
    std::string value = "temporary";
    std::string type = "string";
    db.store(key, value, type);

    ValueTypeVariant retrieved = db.get(key);
    EXPECT_EQ(std::get<std::string>(retrieved), value);

    db.deleteKey(key);

    EXPECT_THROW(db.get(key), std::runtime_error);

    db.destroy();
}

TEST_F(CLIIntegrationTest, SetMultipleThenList) {
    std::string dbName = "set_multiple_list_db";
    testDatabases.push_back(dbName);

    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    std::string key1 = "first";
    std::string value1 = "value1";
    std::string key2 = "second";
    int64_t value2 = 100;
    std::string key3 = "third";
    bool value3 = false;
    std::string stringType = "string";
    std::string intType = "int";
    std::string boolType = "bool";

    db.store(key1, value1, stringType);
    db.store(key2, value2, intType);
    db.store(key3, value3, boolType);

    const KVMap& allEntries = db.getAllEntries();

    EXPECT_EQ(allEntries.size(), 3);
    EXPECT_TRUE(allEntries.find(key1) != allEntries.end());
    EXPECT_TRUE(allEntries.find(key2) != allEntries.end());
    EXPECT_TRUE(allEntries.find(key3) != allEntries.end());

    db.destroy();
}

TEST_F(CLIIntegrationTest, SetSaveReloadGet) {
    std::string dbName = "save_reload_db";
    testDatabases.push_back(dbName);

    {
        KVDatabase db = KVDatabase::createEmptyDb(dbName);

        std::string key = "persistent_key";
        std::string value = "persistent_value";
        std::string type = "string";
        db.store(key, value, type);
    }

    KVDatabase reloadedDb = KVDatabase::load(dbName);

    std::string key = "persistent_key";
    ValueTypeVariant retrieved = reloadedDb.get(key);
    EXPECT_EQ(std::get<std::string>(retrieved), "persistent_value");

    reloadedDb.destroy();
}

TEST_F(CLIIntegrationTest, DeleteSaveReloadList) {
    std::string dbName = "delete_save_reload_db";
    testDatabases.push_back(dbName);

    {
        KVDatabase db = KVDatabase::createEmptyDb(dbName);

        std::string key1 = "keep_me";
        std::string value1 = "kept";
        std::string key2 = "delete_me";
        std::string value2 = "removed";
        std::string key3 = "also_keep";
        std::string value3 = "also_kept";
        std::string type = "string";

        db.store(key1, value1, type);
        db.store(key2, value2, type);
        db.store(key3, value3, type);

        db.deleteKey(key2);
    }

    KVDatabase reloadedDb = KVDatabase::load(dbName);

    const KVMap& allEntries = reloadedDb.getAllEntries();

    EXPECT_EQ(allEntries.size(), 2);
    EXPECT_TRUE(allEntries.find("keep_me") != allEntries.end());
    EXPECT_TRUE(allEntries.find("also_keep") != allEntries.end());
    EXPECT_TRUE(allEntries.find("delete_me") == allEntries.end());

    std::string key2 = "delete_me";
    EXPECT_THROW(reloadedDb.get(key2), std::runtime_error);

    reloadedDb.destroy();
}

TEST_F(CLIIntegrationTest, GetNonExistentKey) {
    std::string dbName = "nonexistent_key_db";
    testDatabases.push_back(dbName);

    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    std::string key = "does_not_exist";

    EXPECT_THROW(db.get(key), std::runtime_error);

    db.destroy();
}

TEST_F(CLIIntegrationTest, DeleteNonExistentKey) {
    std::string dbName = "delete_nonexistent_db";
    testDatabases.push_back(dbName);

    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    std::string key = "does_not_exist";

    EXPECT_THROW(db.deleteKey(key), std::runtime_error);

    db.destroy();
}

TEST_F(CLIIntegrationTest, AllTypesRoundTrip) {
    std::string dbName = "roundtrip_db";
    testDatabases.push_back(dbName);

    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    std::string stringInput = "test_string";
    std::string stringKey = "string_key";
    std::string stringType = "string";
    db.store(stringKey, stringInput, stringType);
    ValueTypeVariant stringRetrieved = db.get(stringKey);
    std::string stringOutput = TypeConverter::variantToString(stringRetrieved);
    EXPECT_EQ(stringOutput, stringInput);
    EXPECT_EQ(TypeConverter::getTypeName(stringRetrieved), "string");

    std::string intInput = "-999";
    int64_t intValue = TypeConverter::toInt64(intInput);
    std::string intKey = "int_key";
    std::string intType = "int";
    db.store(intKey, intValue, intType);
    ValueTypeVariant intRetrieved = db.get(intKey);
    std::string intOutput = TypeConverter::variantToString(intRetrieved);
    EXPECT_EQ(intOutput, intInput);
    EXPECT_EQ(TypeConverter::getTypeName(intRetrieved), "int");

    std::string boolInput = "true";
    bool boolValue = TypeConverter::toBool(boolInput);
    std::string boolKey = "bool_key";
    std::string boolType = "bool";
    db.store(boolKey, boolValue, boolType);
    ValueTypeVariant boolRetrieved = db.get(boolKey);
    std::string boolOutput = TypeConverter::variantToString(boolRetrieved);
    EXPECT_EQ(boolOutput, boolInput);
    EXPECT_EQ(TypeConverter::getTypeName(boolRetrieved), "bool");

    std::string doubleInput = "2.71828";
    double doubleValue = TypeConverter::toDouble(doubleInput);
    std::string doubleKey = "double_key";
    std::string doubleType = "double";
    db.store(doubleKey, doubleValue, doubleType);
    ValueTypeVariant doubleRetrieved = db.get(doubleKey);
    std::string doubleOutput = TypeConverter::variantToString(doubleRetrieved);
    EXPECT_EQ(std::stod(doubleOutput), std::stod(doubleInput));
    EXPECT_EQ(TypeConverter::getTypeName(doubleRetrieved), "double");

    std::string uintInput = "54321";
    uint64_t uintValue = TypeConverter::toUInt64(uintInput);
    std::string uintKey = "uint_key";
    std::string uintType = "uint";
    db.store(uintKey, uintValue, uintType);
    ValueTypeVariant uintRetrieved = db.get(uintKey);
    std::string uintOutput = TypeConverter::variantToString(uintRetrieved);
    EXPECT_EQ(uintOutput, uintInput);
    EXPECT_EQ(TypeConverter::getTypeName(uintRetrieved), "uint");

    db.destroy();
}

TEST_F(CLIIntegrationTest, StoreMultiWordString) {
    std::string dbName = "multiword_db";
    testDatabases.push_back(dbName);

    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    std::string key = "sentence_key";
    std::string value = "hello world from the CLI";
    std::string type = "string";
    db.store(key, value, type);

    ValueTypeVariant retrieved = db.get(key);
    EXPECT_EQ(std::get<std::string>(retrieved), value);

    db.destroy();
}
