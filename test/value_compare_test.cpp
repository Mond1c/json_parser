#include <json.h>
#include <gtest//gtest.h>

namespace {
    struct value_compare_test : ::testing::Test {

    };
}

TEST_F(value_compare_test, number_test) {
    auto v1 = json::Value::new_value(std::uint64_t(30));
    auto v2 = json::Value::new_value(std::uint64_t(30));
    auto v3 = json::Value::new_value("Tom");
    ASSERT_EQ(v1, v2);
    ASSERT_NE(v1, v3);
}

TEST_F(value_compare_test, string_test) {
    auto v1 = json::Value::new_value(std::string("Tom"));
    auto v2 = json::Value::new_value(std::string("Tom"));
    auto v3 = json::Value::new_value(std::string("Dan"));
    ASSERT_EQ(v1, v2);
    ASSERT_NE(v1, v3);
}

TEST_F(value_compare_test, boolean_test) {
    auto v1 = json::Value::new_value(true);
    auto v2 = json::Value::new_value(true);
    auto v3 = json::Value::new_value(false);
    ASSERT_EQ(v1, v2);
    ASSERT_NE(v1, v3);
}

TEST_F(value_compare_test, null_test) {
    auto v1 = json::Value::new_value(nullptr);
    auto v2 = json::Value::new_value(nullptr);
    ASSERT_EQ(v1, v2);
}

TEST_F(value_compare_test, array_test) {
    std::vector<std::string> arr = {"Tom", "Jake"};
    auto v1 = json::Value::new_value(arr);
    auto v2 = json::Value::new_value(arr);
    auto v3 = json::Value::new_value(std::vector<std::string>{"Tom"});
    ASSERT_EQ(v1, v2);
    ASSERT_NE(v1, v3);
}

TEST_F(value_compare_test, object_test) {
    std::unordered_map<std::string, std::shared_ptr<json::Value>> object;
    object["name"] = std::make_shared<json::Value>(json::Value::new_value(std::string("Tom")));
    object["age"] = std::make_shared<json::Value>(json::Value::new_value(std::uint64_t(30)));
    auto v1 = json::Value::new_value(object);
    auto v2 = json::Value::new_value(object);
    object["test"] = std::make_shared<json::Value>(json::Value::new_value(true));
    auto v3 = json::Value::new_value(object);
    ASSERT_EQ(v1, v2);
    ASSERT_NE(v1, v3);
}