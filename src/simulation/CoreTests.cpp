// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the ISC License. See the COPYING file at the top-level directory of
// this distribution or at http://opensource.org/licenses/ISC

#include "simulation/Topologies.h"
#include "lib/catch.hpp"
#include "generated/StellarXDR.h"
#include "main/Application.h"
#include "overlay/LoopbackPeer.h"
#include "util/make_unique.h"
#include "crypto/SHA.h"
#include "main/test.h"
#include "util/Logging.h"
#include "util/types.h"

using namespace stellar;

typedef std::unique_ptr<Application> appPtr;

TEST_CASE("core4 topology", "[simulation]")
{
    Simulation::pointer simulation = Topologies::core4();
    simulation->startAllNodes();

    simulation->crankForAtMost(std::chrono::seconds(2));

    REQUIRE(simulation->haveAllExternalized(3));
}

TEST_CASE("cycle4 topology", "[simulation]")
{
    Simulation::pointer simulation = Topologies::cycle4();
    simulation->startAllNodes();

    simulation->crankForAtMost(std::chrono::seconds(20));

    // Still transiently does not work (quorum retrieval)
    CHECK(simulation->haveAllExternalized(2));
}

TEST_CASE("pair of node creating 50 accounts", "[simulation]")
{
    Simulation::pointer simulation = Topologies::pair(Simulation::OVER_LOOPBACK);

    simulation->startAllNodes();
    simulation->crankForAtLeast(std::chrono::seconds(10));

    REQUIRE(simulation->haveAllExternalized(3));

    simulation->executeAll(simulation->createAccounts(50));
    simulation->crankForAtLeast(std::chrono::seconds(60));
 
    REQUIRE(simulation->haveAllExternalized(4));
    auto problemAccounts = simulation->checkAgainstDbs();
    REQUIRE(problemAccounts.empty());

    simulation->printMetrics("database");
}

