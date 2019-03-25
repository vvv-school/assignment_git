/*
 * Copyright (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Ugo Pattacini <ugo.pattacini@iit.it>
 * CopyPolicy: Released under the terms of the GNU GPL v3.0.
*/

#include <string>
#include <cmath>

#include <robottestingframework/dll/Plugin.h>
#include <robottestingframework/TestAssert.h>

#include <yarp/robottestingframework/TestCase.h>
#include <yarp/os/all.h>
#include <yarp/math/Rand.h>

using namespace std;
using namespace robottestingframework;
using namespace yarp::os;
using namespace yarp::math;

/**********************************************************************/
class TestAssignmentGit : public yarp::robottestingframework::TestCase
{
    RpcClient portGit;

    /******************************************************************/
    string parityTest(const int i) const
    {
        return (i&0x01?"odd":"even");
    }

    /******************************************************************/
    string primalityTest(const int i) const
    {
        if (i<2)
            return "composite";
        for (int j=2; j<=sqrt(i); j++)
            if (i%j==0)
                return "composite";
        return "prime";
    }

public:
    /******************************************************************/
    TestAssignmentGit() :
        yarp::robottestingframework::TestCase("TestAssignmentGit")
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

        ROBOTTESTINGFRAMEWORK_TEST_REPORT(Asserter::format("Set rpc timeout = %g [s]",rpcTmo));
        portGit.asPort().setTimeout(rpcTmo);

        ROBOTTESTINGFRAMEWORK_TEST_REPORT("Connecting Ports");
        ROBOTTESTINGFRAMEWORK_ASSERT_ERROR_IF_FALSE(Network::connect(portGitName,"/service"),
                                  "Unable to connect to /service");

        Rand::init();

        return true;
    }

    /******************************************************************/
    virtual void tearDown()
    {
        ROBOTTESTINGFRAMEWORK_TEST_REPORT("Closing Ports");
        portGit.close();
    }

    /******************************************************************/
    virtual void run()
    {
        int num=(int)Rand::scalar(1.0,100.0);
        string parity=parityTest(num);
        string primality=primalityTest(num);

        Bottle cmd,reply;
        cmd.addInt(num);
        ROBOTTESTINGFRAMEWORK_ASSERT_ERROR_IF_FALSE(portGit.write(cmd,reply),"Unable to talk to the module");

        string parityRx=reply.get(0).asString();
        string primalityRx=reply.get(1).asString();

        ROBOTTESTINGFRAMEWORK_TEST_REPORT(Asserter::format("Request=%d; Response=%s; Result=%s",
                                         num,parityRx.c_str(),parity.c_str()));
        ROBOTTESTINGFRAMEWORK_TEST_REPORT(Asserter::format("Request=%d; Response=%s; Result=%s",
                                         num,primalityRx.c_str(),primality.c_str()));

        int score=(parityRx==parity?1:0);
        score+=(primalityRx==primality?2:0);
        ROBOTTESTINGFRAMEWORK_TEST_CHECK(score>0,Asserter::format("Total score = %d",score));
    }
};

ROBOTTESTINGFRAMEWORK_PREPARE_PLUGIN(TestAssignmentGit)
