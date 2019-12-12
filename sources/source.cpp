#include <iostream>
#include "json.hpp"


int main() {

    Json data("{\n"
               "    \"lastname\" : \"Ivanov\",\n"
               "    \"firstname\" : \"Ivan\",\n"
               "    \"age\" : 25,\n"
               "    \"islegal\" : false,\n"
               "    \"marks\" : [\n"
               "    \t4,5,5,5,2,3\n"
               "    ],\n"
               "    \"address\" : {\n"
               "    \t\"city\" : \"Moscow\",\n"
               "        \"street\" : \"Vozdvijenka\"\n"
               "    }\n"
               "}");

    data.parse();
    auto test = any_cast<Json>(data["marks"]);
    if (!any_cast<bool>(data.mean_code[3]))
    {
        cout << any_cast<int>(test[4]) << std::endl;
        cout << any_cast<int>(data["age"]) << std::endl;
    }

    return 0;
}
