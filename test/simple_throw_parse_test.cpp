#include <json.h>
#include <gtest/gtest.h>

namespace {
    struct simple_throw_parse_test : ::testing::Test {

    };
}

TEST_F(simple_throw_parse_test, missed_opening_bracket) {
    const std::string raw_json = "\"age\": 30}";
    std::stringstream ss(raw_json);
    ASSERT_THROW(json::parse_json(ss), std::runtime_error);
}

TEST_F(simple_throw_parse_test, missed_closing_bracket) {
    const std::string raw_json = "{\"age\": 30";
    std::stringstream ss(raw_json);
    ASSERT_THROW(json::parse_json(ss), std::runtime_error);
}

TEST_F(simple_throw_parse_test, missed_key_start) {
    const std::string raw_json = "{age\": 30}";
    std::stringstream ss(raw_json);
    ASSERT_THROW(json::parse_json(ss), std::runtime_error);
}

TEST_F(simple_throw_parse_test, missed_key_end) {
    const std::string raw_json = "{\"age: 30}";
    std::stringstream ss(raw_json);
    ASSERT_THROW(json::parse_json(ss), std::runtime_error);
}

TEST_F(simple_throw_parse_test, missed_key) {
    const std::string raw_json = "{:30}";
    std::stringstream ss(raw_json);
    ASSERT_THROW(json::parse_json(ss), std::runtime_error);
}

TEST_F(simple_throw_parse_test, missed_value) {
    const std::string raw_json = "{\"age\": }";
    std::stringstream ss(raw_json);
    ASSERT_THROW(json::parse_json(ss), std::runtime_error);
}
