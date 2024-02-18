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
    int64_t intValue = 5987987;
    std::string intType = "int";
    db.store(intKey, intValue, intType);

    ValueTypeVariant pIntVal = db.get(intKey);
    int64_t castPIntVal = std::get<int64_t>(pIntVal);
    EXPECT_EQ(castPIntVal, intValue);

    /** Type Int (negative) **/
    std::string negativeKey = "negative_int_key";
    int64_t negativeIntValue = -54218;
    db.store(negativeKey, negativeIntValue, intType);

    ValueTypeVariant nIntVal = db.get(negativeKey);
    int64_t castNIntVal = std::get<int64_t>(nIntVal);
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
    int64_t castLoadInt = std::get<int64_t>(loadInt);
    EXPECT_EQ(castLoadInt, intValue);

    /** Type Int (negative) **/

    ValueTypeVariant loadNInt = loadedDb.get(negativeKey);
    int64_t castLoadNInt = std::get<int64_t>(loadNInt);
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


    db.destroy();
}


TEST(DBCoreTest, StoreAndRetrieveNestedKVMaps) {
    /** Story
     * [Who] As a user,
     * [What] I need to store and retrieve nested key-value maps
     * [Value] So that complex data structures can be persisted and utilized.
     */

    /** Setup **/
    std::string dbName = "nested_kv_map_db";
    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    // Define nested KV map
    KVMap nestedMap;
    nestedMap["inner_string_key"] = {"inner_string_value", "string"};
    nestedMap["inner_int_key"] = {123, "int"};
    KVMap outerMap;
    outerMap["nested_map_key"] = {nestedMap, "map"};

    // Store the nested KV map in the database
    std::string mapKey = "outer_map_key";
    std::string type = "map";
    db.store(mapKey, outerMap, type);

    /** Test Retrieval **/
    // Retrieve the stored nested KV map
    ValueTypeVariant retrievedValueVariant = db.get(mapKey);
    auto retrievedMap = std::get<KVMap>(retrievedValueVariant);

    // Verify the nested structure
    ASSERT_TRUE(retrievedMap.find("nested_map_key") != retrievedMap.end());
    auto innerMapVariant = retrievedMap["nested_map_key"];
    auto retrievedInnerMap = std::get<KVMap>(innerMapVariant.value);

    // Verify contents of the inner map
    ASSERT_TRUE(retrievedInnerMap.find("inner_string_key") != retrievedInnerMap.end());
    auto retrievedStringValueVariant = retrievedInnerMap["inner_string_key"];
    std::string retrievedStringValue = std::get<std::string>(retrievedStringValueVariant.value);
    EXPECT_EQ(retrievedStringValue, "inner_string_value");

    ASSERT_TRUE(retrievedInnerMap.find("inner_int_key") != retrievedInnerMap.end());
    auto retrievedIntValueVariant = retrievedInnerMap["inner_int_key"];
    int retrievedIntValue = std::get<int64_t>(retrievedIntValueVariant.value);
    EXPECT_EQ(retrievedIntValue, 123);

    db.destroy();
}

TEST(DBCoreTest, StoreAndRetrieveDeeplyNestedKVMaps) {
    /** Story
     * [Who] As a user,
     * [What] I need to store and retrieve deeply nested key-value maps
     * [Value] So that very complex data structures can be persisted and utilized.
     */

    /** Setup **/
    std::string dbName = "deeply_nested_kv_map_db";
    KVDatabase db = KVDatabase::createEmptyDb(dbName);

    // Define deeply nested KV map
    KVMap innermostMap;
    innermostMap["deepest_string_key"] = {"deepest_string_value", "string"};
    innermostMap["deepest_int_key"] = {42, "int"};

    KVMap nestedMap;
    nestedMap["inner_string_key"] = {"inner_string_value", "string"};
    nestedMap["inner_int_key"] = {123, "int"};
    nestedMap["inner_nested_map"] = {innermostMap, "map"}; // Third level of nesting

    KVMap outerMap;
    outerMap["nested_map_key"] = {nestedMap, "map"};

    // Store the deeply nested KV map in the database
    std::string mapKey = "outer_map_key";
    std::string type = "map";
    db.store(mapKey, outerMap, type);

    /** Test Retrieval **/
    // Retrieve the stored deeply nested KV map
    ValueTypeVariant retrievedValueVariant = db.get(mapKey);
    auto retrievedMap = std::get<KVMap>(retrievedValueVariant);

    // Verify the nested structure
    ASSERT_TRUE(retrievedMap.find("nested_map_key") != retrievedMap.end());
    auto innerMapVariant = retrievedMap["nested_map_key"];
    auto retrievedInnerMap = std::get<KVMap>(innerMapVariant.value);

    // Verify contents of the nested map
    ASSERT_TRUE(retrievedInnerMap.find("inner_nested_map") != retrievedInnerMap.end());
    auto retrievedInnermostMapVariant = retrievedInnerMap["inner_nested_map"];
    auto retrievedInnermostMap = std::get<KVMap>(retrievedInnermostMapVariant.value);

    // Verify contents of the innermost map
    ASSERT_TRUE(retrievedInnermostMap.find("deepest_string_key") != retrievedInnermostMap.end());
    auto retrievedDeepestStringValueVariant = retrievedInnermostMap["deepest_string_key"];
    std::string retrievedDeepestStringValue = std::get<std::string>(retrievedDeepestStringValueVariant.value);
    EXPECT_EQ(retrievedDeepestStringValue, "deepest_string_value");

    ASSERT_TRUE(retrievedInnermostMap.find("deepest_int_key") != retrievedInnermostMap.end());
    auto retrievedDeepestIntValueVariant = retrievedInnermostMap["deepest_int_key"];
    int64_t retrievedDeepestIntValue = std::get<int64_t>(retrievedDeepestIntValueVariant.value);
    EXPECT_EQ(retrievedDeepestIntValue, 42);

    db.destroy();
}

