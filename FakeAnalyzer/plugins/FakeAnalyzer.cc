//
// Original Author:  Cilicia Uzziel Perez
//         Created:  Thu, 30 Jul 2020 13:51:06 GMT
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Common Classes
#include "multiphoton-analysis/CommonClasses/interface/EventInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/TriggerInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/JetInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/PhotonID.h"
#include "multiphoton-analysis/CommonClasses/interface/PhotonInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/VertexInfo.h"

// for TFileService, trees
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TTree.h"

// ECAL topology
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

// for EGM ID
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"

// for photons
#include "DataFormats/PatCandidates/interface/Photon.h"

// for jets
#include "DataFormats/PatCandidates/interface/Jet.h"

// for trigger and filter decisions
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

using namespace std;
using namespace edm;

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class FakeAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit FakeAnalyzer(const edm::ParameterSet&);
      ~FakeAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
    //edm::EDGetToken                          photonsMiniAODToken_;
    edm::EDGetToken                                 photonsMiniAODToken_;
    //edm::EDGetTokenT<edm::View<pat::Photon> >        photonsMiniAODToken_;
    ExoDiPhotons::eventInfo_t fEventInfo;
};

FakeAnalyzer::FakeAnalyzer(const edm::ParameterSet& ps)

{
   //now do what ever initialization is needed
   usesResource("TFileService");
   //photonsMiniAODToken_  = mayConsume<edm::View<pat::Photon> >(ps.getParameter<edm::InputTag>("photonsMiniAOD"));
   photonsMiniAODToken_      = consumes<edm::View<pat::Photon> >      (ps.getParameter<edm::InputTag>("photonsMiniAOD"));
   //photonsMiniAODToken_     = consumes<View<pat::Photon> >            (ps.getParameter<InputTag>("photonsMiniAOD"));
}


FakeAnalyzer::~FakeAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
FakeAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   // edm::Handle<edm::View<pat::Photon> > photons;
   // iEvent.getByToken(photonsMiniAODToken_,photons);
   edm::Handle<edm::View<pat::Photon> >       photons;
   iEvent.getByToken(photonsMiniAODToken_,  photons);

   ExoDiPhotons::InitEventInfo(fEventInfo);
   ExoDiPhotons::FillBasicEventInfo(fEventInfo, iEvent);

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void
FakeAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
FakeAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
FakeAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(FakeAnalyzer);
