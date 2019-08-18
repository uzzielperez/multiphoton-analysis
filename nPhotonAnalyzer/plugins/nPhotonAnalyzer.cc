#include "multiphoton-analysis/CommonClasses/interface/nPhotonAnalyzer.h"

using namespace std;
using namespace edm;

nPhotonAnalyzer::nPhotonAnalyzer(const edm::ParameterSet& ps)

{
   usesResource("TFileService");
   isGood_                    = false;
   nPV_                       = 0;
   GenPhoton0_iso_      = 9999.99;
   GenPhoton1_iso_      = 9999.99;

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
   islocal_                  =                                           ps.getParameter<bool>("islocal");
   isDAS_                    =                                           ps.getParameter<bool>("isDAS");
   outputFile_               =                                   TString(ps.getParameter<std::string>("outputFile"));

   if (islocal_){
   fgenTree = fs->make<TTree>("fgenTree","GENDiphotonTree");
   fgenTree->Branch("Event",       &fEventInfo,       ExoDiPhotons::eventBranchDefString.c_str());
   fgenTree->Branch("GenPhoton1",    &fGenPhoton1Info,    ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenPhoton2",    &fGenPhoton2Info,    ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenPhoton3",    &fGenPhoton3Info,    ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenDiPhoton12", &fGenDiphotonInfo12, ExoDiPhotons::diphotonBranchDefString.c_str());
   fgenTree->Branch("GenDiPhoton13", &fGenDiphotonInfo13, ExoDiPhotons::diphotonBranchDefString.c_str());
   fgenTree->Branch("GenDiPhoton23", &fGenDiphotonInfo23, ExoDiPhotons::diphotonBranchDefString.c_str());
   fgenTree->Branch("GenTriPhoton",  &fGenTriphotonInfo,  ExoDiPhotons::triphotonBranchDefString.c_str());
   fgenTree->Branch("isGood",      &isGood_);
   fgenTree->Branch("nPV", &nPV_);
   }

   if (isDAS_){
   fTree = fs->make<TTree>("fTree", "DiphotonTree");
   fTree->Branch("Event",       &fEventInfo,       ExoDiPhotons::eventBranchDefString.c_str());
   fTree->Branch("GenPhoton1",  &fGenPhoton1Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fTree->Branch("GenPhoton2",  &fGenPhoton2Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fTree->Branch("Gendiphoton", &fGenDiphotonInfo12, ExoDiPhotons::diphotonBranchDefString.c_str());
   fTree->Branch("isGood",            &isGood_);
   fTree->Branch("nPV", &nPV_);
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
   ExoDiPhotons::InitEventInfo(fEventInfo);
   ExoDiPhotons::InitGenParticleInfo(fGenPhoton1Info);
   ExoDiPhotons::InitGenParticleInfo(fGenPhoton2Info);
   ExoDiPhotons::InitGenParticleInfo(fGenPhoton3Info);
   ExoDiPhotons::InitDiphotonInfo(fGenDiphotonInfo12);
   ExoDiPhotons::InitDiphotonInfo(fGenDiphotonInfo13);
   ExoDiPhotons::InitDiphotonInfo(fGenDiphotonInfo23);
   ExoDiPhotons::InitTriphotonInfo(fGenTriphotonInfo);
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
   ExoDiPhotons::FillGenEventInfo(fEventInfo, &(*genInfo));
   if (isDAS_) ExoDiPhotons::FillEventWeights(fEventInfo, outputFile_, nEventsSample_);
   fillGenInfo(genParticles);
   //ExoDiPhotons::FillBasicEventInfo(fEventInfo, iEvent);
   //ExoDiPhotons::fillGenDiPhoInfo(  fGenPhoton1Info, fGenPhoton2Info, fGenDiPhotonInfo, genParticles);


   //Debugging!
   cout <<  "Run: "    << iEvent.id().run()
        << ", LS: "    << iEvent.id().luminosityBlock()
        << ", Event: " << iEvent.id().event()
        << endl;
   //Debugging!

   //Fill
   if (islocal_)         fgenTree->Fill();
   if (isDAS_)              fTree->Fill();

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

      //---- Photon Information
      // Samples with only fakes may have no hard-process photons
      // Samples with one fake may have only one hard-process photon in diphoton pair
      // Samples with fakes may have only one hard-process photon in the triphoton system

      if(genPhotons.size() < 1) return;
      const reco::GenParticle *genPhoton1 = &(*genPhotons.at(0));
      if (genPhoton1) ExoDiPhotons::FillGenParticleInfo(fGenPhoton1Info, genPhoton1);

      if(genPhotons.size() < 2) return;
      const reco::GenParticle *genPhoton2 = &(*genPhotons.at(1));
      if (genPhoton2) ExoDiPhotons::FillGenParticleInfo(fGenPhoton2Info, genPhoton2);

      if(genPhotons.size() < 3) return;
      const reco::GenParticle *genPhoton3 = &(*genPhotons.at(2));
      if (genPhoton3) ExoDiPhotons::FillGenParticleInfo(fGenPhoton3Info, genPhoton3);

      //---- Diphoton/Triphoton Information
      if (genPhoton1 && genPhoton2) ExoDiPhotons::FillDiphotonInfo(fGenDiphotonInfo12,genPhoton1,genPhoton2);
      if (genPhoton1 && genPhoton3) ExoDiPhotons::FillDiphotonInfo(fGenDiphotonInfo13,genPhoton1,genPhoton3);
      if (genPhoton2 && genPhoton3) ExoDiPhotons::FillDiphotonInfo(fGenDiphotonInfo23,genPhoton2,genPhoton3);
      if (genPhoton1 && genPhoton2 && genPhoton3) ExoDiPhotons::FillTriphotonInfo(fGenTriphotonInfo,genPhoton1,genPhoton2,genPhoton3);

}//end of fillGenInfo


DEFINE_FWK_MODULE(nPhotonAnalyzer);
