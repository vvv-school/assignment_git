// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
//
// Author: Ugo Pattacini - <ugo.pattacini@iit.it>

#include <cstdlib>
#include <cmath>
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
        return EXIT_FAILURE;
    }

    // open a port to receive requests
    Port port;
    port.open("/service");

    // wait for the incoming request
    Bottle request;
    port.read(request,true);

    // retrieve the request payload
    int num=request.get(0).asInt();

    // process the payload
    bool isNumEven=true;    // FILL IN THE CODE
    bool isNumPrime=true;   // FILL IN THE CODE

    // build the response
    string parity=(isNumEven?"even":"odd");
    string primality=(isNumPrime?"prime":"composite");
    Bottle response;
    response.addString("parity-dummy");     // FILL IN THE CODE
    response.addString("primality-dummy");  // FILL IN THE CODE

    // send back the response
    port.reply(response);

    // shut down
    port.close();

    return EXIT_SUCCESS;
}
