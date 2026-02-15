#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include "../src/KVDatabase/KVDatabase.h"
#include "../src/FSManager/FSManager.h"

namespace fs = std::filesystem;

class KVDatabaseDeleteTest : public ::testing::Test {
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

TEST_F(KVDatabaseDeleteTest, DeleteExistingKey) {
    std::string dbName = "delete_existing_key_db";
    testDatabases.push_back(dbName);

    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    std::string key = "test_key";
    std::string value = "test_value";
    std::string type = "string";
    db.store(key, value, type);

    ValueTypeVariant retrieved = db.get(key);
    EXPECT_EQ(std::get<std::string>(retrieved), value);

    db.deleteKey(key);

    EXPECT_THROW(db.get(key), std::runtime_error);

    db.destroy();
}

TEST_F(KVDatabaseDeleteTest, DeleteNonExistentKey) {
    std::string dbName = "delete_non_existent_key_db";
    testDatabases.push_back(dbName);

    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    std::string key = "non_existent_key";

    EXPECT_THROW(db.deleteKey(key), std::runtime_error);

    db.destroy();
}

TEST_F(KVDatabaseDeleteTest, DeleteKeyPersistence) {
    std::string dbName = "delete_persistence_db";
    testDatabases.push_back(dbName);

    {
        KVDatabase db = KVDatabase::createEmptyDb(dbName);

        std::string key1 = "key1";
        std::string value1 = "value1";
        std::string key2 = "key2";
        std::string value2 = "value2";
        std::string type = "string";

        db.store(key1, value1, type);
        db.store(key2, value2, type);

        db.deleteKey(key1);
    }

    KVDatabase reloadedDb = KVDatabase::load(dbName);

    std::string key1 = "key1";
    std::string key2 = "key2";

    EXPECT_THROW(reloadedDb.get(key1), std::runtime_error);

    ValueTypeVariant retrieved = reloadedDb.get(key2);
    EXPECT_EQ(std::get<std::string>(retrieved), "value2");

    reloadedDb.destroy();
}

TEST_F(KVDatabaseDeleteTest, DeleteKeyNotInGetAllEntries) {
    std::string dbName = "delete_getall_db";
    testDatabases.push_back(dbName);

    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    std::string key1 = "key1";
    std::string value1 = "value1";
    std::string key2 = "key2";
    int64_t value2 = 42;
    std::string stringType = "string";
    std::string intType = "int";

    db.store(key1, value1, stringType);
    db.store(key2, value2, intType);

    const KVMap& allEntries = db.getAllEntries();
    EXPECT_EQ(allEntries.size(), 2);
    EXPECT_TRUE(allEntries.find(key1) != allEntries.end());
    EXPECT_TRUE(allEntries.find(key2) != allEntries.end());

    db.deleteKey(key1);

    const KVMap& entriesAfterDelete = db.getAllEntries();
    EXPECT_EQ(entriesAfterDelete.size(), 1);
    EXPECT_TRUE(entriesAfterDelete.find(key1) == entriesAfterDelete.end());
    EXPECT_TRUE(entriesAfterDelete.find(key2) != entriesAfterDelete.end());

    db.destroy();
}

TEST_F(KVDatabaseDeleteTest, DeleteMultipleKeys) {
    std::string dbName = "delete_multiple_db";
    testDatabases.push_back(dbName);

    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    std::string key1 = "key1";
    std::string key2 = "key2";
    std::string key3 = "key3";
    std::string value = "value";
    std::string type = "string";

    db.store(key1, value, type);
    db.store(key2, value, type);
    db.store(key3, value, type);

    db.deleteKey(key1);
    db.deleteKey(key2);

    const KVMap& entries = db.getAllEntries();
    EXPECT_EQ(entries.size(), 1);

    EXPECT_THROW(db.get(key1), std::runtime_error);
    EXPECT_THROW(db.get(key2), std::runtime_error);

    ValueTypeVariant retrieved = db.get(key3);
    EXPECT_EQ(std::get<std::string>(retrieved), value);

    db.destroy();
}
