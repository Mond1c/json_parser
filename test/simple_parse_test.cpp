#include <json.h>
#include <gtest/gtest.h>

namespace {
    struct simple_parse_test : ::testing::Test {

    };
}

TEST_F(simple_parse_test, number_test) {
    const std::string raw_json = "{\"number\": 30}";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("number"));
    ASSERT_TRUE(obj["number"].is_uint64());
    ASSERT_EQ(obj["number"].to_uint64(), 30);
}

TEST_F(simple_parse_test, string_test) {
    const std::string raw_json = R"({"name": "Jake"})";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("name"));
    ASSERT_TRUE(obj["name"].is_string());
    ASSERT_EQ(obj["name"].to_string(), "Jake");
}

TEST_F(simple_parse_test, boolean_test) {
    const std::string raw_json = "{\"value\": true}";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("value"));
    ASSERT_TRUE(obj["value"].is_boolean());
    ASSERT_TRUE(obj["value"].to_boolean());
}

TEST_F(simple_parse_test, null_test) {
    const std::string raw_json = "{\"value\": null}";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("value"));
    ASSERT_TRUE(obj["value"].is_null());
    ASSERT_EQ(obj["value"].to_null(), nullptr);
}

TEST_F(simple_parse_test, array_test_string) {
    const std::vector<std::shared_ptr<json::Value>> array = {
            std::make_shared<json::Value>(json::Value::new_value(std::string("Jake"))),
            std::make_shared<json::Value>(json::Value::new_value(std::string("Tom")))};
    const std::string raw_json = R"({"array": ["Jake", "Tom"]})";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("array"));
    ASSERT_TRUE(obj["array"].is_array());
    ASSERT_EQ(obj["array"].to_array().size(), array.size());
    for (std::size_t i = 0; i < array.size(); i++) {
        ASSERT_EQ(*array[i], *obj["array"].to_array()[i]);
    }
}


TEST_F(simple_parse_test, array_test_number) {
    const std::vector<std::shared_ptr<json::Value>> array = {
            std::make_shared<json::Value>(json::Value::new_value(std::uint64_t(1))),
            std::make_shared<json::Value>(json::Value::new_value(std::uint64_t(2)))};
    const std::string raw_json = R"({"array": [1, 2]})";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("array"));
    ASSERT_TRUE(obj["array"].is_array());
    ASSERT_EQ(obj["array"].to_array().size(), array.size());
    for (std::size_t i = 0; i < array.size(); i++) {
        ASSERT_EQ(*array[i], *obj["array"].to_array()[i]);
    }
}

TEST_F(simple_parse_test, array_test_boolean) {
    const std::vector<std::shared_ptr<json::Value>> array = {
            std::make_shared<json::Value>(json::Value::new_value(true)),
            std::make_shared<json::Value>(json::Value::new_value(false))};
    const std::string raw_json = R"({"array": [true, false]})";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("array"));
    ASSERT_TRUE(obj["array"].is_array());
    ASSERT_EQ(obj["array"].to_array().size(), array.size());
    for (std::size_t i = 0; i < array.size(); i++) {
        ASSERT_EQ(*array[i], *obj["array"].to_array()[i]);
    }
}

TEST_F(simple_parse_test, array_test_null) {
    const std::vector<std::shared_ptr<json::Value>> array = {
            std::make_shared<json::Value>(json::Value::new_value(nullptr)),
            std::make_shared<json::Value>(json::Value::new_value(nullptr))};
    const std::string raw_json = R"({"array": [null, null]})";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("array"));
    ASSERT_TRUE(obj["array"].is_array());
    ASSERT_EQ(obj["array"].to_array().size(), array.size());
    for (std::size_t i = 0; i < array.size(); i++) {
        ASSERT_EQ(*array[i], *obj["array"].to_array()[i]);
    }
}

TEST_F(simple_parse_test, array_test_array) {
    const std::vector<std::shared_ptr<json::Value>> inner = {
            std::make_shared<json::Value>(json::Value::new_value(std::string("Jake"))),
            std::make_shared<json::Value>(json::Value::new_value(std::string("Tom")))};
    const std::vector<std::shared_ptr<json::Value>> array = {
            std::make_shared<json::Value>(json::Value::new_value(inner)),
            std::make_shared<json::Value>(json::Value::new_value(inner))};
    const std::string raw_json = R"({"array": [["Jake", "Tom"], ["Jake", "Tom"]]})";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("array"));
    ASSERT_TRUE(obj["array"].is_array());
    ASSERT_EQ(obj["array"].to_array().size(), array.size());
    for (std::size_t i = 0; i < array.size(); i++) {
        ASSERT_TRUE(array[i]->is_array());
        ASSERT_EQ(obj["array"].to_array()[i]->to_array().size(), inner.size());
        for (std::size_t j = 0; j < inner.size(); j++) {
            ASSERT_EQ(*inner[j], *obj["array"].to_array()[i]->to_array()[j]);
        }
    }
}

TEST_F(simple_parse_test, array_test_object) {
    std::unordered_map<std::string, std::shared_ptr<json::Value>> object;
    object["name"] = std::make_shared<json::Value>(json::Value::new_value(std::string("Tom")));
    object["age"] = std::make_shared<json::Value>(json::Value::new_value(std::uint64_t(30)));
    const std::vector<std::shared_ptr<json::Value>> array = {
            std::make_shared<json::Value>(json::Value::new_value(object)),
            std::make_shared<json::Value>(json::Value::new_value(object))};
    const std::string raw_json = R"({"array": [{"name": "Tom", "age": 30}, {"name": "Tom", "age": 30}]})";
    std::stringstream ss(raw_json);
    const json::Json obj = json::parse_json(ss);
    ASSERT_TRUE(obj.contains_key("array"));
    ASSERT_TRUE(obj["array"].is_array());
    ASSERT_EQ(obj["array"].to_array().size(), array.size());
    for (std::size_t i = 0; i < array.size(); i++) {
        ASSERT_TRUE(array[i]->is_object());
        ASSERT_EQ(obj["array"].to_array()[i]->to_object().size(), object.size());
        for (const auto &item: object) {
            ASSERT_TRUE(obj["array"].to_array()[i]->to_object().count(item.first));
            ASSERT_EQ(*obj["array"].to_array()[i]->to_object()[item.first], *item.second);
        }
    }
}

TEST_F(simple_parse_test, object_test) {
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