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


TEST(FSManagerTest, createDBDirectory){
    /* Story
        * [Who] As a software engineer,
        * [What] I need to create a directory for a given database
        * [Value] So I can store and retrieve database files
        */

    std::string testDbName = "test_db";
    bool dbDirectoryExists = FSManager::createDBDirectory(testDbName);
    EXPECT_TRUE(dbDirectoryExists);
}

TEST(DBCoreTest, createDB){
    /* Story
     * [Who] As a software engineer,
     * [What] I need to create a new db
     * [Value] So I can store and retrieve data
     */


    std::string dbName = "testDb";
    KVDatabase db = KVDatabase::createEmptyDb(dbName);
    std::string directory = db.getDirectory();
    // We know that it is successful when:
    // 1.  A valid directory for the database returned

    EXPECT_TRUE(fs::is_directory(fs::status(directory)));

    //2. The directory is empty
    const auto& p = fs::directory_iterator(directory);
    EXPECT_TRUE(p == fs::end(p));

    EXPECT_TRUE(p == end(p));
}