#include "multiphoton-analysis/CommonClasses/interface/nPhotonAnalyzer.h"

using namespace std;
using namespace edm;

nPhotonAnalyzer::nPhotonAnalyzer(const edm::ParameterSet& ps)

{
   usesResource("TFileService");

   isGood_                    = false;
   nPV_                       = 0;
   SherpaGenPhoton0_iso_      = 9999.99;
   SherpaGenPhoton1_iso_      = 9999.99;

   genParticlesToken_        = consumes<edm::View<reco::GenParticle> >  (ps.getParameter<InputTag>("genparticles"));
   //genParticlesMiniAODToken_ = consumes<edm::View<reco::GenParticle> >  (ps.getParameter<InputTag>("genParticlesMiniAOD"));
   //genParticlesToken_        = consumes<vector<reco::GenParticle>>      (ps.getParameter<InputTag>("genparticles"));
   //rhoToken_                 = consumes<double>                         (ps.getParameter<edm::InputTag>("rho"));
   phoLooseIdMapToken_       = consumes<edm::ValueMap<bool> >           (ps.getParameter<edm::InputTag>("phoLooseIdMap"));
   phoMediumIdMapToken_      = consumes<edm::ValueMap<bool> >           (ps.getParameter<edm::InputTag>("phoMediumIdMap"));
   phoTightIdMapToken_       = consumes<edm::ValueMap<bool> >           (ps.getParameter<edm::InputTag>("phoTightIdMap"));
   nEventsSample_            =                                          (ps.getParameter<uint32_t>("nEventsSample"));
   genInfoToken_             = consumes<GenEventInfoProduct>            (ps.getParameter<edm::InputTag>("genInfo"));
  //genParticlesMiniAODToken_ = mayConsume<edm::View<reco::GenParticle> >(ps.getParameter<edm::InputTag>("genParticlesMiniAOD"));
   isMC_                     =                                           ps.getParameter<bool>("isMC");
   isPythia8gen_             =                                           ps.getParameter<bool>("isPythia8gen");
   isSherpaDiphoton_         =                                           ps.getParameter<bool>("isSherpaDiphoton");
   outputFile_               =                                   TString(ps.getParameter<std::string>("outputFile"));


   fgenTree = fs->make<TTree>("fgenTree","GENDiphotonTree");
   fgenTree->Branch("Event",       &fEventInfo,       ExoDiPhotons::eventBranchDefString.c_str());
   fgenTree->Branch("GenPhoton1",  &fGenPhoton1Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenPhoton2",  &fGenPhoton2Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenDiPhoton", &fGenDiphotonInfo, ExoDiPhotons::diphotonBranchDefString.c_str());
   fgenTree->Branch("weightAll",   &SherpaWeightAll_);
   fgenTree->Branch("isGood",      &isGood_);
   fgenTree->Branch("nPV", &nPV_);

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
   ExoDiPhotons::InitEventInfo(fEventInfo);
   ExoDiPhotons::InitGenParticleInfo(fGenPhoton1Info);
   ExoDiPhotons::InitGenParticleInfo(fGenPhoton2Info);
   ExoDiPhotons::InitDiphotonInfo(fGenDiphotonInfo);

   //---Handle, getByToken
   //edm::Handle<vector<reco::GenParticle> > genParticles;
   edm::Handle<edm::View<reco::GenParticle> > genParticles;
   edm::Handle<GenEventInfoProduct>           genInfo;
   edm::Handle<edm::ValueMap<bool> >          id_decisions[3];

   iEvent.getByToken(genParticlesToken_,    genParticles);
   iEvent.getByToken(genInfoToken_,         genInfo);
   //iEvent.getByToken(rhoToken_,          rhoH);
   iEvent.getByToken(phoLooseIdMapToken_,   id_decisions[LOOSE]);
   iEvent.getByToken(phoMediumIdMapToken_,  id_decisions[MEDIUM]);
   iEvent.getByToken(phoTightIdMapToken_ ,  id_decisions[TIGHT]);

   //---Update
   ExoDiPhotons::FillBasicEventInfo(fEventInfo, iEvent);
   fillGenInfo(genParticles);
   ExoDiPhotons::FillEventWeights(fEventInfo, outputFile_, nEventsSample_);
   SherpaWeightAll_ = fEventInfo.weightAll;
   cout << "SherpaWeightAll_ =" << SherpaWeightAll_ << endl;
   //ExoDiPhotons::FillBasicEventInfo(fEventInfo, iEvent);
   //ExoDiPhotons::fillGenDiPhoInfo(  fGenPhoton1Info, fGenPhoton2Info, fGenDiPhotonInfo, genParticles);


   //Debugging!
   cout <<  "Run: "    << iEvent.id().run()
        << ", LS: "    << iEvent.id().luminosityBlock()
        << ", Event: " << iEvent.id().event()
        << endl;
   //Debugging!

   //Fill
   fgenTree->Fill();


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


void nPhotonAnalyzer::fillGenInfo(const edm::Handle<edm::View<reco::GenParticle> > genParticles){

      // Store Information in these vectors
      vector< edm::Ptr<const reco::GenParticle> > genPhotons;
      vector<int> interactingPartons;

      //Test
      //const reco::GenParticle *genPho1 = NULL;
      //const reco::GenParticle *genPho2 = NULL;

      for (size_t i = 0; i < genParticles->size(); ++i){
        const auto gen = genParticles->ptrAt(i);
        //Pythia8 status 20-30 is Hard interaction
        if (gen->isHardProcess() && gen->pt() == 0)    interactingPartons.push_back(gen->pdgId());
        if (gen->isHardProcess() && gen->pdgId()==22){
          genPhotons.push_back(gen);
          cout << "Hard Process genParticle: "
               << "status = "  << gen->status()
               << "; pdgId = " << gen->pdgId()
               << "; pt = "    << gen->pt()
               << "; eta = "   << gen->eta()
               << "; phi = "   << gen->phi()
               << endl;
        }
      }//end for loop over gen particles

      sort(genPhotons.begin(), genPhotons.end(), ExoDiPhotons::comparePhotonsByPt);
      if(interactingPartons.size() == 2){
        fEventInfo.interactingParton1PdgId = interactingPartons[0];
        fEventInfo.interactingParton2PdgId = interactingPartons[1];
      }
      else cout << "Exactly two interacting partons not found!" << endl;

      // Samples with only fakes may have no hard-process photons
      if(genPhotons.size() < 1) return;
      const reco::GenParticle *genPhoton1 = &(*genPhotons.at(0));
      if (genPhoton1) ExoDiPhotons::FillGenParticleInfo(fGenPhoton1Info, genPhoton1);

      // Samples with one fake may have only one hard-process photon
      if(genPhotons.size() < 2) return;
      // fill gen photon info
      const reco::GenParticle *genPhoton2 = &(*genPhotons.at(1));
      if (genPhoton2) ExoDiPhotons::FillGenParticleInfo(fGenPhoton2Info, genPhoton2);

      // fill gen diphoton info
      if (genPhoton1 && genPhoton2) ExoDiPhotons::FillDiphotonInfo(fGenDiphotonInfo,genPhoton1,genPhoton2);

}//end of fillGenInfo


DEFINE_FWK_MODULE(nPhotonAnalyzer);
