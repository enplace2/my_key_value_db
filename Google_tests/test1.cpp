//
// Created by Nick Place on 1/14/24.
//
#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include "../src/KVDatabase/KVDatabase.h"
#include "../src/FSManager/FSManager.h"

namespace fs = std::filesystem;

TEST(FSManagerTest, createBaseDirectory){
    /* Story
        * [Who] As a software engineer,
        * [What] I need to create the base directory for the program
        * [Value] So I can store databases and other related files
        */
    bool baseDirExists = FSManager::createBaseDirectory();
    EXPECT_TRUE(baseDirExists);
}


TEST(FSManagerTest, createDbDirectory){
    /* Story
        * [Who] As a software engineer,
        * [What] I need to create a directory for a given database
        * [Value] So I can store and retrieve database files
        */

    std::string testDbName = "test_db";
    std::string directoryPath = FSManager::createDbDirectory(testDbName);
    std::string storeFilePath = FSManager::createDbStoreFile(testDbName);
    EXPECT_TRUE(fs::is_directory(fs::status(directoryPath)));
    EXPECT_TRUE(fs::is_regular_file(storeFilePath));
}

TEST(DBCoreTest, createEmptyDB){
    /* Story
     * [Who] As a user,
     * [What] I need to create a new db
     * [Value] So I can store and retrieve data
     */

    std::string dbName = "test_db_2";
    KVDatabase db = KVDatabase::createEmptyDb(dbName);
    std::string directoryPath = db.getDirectory();
    // We know that it is successful when:
    // 1.  A valid directory for the database returned

    EXPECT_TRUE(fs::is_directory(fs::status(directoryPath)));

    //2. The directory only has one file
    //3. The store file exists
    int fileCount = 0;
    bool kvStoreExists = false;

    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            fileCount++;
            if (entry.path().filename() == "store.kv") {
                kvStoreExists = true;
            } else {
                kvStoreExists = false;
                break; // Found a file that is not "store.kv"
            }
        }
    }

    EXPECT_EQ(fileCount, 1);

    EXPECT_TRUE(kvStoreExists);


    /* Story
   * [Who] As a user,
   * [What] I need to destroy a database
   * [Value] So I can manage my db system appropriately
   */

    //We know that it is successful when:
    //1. the db directory no longer exists
    db.destroy();
    EXPECT_FALSE(fs::exists(directoryPath));
}

TEST(DBCoreTest, storeAndRetrieveValues){
    /* Story
     * [Who] As user
     * [What] I need to store and retrieve a value with a simple key
     * [Value] So that data can persist and be used at a later time
     */

    //We know that it is successful when:
    //1. we can write to the database, and then show that retrieving the value
    // returns the same value that we wrote to the db
    std::string dbName = "test_db";
    KVDatabase emptyDb = KVDatabase::createEmptyDb(dbName);
    KVDatabase db = KVDatabase::load(dbName);
    std::string stringKey = "string_key";
    std::string value = "some_value";
    std::string type = "string";
    db.store(stringKey, value, type);

    ValueTypeVariant dbValue = db.get(key);
    std::string valueString = std::get<std::string>(dbValue);
    EXPECT_EQ(valueString, value);
    db.destroy();
}