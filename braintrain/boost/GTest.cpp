#include "gtest/gtest.h"

// 1. Define the Test Fixture.
struct MyDatabaseTest : public ::testing::Test {
    // Common objects for tests
    std::string db_conn_;
    std::string test_user_;

    // Setup method, called before each TEST_F
    void SetUp() override {
        std::cout << "setup" << std::endl;
        db_conn_ = "my-connection";
        // db_conn_.clearTables(); // Ensure clean state
        test_user_ = "usr";
        // db_conn_.addUser(test_user_);
    }

    // Teardown method, called after each TEST_F
    void TearDown() override {
        std::cout << "teardown" << std::endl;
        // db_conn_.disconnect();
    }

    // Constructor/Destructor can also be used for setup/teardown
    MyDatabaseTest() : db_conn_("config.ini") {}
    ~MyDatabaseTest() override = default;
};

// 2. Write tests using TEST_F
TEST_F(MyDatabaseTest, CanRetrieveAddedUser) {
    // Arrange (already done in SetUp, test_user_ is available)

    // Act
    std::string retrieved_user = test_user_ + "user1";
    // Assert
    EXPECT_EQ(test_user_ + "user1", retrieved_user);
};