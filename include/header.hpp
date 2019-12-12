//
// Created by mlkwov on 07.12.2019.
//
#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <string.h>
#include <any>
#include <fstream>


using std::any;
using std::vector;
using std::cout;
using std::string;
using std::map;
using std::ifstream;
using std::any_cast;


class Json {
public:
    string j_data;
    vector<string> key_code;
    vector<any> mean_code;
    vector<any> array_code;
    unsigned int cur_position = 0;

    Json() {}

    explicit Json(const string &s) {
        j_data = s;
    }

    bool is_object()
    {
        int i = 0;
        while (j_data[i] != '{' && j_data[i] != '[' && j_data[i] != '\"'&&
               (j_data[i] <= '0' || j_data[i] >= '9')) {
            i++;
        }
        if (j_data[i] == '{')
        {
            return true;
        } else {
            return false;
        }
    }

    bool is_array()
    {
        int i = 0;
        while ( j_data[i] != '{' && j_data[i] != '[' &&
                j_data[i] != '\"'&&(j_data[i] <= '0' || j_data[i] >= '9')){
            i++;
        }
        if (j_data[i] == '[')
        {
            return true;
        } else {
            return false;
        }
    }

    any &operator[](const string &key)
    {
        if (this->is_object()) {
            int i;
            for (i = 0; i <(int)key_code.size(); i++) {
                if (key_code[i] == key)
                {
                    break;
                }
            }
            return mean_code[i];
        }
        return mean_code[0];
    }

    any &operator[](const int &index)
    {
        if (this->is_array()) {
            return array_code[index];
        }
        return mean_code[index];
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


    int get_num_meaning()
    {
        string buffer;
        buffer.push_back(j_data[cur_position]);
        cur_position++;
        while (j_data[cur_position] >= '0' && j_data[cur_position] <= '9') {
            buffer.push_back(j_data[cur_position]);
            cur_position++;
        }
        return atoi(buffer.c_str());
    }

    any get_the_meaning()
    {
        any meaning;
        while (j_data[cur_position] != '{' && j_data[cur_position]
                                              != '[' && j_data[cur_position] != '\"' &&
               (j_data[cur_position] <= '0' || j_data[cur_position]
                                               >= '9') && j_data[cur_position] != 't' &&
               j_data[cur_position] != 'f') {
            cur_position++;
        }
        if (j_data[cur_position] >= '0' && j_data[cur_position] <= '9')
        {
            meaning = get_num_meaning();
            return meaning;
        }
        if (j_data[cur_position] == '\"')
        {
            cur_position++;
            meaning = get_string();
            find_end_meaning_object();
            return meaning;
        }
        if (j_data[cur_position] >= 'a' && j_data[cur_position] <= 'z')
        {
            string buffer;
            while (j_data[cur_position] >= 'a' && j_data[cur_position] <= 'z') {
                buffer.push_back(j_data[cur_position]);
                cur_position++;
            }
            if (buffer == "true") {
                meaning = true;
            }
            if (buffer == "false") {
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

    static Json parse_from_file(const string &path_file)
    {
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

    void object_parse()
    {
        any meaning;
        string key;
        while (j_data[cur_position] != '}') {
            key = get_key();
            key_code.push_back(key);
            meaning = get_the_meaning();
            mean_code.push_back(meaning);
            find_end_meaning_object();
        }
        if (j_data.find(",", cur_position) != string::npos)
        {
            cur_position++;
        } else {
            if (j_data.find("]", cur_position) != string::npos)
            {
                cur_position++;
            } else {
                if (j_data.find("}", cur_position + 1) != string::npos)
                {
                    cur_position++;
                }
            }
        }
    }

    void array_parse()
    {
        while (j_data[cur_position] != ']') {
            cur_position++;
            if (j_data[cur_position] != ']')
            {
                array_code.push_back(get_the_meaning());
            }
            find_end_meaning_object();
        }
        if (j_data.find(",", cur_position) != string::npos)
        {
            cur_position++;
        } else {
            if (j_data.find("}", cur_position) != string::npos)
            {
                cur_position++;
            } else {
                if (j_data.find("]", cur_position + 1) != string::npos)
                {
                    cur_position++;
                }
            }
        }
    }

    Json parse()
    {
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

    static Json parse(const string &s)
    {
        Json curr;
        curr.j_data = s;
        curr.parse();
        return curr;
    }
};
#endif // INCLUDE_HEADER_HPP_