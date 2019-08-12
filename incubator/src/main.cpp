//
// Created by Ivan Kishchenko on 2019-07-11.
//


#include <nlohmann/json.hpp>

int main(int argc, char *argv[]) {
    nlohmann::json test = {
            {
                    "pi",      3.141
            },
            {
                    "happy",   true
            },
            {
                    "name",    "Niels"
            },
            {
                    "nothing", nullptr
            },
            {
                    "answer",  {
                                       {
                                               "everything", 42
                                       }
                               }
            },
            {
                    "list",    {
                                       1,
                                       0,
                                       2
                               }
            },
            {
                    "object",  {
                                       {
                                               "currency",   "USD"
                                       },
                                       {
                                               "value", 42.99
                                       }
                               }
            }
    };

    std::string s = test.dump();


    auto t = test;

    s = t.dump();
    
    return 0;
}
