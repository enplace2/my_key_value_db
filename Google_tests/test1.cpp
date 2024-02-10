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
    /**
     * Story
     * [Who] As user
     * [What] I need to store and retrieve a value with a simple key
     * [Value] So that data can persist and be used at a later time
     */

    /*** We know that it is successful when: ***/

    /*****************************************************************************************
     * 1. We can write to the database, and then show that retrieving the value returns the
     * same value that we wrote to the db
    *****************************************************************************************/

   /**
    * We will do this for string, int (positive and negative), bool (true and false),
    * double (positive and negative), uint64
    * */


    std::string dbName = "test_db";
    KVDatabase emptyDb = KVDatabase::createEmptyDb(dbName);
    KVDatabase db = KVDatabase::load(dbName);

    /** Type String **/

    std::string stringKey = "string_key";
    std::string value = "some_value";
    std::string type = "string";
    db.store(stringKey, value, type);

    ValueTypeVariant dbValue = db.get(stringKey);
    std::string valueString = std::get<std::string>(dbValue);
    EXPECT_EQ(valueString, value);

    /** Type Int (positive) **/
    std::string intKey = "int_key";
    int intValue = 5987987;
    std::string intType = "int";
    db.store(intKey, intValue, intType);

    ValueTypeVariant pIntVal = db.get(intKey);
    int castPIntVal = std::get<int>(pIntVal);
    EXPECT_EQ(castPIntVal, intValue);

    /** Type Int (negative) **/
    std::string negativeKey = "negative_int_key";
    int negativeIntValue = -54218;
    db.store(negativeKey, negativeIntValue, intType);

    ValueTypeVariant nIntVal = db.get(negativeKey);
    int castNIntVal = std::get<int>(nIntVal);
    EXPECT_EQ(castNIntVal, negativeIntValue);

    /** Type bool (true) **/
    std::string trueKey = "true_key";
    bool trueVal = true;
    std::string boolType = "bool";
    db.store(trueKey, trueVal, boolType);

    ValueTypeVariant tbool = db.get(trueKey);
    bool castTbool = std::get<bool>(tbool);
    EXPECT_EQ(castTbool, trueVal);

    /** Type bool (false) **/
    std::string falseKey = "false_key";
    bool falseVal = false;
    db.store(falseKey, falseVal, boolType);

    ValueTypeVariant fbool = db.get(falseKey);
    bool castFbool = std::get<bool>(fbool);
    EXPECT_EQ(castFbool, falseVal);

    /** Type double (positive) **/
    std::string doubleKey = "double_key";
    double doubleVal = 1234567.12;
    std::string doubleType = "double";
    db.store(doubleKey, doubleVal, doubleType);

    ValueTypeVariant pDouble = db.get(doubleKey);
    double castPdouble = std::get<double>(pDouble);
    EXPECT_EQ(castPdouble, doubleVal);



    /** Type double (negative) **/
    std::string negDoubleKey = "neg_double_key";
    double negDoubleVal = -987654.987654321;
    db.store(negDoubleKey, negDoubleVal, doubleType);

    ValueTypeVariant nDouble = db.get(negDoubleKey);
    double castNdouble = std::get<double>(nDouble);
    EXPECT_EQ(castNdouble, negDoubleVal);

    /** Type uint64 **/
    std::string uintKey = "uint_key";
    uint64_t uintVal = 1234545686;
    std::string uintType = "uint";
    db.store(uintKey, uintVal, uintType);

    ValueTypeVariant uintdbVal = db.get(uintKey);
    uint64_t castUint = std::get<uint64_t>(uintdbVal);
    EXPECT_EQ(castUint, uintVal);


    /***********************************************************************
     * 2. We can load that database and retrieve the saved value
     **********************************************************************/
    KVDatabase loadedDb = KVDatabase::load(dbName);

    /** Type String **/
    ValueTypeVariant dbValueFromLoaded = loadedDb.get(stringKey);
    std::string loadedValueString = std::get<std::string>(dbValueFromLoaded);
    EXPECT_EQ(loadedValueString, value);

    /** Type Int (positive) **/
    ValueTypeVariant loadInt = loadedDb.get(intKey);
    int castLoadInt = std::get<int>(loadInt);
    EXPECT_EQ(castLoadInt, intValue);

    /** Type Int (negative) **/

    ValueTypeVariant loadNInt = loadedDb.get(negativeKey);
    int castLoadNInt = std::get<int>(loadNInt);
    EXPECT_EQ(castLoadNInt, negativeIntValue);


    /** Type bool (true) **/
    ValueTypeVariant loadTBool = loadedDb.get(trueKey);
    bool castLoadTBool = std::get<bool>(loadTBool);
    EXPECT_EQ(castLoadTBool, trueVal);

    /** Type bool (false) **/
    ValueTypeVariant loadFBool = loadedDb.get(falseKey);
    bool castLoadFBool = std::get<bool>(loadFBool);
    EXPECT_EQ(castLoadFBool, falseVal);

    /** Type double (positive) **/
    ValueTypeVariant loadPdub = loadedDb.get(doubleKey);
    double castLoadPdub = std::get<double>(loadPdub);
    EXPECT_EQ(castLoadPdub , doubleVal);

    /** Type double (negative) **/
    ValueTypeVariant loadNdub = loadedDb.get(negDoubleKey);
    double castLoadNdub = std::get<double>(loadNdub);
    EXPECT_EQ(castLoadNdub , negDoubleVal);

    /** Type uint64 **/

    ValueTypeVariant loadUint = loadedDb.get(uintKey);
    uint64_t castLoadUint= std::get<uint64_t>(loadUint);
    EXPECT_EQ(castLoadUint , uintVal);


    //db.destroy();
}