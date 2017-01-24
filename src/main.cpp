// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
//
// Author: Ugo Pattacini - <ugo.pattacini@iit.it>

#include <string>

#include <yarp/os/Network.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>

using namespace std;
using namespace yarp::os;

/***************************************************/
int main()
{
    // init link to Yarp network
    Network yarp;
    if (!yarp.checkNetwork())
    {
        yError()<<"YARP doesn't seem to be available";
        return 1;
    }

    // open a port to receive requests
    Port port;
    port.open("/service");

    // wait for the first incoming request
    Bottle request;
    port.read(request,true);

    // retrieve the request payload
    int num=request.get(0).asInt();

    // process the payload
    bool isNumEven=true;    // FILL IN THE CODE

    // build the response
    string isNumEvenStr=(isNumEven?"even":"odd");
    Bottle response;
    response.addString("dummy"); // FILL IN THE CODE

    // send back the response
    port.reply(response);

    // shut down
    port.close();

    return 0;
}
