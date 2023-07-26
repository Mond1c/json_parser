#include "json.h"
#include <iostream>
#include <fstream>
std::ostream& operator<<(std::ostream& out, const std::vector<std::string>& v) {
    for (const auto& s : v) {
        out << '\"' << s << '\"' << " ";
    }
    return out;
}

void simple_test() {
  //  std::ifstream file("input.json");
    std::string s = "{\"employee\":{\"name\":\"John\", \"a    ge\":30, \"city\":\"New York\"}}";
    std::istringstream ss(s);
    std::istream& i = ss;
    auto obj = json::parse_json(i);

    std::cout << obj["employee"].is_object() << std::endl;
    std::cout << obj["employee"].to_object()["name"]->to_string() <<     std::endl;
    std::cout << obj["employee"].to_object()["age"]->to_uint64() <<     std::endl;
    std::cout << obj["employee"].to_object()["city"]->to_string() <<     std::endl;
    std::cout << obj.size() << std::endl;
    std::cout << obj["employee"].to_object().size() << std::endl;
}

void file_test() {
    std::ifstream file("test.json");
    auto obj = json::parse_json(file);
    for (const auto& i : obj) {
        std::cout << i.first << std::endl;
    }
    std::cout << obj["task_count"].is_uint64() << std::endl << obj["task_count"].to_uint64() << std::endl;
    std::cout << obj["task2"].at("name")->to_string() << std::endl;
    std::cout << obj.size() << std::endl;

    std::ofstream output("output.json");
    json::dump_json(output, obj);
}


int main() {
    file_test();
}
