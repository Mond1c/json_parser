#pragma once

#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <sstream>

namespace json {

    enum class ValueType : std::uint8_t {
        Uint64,
        String,
        Object,
        Array,
        Boolean,
        Null
    };

    class Value;

    class Json {
    private:
        using json_object = std::unordered_map<std::string, std::shared_ptr<Value>>;
    public:
        using iterator = json_object::iterator;
        using const_iterator = json_object::const_iterator;
    public:
        Json() = default;
        explicit Json(json_object  obj) : object(std::move(obj)) {}

        std::size_t size() const;

        bool contains_key(const std::string &key) const;

        const Value &operator[](const std::string &key) const;

        Value &operator[](const std::string &key);

        iterator begin();
        const_iterator begin() const;

        iterator end();
        const_iterator end() const;
    private:

        json_object object;

        friend class Value;
    };

    class Value {
    public:
       // Value() = default;
        Value(const Value&) = default;
        Value& operator=(const Value& v) = default;

        static Value new_value(uint64_t value) {
            Value instance;
            instance.uint64_value = value;
            instance.value_type = ValueType::Uint64;
            return instance;
        }

        static Value new_value(const std::string &value) {
            Value instance;
            instance.string_value = value;
            instance.value_type = ValueType::String;
            return instance;
        }

        static Value new_value(const Json &object) {
            return new_value(object.object);
        }

        static Value new_value(const std::unordered_map<std::string, std::shared_ptr<Value>> &object) {
            Value instance;
            instance.object_value = object;
            instance.value_type = ValueType::Object;
            return instance;
        }

        static Value new_value(const std::vector<std::string> &array) {
            Value instance;
            instance.array_value = array;
            instance.value_type = ValueType::Array;
            return instance;
        }

        static Value new_value(bool value) {
            Value instance;
            instance.boolean_value = value;
            instance.value_type = ValueType::Boolean;
            return instance;
        }

        static Value new_value(std::nullptr_t) {
            Value instance;
            instance.value_type = ValueType::Null;
            return instance;
        }

        bool is_uint64() const;

        bool is_string() const;

        bool is_object() const;

        bool is_array() const;

        bool is_boolean() const;

        bool is_null() const;

        std::uint64_t to_uint64() const;

        const std::string &to_string() const;

        const std::unordered_map<std::string, std::shared_ptr<Value>> &to_object() const;

        const std::vector<std::string> &to_array() const;

        bool to_boolean() const;

        std::nullptr_t to_null() const;

        std::uint64_t &to_uint64();

        std::string &to_string();

        std::unordered_map<std::string, std::shared_ptr<Value>> &to_object();

        std::vector<std::string> &to_array();

        bool &to_boolean();

        const std::shared_ptr<Value> &at(const std::string &key) const;

        std::shared_ptr<Value> &at(const std::string &key);

        const Value &operator[](const std::string &key) const;

        Value &operator[](const std::string &key);

    private:

        Value() = default;

        ValueType value_type = ValueType::Null;

        std::uint64_t uint64_value{};
        std::string string_value;
        std::unordered_map<std::string, std::shared_ptr<Value>> object_value;
        std::vector<std::string> array_value;
        bool boolean_value{};
    };


    Json parse_json(std::istream &s, char last_char = ' ');
    void dump_json(std::ostream& out, const Json& object);
} // namespace json
