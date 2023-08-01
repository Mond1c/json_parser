#include <json.h>
#include <gtest/gtest.h>

namespace {
    struct simple_test : ::testing::Test {

    };
}

TEST_F(simple_test, number_test) {
    const std::string raw_json = "{\"number\": 30}";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("number"));
    ASSERT_TRUE(obj["number"].is_uint64());
    ASSERT_EQ(obj["number"].to_uint64(), 30);
}

TEST_F(simple_test, string_test) {
    const std::string raw_json = R"({"name": "Jake"})";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("name"));
    ASSERT_TRUE(obj["name"].is_string());
    ASSERT_EQ(obj["name"].to_string(), "Jake");
}

TEST_F(simple_test, boolean_test) {
    const std::string raw_json = "{\"value\": true}";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("value"));
    ASSERT_TRUE(obj["value"].is_boolean());
    ASSERT_TRUE(obj["value"].to_boolean());
}

TEST_F(simple_test, null_test) {
    const std::string raw_json = "{\"value\": null}";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("value"));
    ASSERT_TRUE(obj["value"].is_null());
    ASSERT_EQ(obj["value"].to_null(), nullptr);
}

TEST_F(simple_test, array_test) {
    const std::vector<std::string> array = {"Jake", "Tom"};
    const std::string raw_json = R"({"array": ["Jake", "Tom"]})";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("array"));
    ASSERT_TRUE(obj["array"].is_array());
    ASSERT_EQ(obj["array"].to_array(), array);
}


TEST_F(simple_test, object_test) {
    std::map<std::string, std::shared_ptr<json::Value>> object;
    object["name"] = std::make_shared<json::Value>(json::Value::new_value(std::string("Tom")));
    object["age"] = std::make_shared<json::Value>(json::Value::new_value(std::uint64_t(30)));
    const std::string raw_json = R"({ "person": {"name": "Tom", "age": 30}})";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("person"));
    ASSERT_TRUE(obj["person"].is_object());
    const std::map<std::string, std::shared_ptr<json::Value>> sorted_data(obj["person"].to_object().begin(),
                                                                          obj["person"].to_object().end());
    ASSERT_EQ(object.size(), sorted_data.size());
    for (const auto &item: object) {
        ASSERT_TRUE(sorted_data.count(item.first));
        ASSERT_EQ(*item.second, *sorted_data.at(item.first));
    }
}