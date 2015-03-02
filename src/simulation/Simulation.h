
#pragma once

// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the ISC License. See the COPYING file at the top-level directory of
// this distribution or at http://opensource.org/licenses/ISC

#include "main/Config.h"
#include "main/Application.h"
#include "overlay/LoopbackPeer.h"
#include "generated/StellarXDR.h"
#include "util/Timer.h"
#include "crypto/SHA.h"
#include "medida/medida.h"

#define SIMULATION_CREATE_NODE(N) \
    const Hash v##N##VSeed = sha256("SEED_VALIDATION_SEED_" #N);    \
    const SecretKey v##N##SecretKey = SecretKey::fromSeed(v##N##VSeed); \
    const Hash v##N##NodeID = v##N##SecretKey.getPublicKey();

namespace stellar
{
using namespace std;
    
class Simulation
{
  private:
    VirtualClock mClock;
    bool mMode;
    int mConfigCount;
    Application::pointer mIdleApp;
    map<uint256, Config::pointer> mConfigs;
    map<uint256, Application::pointer> mNodes;
    vector<shared_ptr<LoopbackPeerConnection>> mConnections;

    uint64 getMinBalance();

  public:
      enum Mode
      {
          OVER_TCP,
          OVER_LOOPBACK
      };

    typedef shared_ptr<Simulation> pointer;

    Simulation(Mode mode);
    ~Simulation();

    VirtualClock& getClock();

    uint256 addNode(uint256 validationSeed, 
                    FBAQuorumSet qSet,
                    VirtualClock& clock);
    Application::pointer getNode(uint256 nodeID);
    vector<Application::pointer> getNodes();

    void
        addConnection(uint256 initiator,
        uint256 acceptor);
        
    shared_ptr<LoopbackPeerConnection>
        addLoopbackConnection(uint256 initiator, 
                      uint256 acceptor);

    void addTCPConnection(uint256 initiator,
                          uint256 acception);
        
    void startAllNodes();

    bool haveAllExternalized(int num);

    size_t crankNode(uint256 nodeID, int nbTicks=1);
    size_t crankAllNodes(int nbTicks=1);
    void crankForAtMost(VirtualClock::duration seconds);
    void crankForAtLeast(VirtualClock::duration seconds);

    //////////

    struct TxInfo;

    class AccountInfo : public enable_shared_from_this<AccountInfo> {
    public:
        AccountInfo(size_t id, SecretKey key, uint64_t balance, Simulation & simulation) : mId(id), mKey(key), mBalance(balance), mSeq(0), mSimulation(simulation) {}
        size_t mId;
        SecretKey mKey;
        uint64_t mBalance;
        uint32_t mSeq;

        TxInfo creationTransaction();
    private:
        Simulation& mSimulation;
    };
    using accountInfoPtr = shared_ptr<AccountInfo>;
    vector<accountInfoPtr> mAccounts;

    struct TxInfo {
        accountInfoPtr mFrom;
        accountInfoPtr mTo;
        uint64_t mAmount;
        void execute(shared_ptr<Application> app);
    };


    vector<Simulation::TxInfo> createAccounts(int n);
    void execute(TxInfo transaction);
    void executeAll(vector<TxInfo>& transaction);
    vector<accountInfoPtr> checkAgainstDbs(); // returns the accounts that don't match

    void printMetrics(string domain);
};
}


