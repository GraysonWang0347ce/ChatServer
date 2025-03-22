#include <json.hpp>
#include <vector>
#include <map>
#include<iostream>
#include<string>

using json = nlohmann::json;
using namespace std;



int main(int argc, char *argv[])
{
    json js;
    string str;
    vector<int> vec({1,2,3});

    js["vec"] = vec;

    str = js.dump();

    json buff = json::parse(str);

    cout<<buff["ve"]<<endl;

    cout<<js<<endl;


    return 0;
}