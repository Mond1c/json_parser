#include <json.h>
#include <gtest/gtest.h>

namespace {
    struct simple_dump_test : ::testing::Test {

    };
}

TEST_F(simple_dump_test, number_test) {
    std::stringstream ss;
    json::Json::json_object obj;
    obj["number"] = std::make_shared<json::Value>(json::Value::new_value(std::uint64_t(30)));
    json::dump_json(ss, json::Json(obj));
    ASSERT_EQ(ss.str(), "{\n\"number\": 30\n}\n");
}

TEST_F(simple_dump_test, string_test) {
    std::stringstream ss;
    json::Json::json_object obj;
    obj["name"] = std::make_shared<json::Value>(json::Value::new_value(std::string("Jake")));
    json::dump_json(ss, json::Json(obj));
    ASSERT_EQ(ss.str(), "{\n\"name\": \"Jake\"\n}\n");
}

TEST_F(simple_dump_test, boolean_test) {
    std::stringstream ss;
    json::Json::json_object obj;
    obj["value"] = std::make_shared<json::Value>(json::Value::new_value(false));
    json::dump_json(ss, json::Json(obj));
    ASSERT_EQ(ss.str(), "{\n\"value\": false\n}\n");
}

TEST_F(simple_dump_test, null_test) {
    std::stringstream ss;
    json::Json::json_object obj;
    obj["value"] = std::make_shared<json::Value>(json::Value::new_value(nullptr));
    json::dump_json(ss, json::Json(obj));
    ASSERT_EQ(ss.str(), "{\n\"value\": null\n}\n");
}

TEST_F(simple_dump_test, array_test) {
    std::stringstream ss;
    json::Json::json_object obj;
    obj["array"] = std::make_shared<json::Value>(json::Value::new_value(std::vector<std::shared_ptr<json::Value>>{
            std::make_shared<json::Value>(json::Value::new_value(std::string("Tom"))),
            std::make_shared<json::Value>(json::Value::new_value(std::string("Jake")))}));
    json::dump_json(ss, json::Json(obj));
    ASSERT_EQ(ss.str(), "{\n\"array\": [\"Tom\", \"Jake\"]\n}\n");
}

TEST_F(simple_dump_test, object_test) {
    std::stringstream ss;
    json::Json::json_object obj;
    json::Json::json_object person;
    person["name"] = std::make_shared<json::Value>(json::Value::new_value(std::string("Jake")));
    person["age"] = std::make_shared<json::Value>(json::Value::new_value(std::uint64_t(30)));
    obj["person"] = std::make_shared<json::Value>(json::Value::new_value(person));
    json::dump_json(ss, json::Json(obj));
    ASSERT_EQ(ss.str(), "{\n\"person\": {\n\"age\": 30,\n\"name\": \"Jake\"\n}\n\n}\n");
}