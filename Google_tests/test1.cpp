//
// Created by Nick Place on 1/14/24.
//
#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include "../src/KVDatabase/KVDatabase.h"

namespace fs = std::filesystem;
// A simple test: Test equality of two values
TEST(SimpleTest, Equality) {
EXPECT_EQ(1, 1);
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