#include "include/json.h"
#include <stdexcept>
#include <memory>
#include <iostream>
#include <algorithm>

using namespace json;

#define CHECK_TYPE(func) if (!func()) throw std::runtime_error("JSON: Can't cast this value to your type.");

bool Value::is_uint64() const {
    return value_type == ValueType::Uint64;
}


bool Value::is_string() const {
    return value_type == ValueType::String;
}

bool Value::is_object() const {
    return value_type == ValueType::Object;
}

bool Value::is_array() const {
    return value_type == ValueType::Array;
}

bool Value::is_boolean() const {
    return value_type == ValueType::Boolean;
}

bool Value::is_null() const {
    return value_type == ValueType::Null;
}

std::uint64_t Value::to_uint64() const {
    CHECK_TYPE(is_uint64)
    return uint64_value;
}

const std::string &Value::to_string() const {
    CHECK_TYPE(is_string)
    return string_value;
}

const std::unordered_map<std::string, std::shared_ptr<Value>> &Value::to_object() const {
    CHECK_TYPE(is_object)
    return object_value;
}

const std::vector<std::string> &Value::to_array() const {
    CHECK_TYPE(is_array)
    return array_value;
}

bool Value::to_boolean() const {
    CHECK_TYPE(is_boolean)
    return boolean_value;
}

std::nullptr_t Value::to_null() const {
    CHECK_TYPE(is_null)
    return nullptr;
}

std::uint64_t &Value::to_uint64() {
    CHECK_TYPE(is_uint64)
    return uint64_value;
}

std::string &Value::to_string() {
    CHECK_TYPE(is_string)
    return string_value;
}

std::unordered_map<std::string, std::shared_ptr<Value>> &Value::to_object() {
    CHECK_TYPE(is_object)
    return object_value;
}

std::vector<std::string> &Value::to_array() {
    CHECK_TYPE(is_array)
    return array_value;
}

bool &Value::to_boolean() {
    CHECK_TYPE(is_boolean)
    return boolean_value;
}

const std::shared_ptr<Value> &Value::at(const std::string &key) const {
    if (!is_object()) {
        throw std::runtime_error("JSON: This is not an object!");
    }
    return to_object().at(key);
}

std::shared_ptr<Value> &Value::at(const std::string &key) {
    if (!is_object()) {
        throw std::runtime_error("JSON: This is not an object!");
    }
    return to_object()[key];
}

const Value &Value::operator[](const std::string &key) const {
    if (!is_object()) {
        throw std::runtime_error("JSON: This is not an object!");
    }
    return *to_object().at(key);
}

Value &Value::operator[](const std::string &key) {
    if (!is_object()) {
        throw std::runtime_error("JSON: This is not an object!");
    }
    return *to_object()[key];
}

bool json::operator==(const Value &lhs, const Value &rhs) {
    if (lhs.value_type != rhs.value_type) {
        return false;
    }
    if (lhs.is_object()) {
        if (lhs.to_object().size() != rhs.to_object().size()) {
            return false;
        }
        return std::all_of(lhs.to_object().begin(), lhs.to_object().end(),
                           [&rhs](const auto &item) {
                               return rhs.to_object().count(item.first) && *item.second == rhs[item.first];
                           });
    }
    if (lhs.is_array()) {
        if (lhs.to_array().size() != rhs.to_array().size()) {
            return false;
        }
        return std::equal(lhs.to_array().begin(), lhs.to_array().end(), rhs.to_array().begin());
    }
    if (lhs.is_uint64()) {
        return lhs.to_uint64() == rhs.to_uint64();
    }
    if (lhs.is_boolean()) {
        return lhs.to_boolean() == rhs.to_boolean();
    }
    if (lhs.is_string()) {
        return lhs.to_string() == rhs.to_string();
    }
    return true;
}

bool Json::contains_key(const std::string &key) const {
    return object.count(key);
}

const Value &Json::operator[](const std::string &key) const {
    if (!contains_key(key)) {
        throw std::runtime_error("This key doesn't exist");
    }
    return *object.at(key);
}

Value &Json::operator[](const std::string &key) {
    if (!contains_key(key)) {
        object[key] = std::make_shared<Value>(Value::new_value(nullptr));
    }
    return *object[key];
}

std::size_t Json::size() const {
    return object.size();
}

Json::iterator Json::begin() {
    return object.begin();
}

Json::const_iterator Json::begin() const {
    return object.begin();
}

Json::iterator Json::end() {
    return object.end();
}

Json::const_iterator Json::end() const {
    return object.end();
}

static std::string parse_string(std::istream &s, char &ch) {
    std::string key;
    while (s >> ch && ch != '\"') {
        key += std::string(1, ch);
    }
    return key;
}

static std::uint64_t parse_uint64(std::istream &s, char &ch) {
    uint64_t ans = ch - '0';
    while (s >> ch && isdigit(ch)) {
        ans = ans * 10 + (ch - '0');
    }
    if (ch != ' ' && ch != ',' && ch != '}') {
        throw std::runtime_error("JSON: excepted ,");
    }
    return ans;
}

static bool parse_boolean(std::istream &s, char &ch) {
    s >> ch;
    if (ch == 'r' && s >> ch && ch == 'u' && s >> ch && ch == 'e') {
        s >> ch;
        while (ch == ' ') {
            s >> ch;
        }
        if (ch != ',' && ch != '}') {
            throw std::runtime_error("JSON: excepted , or }");
        }
        return true;
    } else if (ch == 'a' && s >> ch && ch == 'l' && s >> ch && ch == 's' && s >> ch && ch == 'e') {
        s >> ch;
        while (ch == ' ') {
            s >> ch;
        }
        if (ch != ',' && ch != '}') {
            throw std::runtime_error("JSON: excepted , or }");
        }
        return false;
    }
    throw std::runtime_error("JSON: Incorrect value");
}

static std::vector<std::string> parse_array(std::istream &s, char &ch) {
    std::vector<std::string> ans;
    while (s >> ch && ch != ']') {
        if (ch == '\"') {
            ans.push_back(parse_string(s, ch));
        } else if (ch != ' ' && ch != ',') {
            throw std::runtime_error("JSON: Incorrect array");
        }
    }
    if (ch != ']') {
        throw std::runtime_error("JSON: excepted ]");
    }
    s >> ch;
    while (ch == ' ') {
        s >> ch;
    }
    if (ch != ',' && ch != '}') {
        throw std::runtime_error("JSON: excepted , or }");
    }
    return ans;
}

static std::nullptr_t parse_null(std::istream &s) {
    char ch;
    s >> ch;
    if (ch == 'u' && s >> ch && ch == 'l' && s >> ch && ch == 'l') {
        s >> ch;
        while (ch == ' ') {
            s >> ch;
        }
        if (ch != ',' && ch != '}') {
            throw std::runtime_error("JSON: excepted , or }");
        }
        return nullptr;
    }
    throw std::runtime_error("JSON: Incorrect value");
}

Json json::parse_json(std::istream &s, char last_char) {
    Json ans;
    std::string key;
    char ch;
    if (last_char != '{') {
        s >> ch;
        if (ch != '{') {
            throw std::runtime_error("JSON: excepted {");
        }
    }
    while (s >> ch) {
        if (ch == ' ') continue;
        if (ch == '\"' && key.empty()) {
            key = parse_string(s, ch);
        } else if (ch == ':') {

        } else if (ch == '}') {
            break;
        } else {
            if (ch == '\"') {
                ans[key] = Value::new_value(parse_string(s, ch));
            } else if (isdigit(ch)) {
                ans[key] = Value::new_value(parse_uint64(s, ch));
            } else if (ch == '{') {
                ans[key] = Value::new_value(parse_json(s, ch));
            } else if (ch == '[') {
                ans[key] = Value::new_value(parse_array(s, ch));
            } else if (ch == 't' || ch == 'f') {
                ans[key] = Value::new_value(parse_boolean(s, ch));
            } else if (ch == 'n') {
                ans[key] = Value::new_value(parse_null(s));
            }
            if (ch == '}') {
                break;
            }
            key.clear();
        }
    }
    return ans;
}

static void dump_string(std::ostream &out, const std::string &str) {
    out << '\"';
    out << str;
    out << '\"';
}

static void dump_uint64(std::ostream &out, std::uint64_t value) {
    out << value;
}

static void dump_boolean(std::ostream &out, bool value) {
    if (value) {
        out << "true";
    } else {
        out << "false";
    }
}

static void dump_null(std::ostream &out) {
    out << "null";
}

static void dump_array(std::ostream &out, const std::vector<std::string> &array) {
    out << '[';
    for (std::size_t i = 0; i < array.size(); i++) {
        if (i != 0) {
            out << ", ";
        }
        dump_string(out, array[i]);
    }
    out << ']';
}

void json::dump_json(std::ostream &out, const json::Json &object) {
    out << "{\n";
    std::size_t count = 0;
    for (const auto &item: object) {
        dump_string(out, item.first);
        out << ": ";
        if (item.second->is_string()) {
            dump_string(out, item.second->to_string());
        } else if (item.second->is_uint64()) {
            dump_uint64(out, item.second->to_uint64());
        } else if (item.second->is_boolean()) {
            dump_boolean(out, item.second->to_boolean());
        } else if (item.second->is_null()) {
            dump_null(out);
        } else if (item.second->is_array()) {
            dump_array(out, item.second->to_array());
        } else {
            dump_json(out, Json(item.second->to_object()));
        }
        count++;
        if (object.size() != count) {
            out << ",";
        }
        out << "\n";
    }
    out << "}\n";
}