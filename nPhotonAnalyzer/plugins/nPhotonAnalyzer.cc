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
   photonsMiniAODToken_      = consumes<edm::View<pat::Photon> >        (ps.getParameter<edm::InputTag>("photonsMiniAOD"));
   recHitsEBTag_             = ps.getUntrackedParameter<edm::InputTag>("RecHitsEBTag",edm::InputTag("reducedEgamma:reducedEBRecHits"));
   recHitsEETag_             = ps.getUntrackedParameter<edm::InputTag>("RecHitsEETag",edm::InputTag("reducedEgamma:reducedEERecHits"));
   recHitsEBToken            = consumes <edm::SortedCollection<EcalRecHit> > (recHitsEBTag_);
   recHitsEEToken            = consumes <edm::SortedCollection<EcalRecHit> > (recHitsEETag_);
   isMC_                     =                                           ps.getParameter<bool>("isMC");
   islocal_                  =                                           ps.getParameter<bool>("islocal");
   isDAS_                    =                                           ps.getParameter<bool>("isDAS");
   outputFile_               =                                   TString(ps.getParameter<std::string>("outputFile"));

   if (islocal_){
   fgenTree = fs->make<TTree>("fgenTree","GENDiphotonTree");
   fgenTree->Branch("Event",       &fEventInfo,       ExoDiPhotons::eventBranchDefString.c_str());
   fgenTree->Branch("GenPhoton1",  &fGenPhoton1Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenPhoton2",  &fGenPhoton2Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenPhoton3",  &fGenPhoton3Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenDiPhoton12", &fGenDiphotonInfo12, ExoDiPhotons::diphotonBranchDefString.c_str());
   fgenTree->Branch("GenDiPhoton13", &fGenDiphotonInfo13, ExoDiPhotons::diphotonBranchDefString.c_str());
   fgenTree->Branch("GenDiPhoton23", &fGenDiphotonInfo23, ExoDiPhotons::diphotonBranchDefString.c_str());
   fgenTree->Branch("GenTriPhoton",  &fGenTriphotonInfo, ExoDiPhotons::triphotonBranchDefString.c_str());
   // fgenTree->Branch("DiPhoton12",   &fDiphotonInfo12, ExoDiPhotons::diphotonBranchDefString.c_str());
   // fgenTree->Branch("DiPhoton13",   &fDiphotonInfo13, ExoDiPhotons::diphotonBranchDefString.c_str());
   // fgenTree->Branch("DiPhoton23",   &fDiphotonInfo23, ExoDiPhotons::diphotonBranchDefString.c_str());
   // fgenTree->Branch("TriPhoton",    &fTriphotonInfo, ExoDiPhotons::triphotonBranchDefString.c_str());
   fgenTree->Branch("isGood",      &isGood_);
   fgenTree->Branch("nPV", &nPV_);

   xsec_ = ps.getParameter<double>("xsec");
   }

   if (isDAS_){
   fTree = fs->make<TTree>("fTree", "DiphotonTree");
   fTree->Branch("Event",       &fEventInfo,       ExoDiPhotons::eventBranchDefString.c_str());
   fTree->Branch("GenPhoton1",  &fGenPhoton1Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fTree->Branch("GenPhoton2",  &fGenPhoton2Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fTree->Branch("GenPhoton3",  &fGenPhoton3Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenDiPhoton12", &fGenDiphotonInfo12, ExoDiPhotons::diphotonBranchDefString.c_str());
   fgenTree->Branch("GenDiPhoton13", &fGenDiphotonInfo13, ExoDiPhotons::diphotonBranchDefString.c_str());
   fgenTree->Branch("GenDiPhoton23", &fGenDiphotonInfo23, ExoDiPhotons::diphotonBranchDefString.c_str());
   fTree->Branch("GenTriPhoton", &fGenTriphotonInfo, ExoDiPhotons::triphotonBranchDefString.c_str());
   fgenTree->Branch("Photon1",      &fPhoton1Info,      ExoDiPhotons::photonBranchDefString.c_str());
   fgenTree->Branch("Photon2",      &fPhoton2Info,      ExoDiPhotons::photonBranchDefString.c_str());
   fgenTree->Branch("Photon3",      &fPhoton3Info,      ExoDiPhotons::photonBranchDefString.c_str());
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
   ExoDiPhotons::InitPhotonInfo(fPhoton1Info);
   ExoDiPhotons::InitPhotonInfo(fPhoton2Info);
   ExoDiPhotons::InitPhotonInfo(fPhoton3Info);
   // ExoDiPhotons::InitDiphotonInfo(fDiphotonInfo12);
   // ExoDiPhotons::InitDiphotonInfo(fDiphotonInfo13);
   // ExoDiPhotons::InitDiphotonInfo(fDiphotonInfo23);
   // ExoDiPhotons::InitTriphotonInfo(fTriphotonInfo);

   //---Handle, getByToken
   //edm::Handle<vector<reco::GenParticle> > genParticles;
   edm::Handle<edm::View<reco::GenParticle> > genParticles;
   edm::Handle<GenEventInfoProduct>           genInfo;
   edm::Handle<edm::ValueMap<bool> >          id_decisions[3];
   edm::Handle<edm::View<pat::Photon> >       photons;
   edm::Handle< double >                      rhoH;
   edm::Handle<EcalRecHitCollection>          recHitsEB;
   edm::Handle<EcalRecHitCollection>          recHitsEE;
   edm::ESHandle<CaloTopology> caloTopology;

   iEvent.getByToken(genParticlesToken_,    genParticles);
   iEvent.getByToken(genInfoToken_,         genInfo);
   iEvent.getByToken(photonsMiniAODToken_,  photons);
   //iEvent.getByToken(rhoToken_,          rhoH);
   iEvent.getByToken(phoLooseIdMapToken_,   id_decisions[LOOSE]);
   iEvent.getByToken(phoMediumIdMapToken_,  id_decisions[MEDIUM]);
   iEvent.getByToken(phoTightIdMapToken_ ,  id_decisions[TIGHT]);
   iEvent.getByToken(phoTightIdMapToken_ ,  id_decisions[TIGHT]);
   iEvent.getByToken(recHitsEBToken,recHitsEB);
   iEvent.getByToken(recHitsEEToken,recHitsEE);
   iSetup.get<CaloTopologyRecord>().get(caloTopology);
   subDetTopologyEB_ = caloTopology->getSubdetectorTopology(DetId::Ecal,EcalBarrel);
   subDetTopologyEE_ = caloTopology->getSubdetectorTopology(DetId::Ecal,EcalEndcap);

   //---Update
   ExoDiPhotons::FillBasicEventInfo(fEventInfo, iEvent);
   ExoDiPhotons::FillGenEventInfo(fEventInfo, &(*genInfo));
   if (isDAS_) ExoDiPhotons::FillEventWeights(fEventInfo, outputFile_, nEventsSample_);
   if (islocal_) ExoDiPhotons::FillEventWeights(fEventInfo, xsec_, nEventsSample_);
   fillGenInfo(genParticles);
   if (isDAS_) fillPhotonInfo(photons, recHitsEB, recHitsEE, &id_decisions[0], fPhoton1Info, fPhoton2Info, fPhoton3Info);
   // fillPhotonInfo(photons);
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

      // fill gen triphoton info
}//end of fillGenInfo

void nPhotonAnalyzer::fillPhotonInfo(const edm::Handle<edm::View<pat::Photon> >& photons,
                                     const edm::Handle<EcalRecHitCollection>& recHitsEB,
                                     const edm::Handle<EcalRecHitCollection>& recHitsEE,
			                               const edm::Handle<edm::ValueMap<bool> >* id_decisions,
			                               ExoDiPhotons::photonInfo_t& photon1Info,
                                     ExoDiPhotons::photonInfo_t& photon2Info,
                                     ExoDiPhotons::photonInfo_t& photon3Info){

      bool isSat = false;

      // Store Information in these vectors
      std::vector<edm::Ptr<pat::Photon>> goodPhotons;
      std::vector<edm::Ptr<pat::Photon>> selectedPhotons[2][2]; // combinations of real and fake for both leading candidates
      std::vector<std::pair<edm::Ptr<pat::Photon>, int> > realAndFakePhotons;

      for (size_t i = 0; i < photons->size(); ++i){
        const auto pho = photons->ptrAt(i);
        // print photon info
        //cout << "Photon: " << "pt = " << pho->pt() << "; eta = " << pho->eta() << "; phi = " << pho->phi() << endl;
        // bool isSat = false;
        bool isSat = ExoDiPhotons::isSaturated(&(*pho), &(*recHitsEB), &(*recHitsEE), &(*subDetTopologyEB_), &(*subDetTopologyEE_));

        //To-do: Apply high pT, VID (loose, medium, tight) here with flags
        bool pass_HighPtID = ExoDiPhotons::passHighPtID(&(*pho), rho_, isSat);

        if(pass_HighPtID){
          goodPhotons.push_back(pho);
          realAndFakePhotons.push_back(std::pair<edm::Ptr<pat::Photon>, int>(pho, TRUE));
        }
      }// end of photon loop

      sort(goodPhotons.begin(),goodPhotons.end(),ExoDiPhotons::comparePhotonsByPt);
      sort(realAndFakePhotons.begin(), realAndFakePhotons.end(), ExoDiPhotons::comparePhotonPairsByPt);

      if (goodPhotons.size() >= 2){
        isGood_ = true;
        //photonFiller(goodPhotons, recHitsEB, recHitsEE, )
        photonFiller(goodPhotons, recHitsEB, recHitsEE, &id_decisions[0], photon1Info, photon2Info, photon3Info);
        // To-do: isMC genparticle fill
        // To-do: isClosureTest_
      }

}//end of fillPhotonInfo

void nPhotonAnalyzer::photonFiller(const std::vector<edm::Ptr<pat::Photon>>& photons,
      				                         const edm::Handle<EcalRecHitCollection>& recHitsEB,
                                       const edm::Handle<EcalRecHitCollection>& recHitsEE,
      				                         const edm::Handle<edm::ValueMap<bool> >* id_decisions,
      				                         ExoDiPhotons::photonInfo_t& photon1Info,
                                       ExoDiPhotons::photonInfo_t& photon2Info,
                                       ExoDiPhotons::photonInfo_t& photon3Info){

            // std::cout << "Photon 1 pt = " << photons[0]->pt() << "; eta = " << photons[0]->eta() << "; phi = " << photons[0]->phi() << std::endl;
            // std::cout << "Photon 2 pt = " << photons[1]->pt() << "; eta = " << photons[1]->eta() << "; phi = " << photons[1]->phi() << std::endl;
            // fill photon saturation
            photon1Info.isSaturated = ExoDiPhotons::isSaturated(&(*photons[0]), &(*recHitsEB), &(*recHitsEE), &(*subDetTopologyEB_), &(*subDetTopologyEE_));
            std::cout << "Photon 1 isSat: " << photon1Info.isSaturated << std::endl;
            ExoDiPhotons::FillBasicPhotonInfo(photon1Info, &(*photons[0]));
            // ExoDiPhotons::FillPhotonIDInfo(photon1Info, &(*photons[0]), rho_, photon1Info.isSaturated);
            //To-do FillPhotonEGMidInfo

            photon2Info.isSaturated = ExoDiPhotons::isSaturated(&(*photons[1]), &(*recHitsEB), &(*recHitsEE), &(*subDetTopologyEB_), &(*subDetTopologyEE_));
            std::cout << "Photon 2 isSat: " << photon2Info.isSaturated << std::endl;
            ExoDiPhotons::FillBasicPhotonInfo(photon2Info, &(*photons[1]));
            // ExoDiPhotons::FillPhotonIDInfo(photon2Info, &(*photons[1]), rho_, photon2Info.isSaturated);

            photon3Info.isSaturated = ExoDiPhotons::isSaturated(&(*photons[2]), &(*recHitsEB), &(*recHitsEE), &(*subDetTopologyEB_), &(*subDetTopologyEE_));
            std::cout << "Photon 3 isSat: " << photon3Info.isSaturated << std::endl;
            ExoDiPhotons::FillBasicPhotonInfo(photon3Info, &(*photons[2]));
            // ExoDiPhotons::FillPhotonIDInfo(photon3Info, &(*photons[2]), rho_, photon3Info.isSaturated);

}//end photonFiller


DEFINE_FWK_MODULE(nPhotonAnalyzer);
