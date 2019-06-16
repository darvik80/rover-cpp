//
// Created by Ivan Kishchenko on 2019-05-22.
//

#ifndef ROVER_SERVER_H
#define ROVER_SERVER_H


#include <Poco/Util/ServerApplication.h>

class Server: public Poco::Util::ServerApplication
{
private:
    int main(const std::vector<std::string>& args) override;

protected:
    void initialize(Application &self) override;
};

#endif //ROVER_SERVER_H
