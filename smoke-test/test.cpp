/*
 * Copyright (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Ugo Pattacini <ugo.pattacini@iit.it>
 * CopyPolicy: Released under the terms of the GNU GPL v3.0.
*/

#include <string>

#include <rtf/yarp/YarpTestCase.h>
#include <rtf/dll/Plugin.h>

#include <yarp/os/all.h>
#include <yarp/math/Rand.h>

using namespace std;
using namespace RTF;
using namespace yarp::os;
using namespace yarp::math;

/**********************************************************************/
class TestAssignmentGit : public YarpTestCase
{
    RpcClient portGit;

public:
    /******************************************************************/
    TestAssignmentGit() :
        YarpTestCase("TestAssignmentGit")
    {
    }

    /******************************************************************/
    virtual ~TestAssignmentGit()
    {
    }

    /******************************************************************/
    virtual bool setup(yarp::os::Property& property)
    {
        float rpcTmo=(float)property.check("rpc-timeout",Value(10.0)).asDouble();

        string portGitName("/"+getName());
        portGit.open(portGitName);

        RTF_TEST_REPORT(Asserter::format("Set rpc timeout = %g [s]",rpcTmo));
        portGit.asPort().setTimeout(rpcTmo);

        RTF_TEST_REPORT("Connecting Ports");
        RTF_ASSERT_ERROR_IF(Network::connect(portGitName,"/service"),
                            "Unable to connect to /service");

        Rand::init();

        return true;
    }

    /******************************************************************/
    virtual void tearDown()
    {
        RTF_TEST_REPORT("Closing Ports");
        portGit.close();
    }

    /******************************************************************/
    virtual void run()
    {
        int num=(int)Rand::scalar(1.0,100.0);

        Bottle cmd,reply;
        cmd.addInt(num);
        RTF_ASSERT_ERROR_IF(portGit.write(cmd,reply),"Unable to talk to the module");

        string response=reply.get(0).asString();
        RTF_TEST_CHECK(response==(num&0x01?"odd":"even"),
                       Asserter::format("Request=%d; Response=%s",
                                        num,response.c_str()));
    }
};

PREPARE_PLUGIN(TestAssignmentGit)
