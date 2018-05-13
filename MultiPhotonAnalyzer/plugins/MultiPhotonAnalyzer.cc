
#include "multiphoton-analysis/CommonClasses/interface/ngNtuplizer.h"

using namespace std;

//// system include files
//#include <memory>
//#include <vector>
//
//// user include files
//#include "FWCore/Framework/interface/Frameworkfwd.h"
//#include "FWCore/Framework/interface/one/EDAnalyzer.h"
//
//#include "FWCore/Framework/interface/Event.h"
//#include "FWCore/Framework/interface/MakerMacros.h"
//
//#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
//// GenParticles 
//#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
//#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
//#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
//
//// T-headers, TFileService 
//#include "FWCore/ServiceRegistry/interface/Service.h"
//#include "CommonTools/UtilAlgos/interface/TFileService.h"
//#include "TLorentzVector.h"
//#include "TH2D.h"
//#include "TTree.h"
//
//// Common Classes
//#include "multiphoton-analysis/CommonClasses/interface/EventInfo.h"
////#include "multiphoton-analysis/CommonClasses/interface/BeamSpotInfo.h"
//#include "multiphoton-analysis/CommonClasses/interface/VertexInfo.h"
//#include "multiphoton-analysis/CommonClasses/interface/TriggerInfo.h"
//#include "multiphoton-analysis/CommonClasses/interface/JetInfo.h"
//#include "multiphoton-analysis/CommonClasses/interface/PhotonID.h"
//#include "multiphoton-analysis/CommonClasses/interface/PhotonInfo.h"
//#include "multiphoton-analysis/CommonClasses/interface/GenParticleInfo.h"
//#include "multiphoton-analysis/CommonClasses/interface/DiPhotonInfo.h"
//#include "multiphoton-analysis/CommonClasses/interface/PileupInfo.h"
//
//// for EGM IDs
//#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"
//
//// for photons
//#include "DataFormats/PatCandidates/interface/Photon.h"
//
//// for jets
//#include "DataFormats/PatCandidates/interface/Jet.h"
//
//// for genParticles
//#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
//
//// for genEventInfo
//#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
//
//// for deltaR
//#include "DataFormats/Math/interface/deltaR.h"
//
//// for trigger and filter decisions
//#include "DataFormats/Common/interface/TriggerResults.h"
//#include "FWCore/Common/interface/TriggerNames.h"
//
//

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the nclude "HiggsAnalysis/HiggsTo2photons/interface/CiCPhotonID.h"class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.
//
//class MultiPhotonAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
//   public:
//      explicit MultiPhotonAnalyzer(const edm::ParameterSet&);
//      ~MultiPhotonAnalyzer();
//
//      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
//
//
//   private:
//      virtual void beginJob() override;
//      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
//      virtual void endJob() override;
//
//      // ----------member data ---------------------------
//      edm::Service<TFileService> fs;
//      
//      // -----Tokens & InputTags
//      //edm::EDGetTokenT<vector<reco::GenParticle> > genParticlesToken_;
//      //edm::InputTag genParticles_;
//      //edm::InputTag particles_;
//
//      // ----Trees  
//      TTree *fgenTree;
//      
//      // ----Structs (Instantiate here)  
//      // event 
//      ExoDiPhotons::eventInfo_t fEventInfo;
//      //ExoDiPhotons::genParticleInfo_t fGenPhoton1Info;
//      //ExoDiPhotons::genParticleInfo_t fGenPhoton2Info;
//
//      // ----Flags (switch in configuration file) 
//      // MC flag 
//      bool isGEN_; 
//};
//
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MultiPhotonAnalyzer::MultiPhotonAnalyzer(const edm::ParameterSet& iConfig)
: //genParticlesToken_ = consumes<vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genparticles"));  
  isGEN_(iConfig.getParameter<bool>("isGEN"))
{
   //now do what ever initialization is needed
   usesResource("TFileService");
  
   // Make Tree/s 
   fgenTree = fs->make<TTree>("fgenTree","GENDiphotonTree");
  
   //-----Branches
   //GenParticles
   fgenTree->Branch("Event",&fEventInfo,ExoDiPhotons::eventBranchDefString.c_str());
   //fTree->Branch("GenPhoton1",&fGenPhoton1Info,ExoDiPhotons::genParticleBranchDefString.c_str());
   //fTree->Branch("GenPhoton2",&fGenPhoton2Info,ExoDiPhotons::genParticleBranchDefString.c_str());
   
   //-----Tokens
}


MultiPhotonAnalyzer::~MultiPhotonAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MultiPhotonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   //------ Event Information 
   ExoDiPhotons::InitEventInfo(fEventInfo);
   ExoDiPhotons::FillBasicEventInfo(fEventInfo, iEvent);
   //Debug
   cout <<  "Run: " << iEvent.id().run() 
        << ", LS: " <<  iEvent.id().luminosityBlock() 
        << ", Event: " << iEvent.id().event() 
        << endl;

   //------- GEN Information

    
   //----Fill Tree
   if(isGEN_) fgenTree->Fill();


  
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
MultiPhotonAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MultiPhotonAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MultiPhotonAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MultiPhotonAnalyzer);
