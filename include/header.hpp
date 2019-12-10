// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <string.h>
#include <any>
#include <fstream>

//using namespace std;
using std::any;
using std::vector;
using std::cout;
using std::string;
using std::map;
using std::ifstream;
using std::any_cast;
//#include "json.hpp"
//using json=nlohmann::json;

class Json {
public:
    string j_data;
    vector<string> _keys;
    vector<any> _meanings;
    vector<any> _array_meanings;
    unsigned int cur_position = 0;
    unsigned int _smeschenie = 0;

    Json() {}

    Json(const string &s) {
        j_data = s;
    }

    map<string, any> _field;

    bool is_object() {
        int i = 0;
        while (j_data[i] != '{' && j_data[i] != '[' && j_data[i] != '\"'&&
               (j_data[i] <= '0' || j_data[i] >= '9')) {
            i++;
        }
        if (j_data[i] == '{') {
            return true;
        } else { return false; }
    }

    bool is_array() {
        int i = 0;
        while ( j_data[i] != '{' && j_data[i] != '[' && j_data[i]
                                                        != '\"'&&(j_data[i] <= '0' || j_data[i] >= '9')){
            i++;
        }
        if (j_data[i] == '[') {
            return true;
        } else { return false; }
    }

    any &operator[](const string &key) {
        if (this->is_object()) {
            int i;
            for (i = 0; i <(int)_keys.size(); i++) {
                if (_keys[i] == key) { break; }
            }
            return _meanings[i];
        }
        return _meanings[0];
    }

    any &operator[](const int &index) {
        if (this->is_array()) {
            return _array_meanings[index];
        }
        return _meanings[index];
    }

    string get_string()
    //считывает с начала строки,до " а потом перескакивает его
    {
        string buf;
        while (j_data[cur_position] != '\"') {
            buf.push_back(j_data[cur_position]);
            cur_position++;
        }
        cur_position++;
        return buf;
    }

    int get_number()
    //считывает число и доводит курсор до знака разделения или конца (, } ])
    {
        string meaning;
        while (j_data[cur_position] <= '0' || j_data[cur_position] >= '9') {
            cur_position++;
        }
        while (j_data[cur_position] >= '0' && j_data[cur_position] <= '9') {
            meaning.push_back(j_data[cur_position]);
        }
        while (j_data[cur_position] != ',' && j_data[cur_position] != '}'
               && j_data[cur_position] != ']') {
            cur_position++;
        }
        return atoi(meaning.c_str());
    }
    string get_key()//находит ключ и смещает значение курсора до : +1
    {
        while (j_data[cur_position] != '\"'){
            cur_position++;
        }
        cur_position++;
        string key = get_string();
        cout << key << " ";
        while (j_data[cur_position] != ':') {
            cur_position++;
        }
        cur_position++;
        return key;
    }

    void find_end_meaning_object()//перемещает курсор до , ] или }
    {
        while (j_data[cur_position] != ',' && j_data[cur_position] != '}'
               && j_data[cur_position] != ']') {
            cur_position++;
        }
    }


    int get_num_meaning() {
        string buf;
        buf.push_back(j_data[cur_position]);
        cur_position++;
        while (j_data[cur_position] >= '0' && j_data[cur_position] <= '9') {
            buf.push_back(j_data[cur_position]);
            cur_position++;
        }
        return atoi(buf.c_str());
    }

    any get_the_meaning() {
        any meaning;
        while (j_data[cur_position] != '{' && j_data[cur_position]
                                              != '[' && j_data[cur_position] != '\"' &&
               (j_data[cur_position] <= '0' || j_data[cur_position]
                                               >= '9') && j_data[cur_position] != 't' &&
               j_data[cur_position] != 'f') {
            cur_position++;
        }
        if (j_data[cur_position] >= '0' && j_data[cur_position] <= '9') {
            meaning = get_num_meaning();
            return meaning;
        }
        if (j_data[cur_position] == '\"') {
            cur_position++;
            meaning = get_string();
            find_end_meaning_object();
            return meaning;
        }
        if (j_data[cur_position] >= 'a' && j_data[cur_position] <= 'z') {
            string buf;
            while (j_data[cur_position] >= 'a' && j_data[cur_position] <= 'z') {
                buf.push_back(j_data[cur_position]);
                cur_position++;
            }
            if (buf == "true") {
                meaning = true;
            }
            if (buf == "false") {
                meaning = false;
            }
            find_end_meaning_object();
            return meaning;
        }
        if (j_data[cur_position] == '{') {
            Json tmp(j_data.substr(cur_position, j_data.size() - cur_position));
            meaning = tmp.parse();
            cur_position += tmp.cur_position;
            if ((j_data.find(",", cur_position) <
                 j_data.find("}", cur_position + 1)) &&
                (j_data.find("}", cur_position + 1) != string::npos)) {
                while (j_data[cur_position] != ',') {
                    cur_position++;
                }
            }
            return meaning;
        }

        if (j_data[cur_position] == '[') {
            Json tmp(j_data.substr(cur_position, j_data.size() - cur_position));
            meaning = tmp.parse();
            cur_position += tmp.cur_position;
            if ((j_data.find(",", cur_position) <
                 j_data.find("]", cur_position + 1)) &&
                (j_data.find(",", cur_position) != string::npos)) {
                while (j_data[cur_position] != ',') {
                    cur_position++;
                }
            }
            return meaning;
        }
        return meaning;
    }

    static Json parse_from_file(const string &path_file) {
        ifstream fin(path_file);
        char c;
        string data;
        c = fin.get();
        while (!fin.eof()) {
            data.push_back(c);
            c = fin.get();
        }
        cout << data;
        fin.close();
        Json tmp;
        tmp.j_data = data;
        tmp.parse();
        return tmp;
    }

    void object_parse() {
        any meaning;
        string key;
        while (j_data[cur_position] != '}') {
            key = get_key();
            //cout<<key<<": ";
            _keys.push_back(key);
            meaning = get_the_meaning();
            _meanings.push_back(meaning);
            find_end_meaning_object();
        }
        if (j_data.find(",", cur_position) != string::npos) {
            cur_position++;
        } else {
            if (j_data.find("]", cur_position) != string::npos) {
                cur_position++;
            } else {
                if (j_data.find("}", cur_position + 1) != string::npos) {
                    cur_position++;
                }
            }
        }
    }

    void array_parse() {
        while (j_data[cur_position] != ']') {
            cur_position++;
            if (j_data[cur_position] != ']')
            { _array_meanings.push_back(get_the_meaning()); }
            find_end_meaning_object();
        }
        if (j_data.find(",", cur_position) != string::npos) {
            cur_position++;
        } else {
            if (j_data.find("}", cur_position) != string::npos) {
                cur_position++;
            } else {
                if (j_data.find("]", cur_position + 1) != string::npos) {
                    cur_position++;
                }
            }
        }
    }

    Json parse() {
        if (this->is_array()) {
            cout << "it's array ";
            array_parse();
        }
        if (this->is_object()) {
            cout << "it's object ";
            object_parse();
        }
        return *(this);
    }

    static Json parse(const string &s) {
        Json tmp;
        tmp.j_data = s;
        tmp.parse();
        return tmp;
    }


};


int main() {
    Json test;
    // test = Json::parse_from_file("/home/sergei/Desktop/test.json");
    //cout << any_cast<string>(test["firstname"]);
    Json hello("{\n"
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

    hello.parse();
    auto hi = any_cast<Json>(hello["marks"]);
    if (!any_cast<bool>(hello._meanings[3]))
    {
        //cout<<any_cast<string>(hi._meanings[1])<<endl;
        cout << any_cast<int>(hi[4]) << std::endl;
        cout << any_cast<int>(hello["age"]) << std::endl;
    }

    return 0;
}

#endif // INCLUDE_HEADER_HPP_
