#include "multiphoton-analysis/CommonClasses/interface/nPhotonAnalyzer.h"

using namespace std;
using namespace edm;

nPhotonAnalyzer::nPhotonAnalyzer(const edm::ParameterSet& ps)

{
   usesResource("TFileService");

   isGood_                    = false;
   //isMC_                      = true;
   //isPythia8gen_              = true;
   //isSherpaDiphoton_          = false;
   nPV_                       = 0;
   SherpaGenPhoton0_iso_      = 9999.99;
   SherpaGenPhoton1_iso_      = 9999.99;

   genParticlesToken_        = consumes<vector<reco::GenParticle>>      (ps.getParameter<InputTag>("genparticles"));
   rhoToken_                 = consumes<double>                         (ps.getParameter<edm::InputTag>("rho"));
   phoLooseIdMapToken_       = consumes<edm::ValueMap<bool> >           (ps.getParameter<edm::InputTag>("phoLooseIdMap"));
   phoMediumIdMapToken_      = consumes<edm::ValueMap<bool> >           (ps.getParameter<edm::InputTag>("phoMediumIdMap"));
   phoTightIdMapToken_       = consumes<edm::ValueMap<bool> >           (ps.getParameter<edm::InputTag>("phoTightIdMap"));
   nEventsSample_            =                                          (ps.getParameter<uint32_t>("nEventsSample"));
   genInfoToken_             = consumes<GenEventInfoProduct>            (ps.getParameter<edm::InputTag>("genInfo"));
   genParticlesMiniAODToken_ = mayConsume<edm::View<reco::GenParticle> >(ps.getParameter<edm::InputTag>("genParticlesMiniAOD"));
   isMC_                     =                                           ps.getParameter<bool>("isMC");
   isPythia8gen_             =                                           ps.getParameter<bool>("isPythia8gen");
   isSherpaDiphoton_         =                                           ps.getParameter<bool>("isSherpaDiphoton");

   if (isPythia8gen_){
   fgenTree = fs->make<TTree>("fgenTree","GENDiphotonTree");
   fgenTree->Branch("Event",       &fEventInfo,       ExoDiPhotons::eventBranchDefString.c_str());
   fgenTree->Branch("GenPhoton1",  &fGenPhoton1Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenPhoton2",  &fGenPhoton2Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenDiPhoton", &fGenDiPhotonInfo, ExoDiPhotons::diphotonBranchDefString.c_str());
   fgenTree->Branch("isGood",      &isGood_);
   fgenTree->Branch("nPV", &nPV_);
   }

   if (isSherpaDiphoton_){
   fSherpaGenTree = fs->make<TTree>("fSherpaGenTree", "GENSherpaDiphotonTree");
   fSherpaGenTree->Branch("Event",             &fEventInfo,             ExoDiPhotons::eventBranchDefString.c_str());
   fSherpaGenTree->Branch("SherpaGenPhoton1",  &fSherpaGenPhoton1Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fSherpaGenTree->Branch("SherpaGenPhoton2",  &fSherpaGenPhoton2Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fSherpaGenTree->Branch("SherpaGenDiphoton", &fSherpaGenDiphotonInfo, ExoDiPhotons::diphotonBranchDefString.c_str());
   fSherpaGenTree->Branch("weightAll",         &SherpaWeightAll_);
   fSherpaGenTree->Branch("isGood",            &isGood_);
   fSherpaGenTree->Branch("nPV", &nPV_);
   }

}


nPhotonAnalyzer::~nPhotonAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// member functions

// ------------ method called for each event  ------------
void
nPhotonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;
   using namespace pat;

   //---Initialize
   ExoDiPhotons::InitEventInfo(       fEventInfo        );

   ExoDiPhotons::InitGenParticleInfo( fGenPhoton1Info   );
   ExoDiPhotons::InitGenParticleInfo( fGenPhoton2Info   );
   ExoDiPhotons::InitDiphotonInfo(    fGenDiPhotonInfo  );

   //---Handle, getByToken
   edm::Handle<vector<reco::GenParticle> > genParticles;
   edm::Handle<GenEventInfoProduct> genInfo;
   edm::Handle< double > rhoH;
   edm::Handle<edm::ValueMap<bool> > id_decisions[3];

   iEvent.getByToken(genParticlesToken_, genParticles);
   iEvent.getByToken(genInfoToken_,      genInfo);
   iEvent.getByToken(rhoToken_,          rhoH);
   iEvent.getByToken(phoLooseIdMapToken_, id_decisions[LOOSE]);
   iEvent.getByToken(phoMediumIdMapToken_,id_decisions[MEDIUM]);
   iEvent.getByToken(phoTightIdMapToken_ ,id_decisions[TIGHT]);

   //---Update Information
   ExoDiPhotons::FillBasicEventInfo(fEventInfo, iEvent);
   ExoDiPhotons::fillGenDiPhoInfo(  fGenPhoton1Info, fGenPhoton2Info, fGenDiPhotonInfo, genParticles);

   rho_ = *rhoH;

   //*Debugging!
   cout <<  "Run: "    << iEvent.id().run()
        << ", LS: "    << iEvent.id().luminosityBlock()
        << ", Event: " << iEvent.id().event()
        << endl;
   //Debugging!*

   //Fill
   if (isPythia8gen_)         fgenTree->Fill();
   if (isSherpaDiphoton_)     fSherpaGenTree->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void
nPhotonAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
nPhotonAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
nPhotonAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
DEFINE_FWK_MODULE(nPhotonAnalyzer);
