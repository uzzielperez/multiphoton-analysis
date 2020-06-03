#include "multiphoton-analysis/CommonClasses/interface/nPhotonAnalyzer.h"

using namespace std;
using namespace edm;

nPhotonAnalyzer::nPhotonAnalyzer(const edm::ParameterSet& ps)
   :effAreaChHadrons_( (ps.getParameter<edm::FileInPath>("effAreaChHadFile")).fullPath() ),
    effAreaNeuHadrons_( (ps.getParameter<edm::FileInPath>("effAreaNeuHadFile")).fullPath() ),
    effAreaPhotons_( (ps.getParameter<edm::FileInPath>("effAreaPhoFile")).fullPath() )
{
   usesResource("TFileService");
   isGood_           = false;
   is3GenRecoed_     = false;
   isgenPho1_recoed_ = false;
   isgenPho2_recoed_ = false;
   isgenPho3_recoed_ = false;
   nPV_                 = 0;
   GenPhoton0_iso_      = 9999.99;
   GenPhoton1_iso_      = 9999.99;
   // EffectiveAreas effAreaChHadrons_;
   // EffectiveAreas effAreaNeuHadrons_;
   // EffectiveAreas effAreaPhotons_;

   genParticlesToken_        = consumes<edm::View<reco::GenParticle> > (ps.getParameter<InputTag>("genparticles"));
   //genParticlesMiniAODToken_ = consumes<edm::View<reco::GenParticle> >  (ps.getParameter<InputTag>("genParticlesMiniAOD"));
   //genParticlesToken_        = consumes<vector<reco::GenParticle>>      (ps.getParameter<InputTag>("genparticles"));
   rhoToken_                 = consumes<double>                      (ps.getParameter<edm::InputTag>("rho"));
   // effAreaChHadrons_         = ps.getParameter<edm::FileInPath>("effAreaChHadFile").fullPath();
   // effAreaNeuHadrons_        = ps.getParameter<edm::FileInPath>("effAreaNeuHadFile").fullPath();
   // effAreaPhotons_           = ps.getParameter<edm::FileInPath>("effAreaPhoFile").fullPath();
   phoLooseIdMapToken_       = consumes<edm::ValueMap<bool> >         (ps.getParameter<edm::InputTag>("phoLooseIdMap"));
   phoMediumIdMapToken_      = consumes<edm::ValueMap<bool> >         (ps.getParameter<edm::InputTag>("phoMediumIdMap"));
   phoTightIdMapToken_       = consumes<edm::ValueMap<bool> >         (ps.getParameter<edm::InputTag>("phoTightIdMap"));
   nEventsSample_            =                                        (ps.getParameter<uint32_t>("nEventsSample"));
   genInfoToken_             = consumes<GenEventInfoProduct>          (ps.getParameter<edm::InputTag>("genInfo"));
   //genParticlesMiniAODToken_ = mayConsume<edm::View<reco::GenParticle> >(ps.getParameter<edm::InputTag>("genParticlesMiniAOD"));
   photonsMiniAODToken_      = consumes<edm::View<pat::Photon> >      (ps.getParameter<edm::InputTag>("photonsMiniAOD"));
   recHitsEBTag_             = ps.getUntrackedParameter<edm::InputTag>("RecHitsEBTag",edm::InputTag("reducedEgamma:reducedEBRecHits"));
   recHitsEETag_             = ps.getUntrackedParameter<edm::InputTag>("RecHitsEETag",edm::InputTag("reducedEgamma:reducedEERecHits"));
   recHitsEBToken            = consumes <edm::SortedCollection<EcalRecHit> > (recHitsEBTag_);
   recHitsEEToken            = consumes <edm::SortedCollection<EcalRecHit> > (recHitsEETag_);
   isMC_                     =                                         ps.getParameter<bool>("isMC");
   islocal_                  =                                         ps.getParameter<bool>("islocal");
   isDAS_                    =                                         ps.getParameter<bool>("isDAS");
   IDmode_                   =                                         ps.getParameter<std::string>("IDmode");
   outputFile_               =                                 TString(ps.getParameter<std::string>("outputFile"));

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
   xsec_ = ps.getParameter<double>("xsec");
   }

   if (isDAS_){
   fTree = fs->make<TTree>("fTree", "DiphotonTree");
   fTree->Branch("Event",       &fEventInfo,       ExoDiPhotons::eventBranchDefString.c_str());
   fTree->Branch("GenPhoton1",  &fGenPhoton1Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fTree->Branch("GenPhoton2",  &fGenPhoton2Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fTree->Branch("GenPhoton3",  &fGenPhoton3Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fTree->Branch("GenDiPhoton12", &fGenDiphotonInfo12, ExoDiPhotons::diphotonBranchDefString.c_str());
   fTree->Branch("GenDiPhoton13", &fGenDiphotonInfo13, ExoDiPhotons::diphotonBranchDefString.c_str());
   fTree->Branch("GenDiPhoton23", &fGenDiphotonInfo23, ExoDiPhotons::diphotonBranchDefString.c_str());
   fTree->Branch("GenTriPhoton", &fGenTriphotonInfo, ExoDiPhotons::triphotonBranchDefString.c_str());
   fTree->Branch("Photon1",      &fPhoton1Info,      ExoDiPhotons::photonBranchDefString.c_str());
   fTree->Branch("Photon2",      &fPhoton2Info,      ExoDiPhotons::photonBranchDefString.c_str());
   fTree->Branch("Photon3",      &fPhoton3Info,      ExoDiPhotons::photonBranchDefString.c_str());
   fTree->Branch("DiPhoton12",   &fDiphotonInfo12, ExoDiPhotons::diphotonBranchDefString.c_str());
   fTree->Branch("DiPhoton13",   &fDiphotonInfo13, ExoDiPhotons::diphotonBranchDefString.c_str());
   fTree->Branch("DiPhoton23",   &fDiphotonInfo23, ExoDiPhotons::diphotonBranchDefString.c_str());
   fTree->Branch("TriPhoton",    &fTriphotonInfo, ExoDiPhotons::triphotonBranchDefString.c_str());
   fTree->Branch("isGood",           &isGood_);
   fTree->Branch("nPV", &nPV_);
   // For

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
   iEvent.getByToken(rhoToken_,          rhoH);
   iEvent.getByToken(phoLooseIdMapToken_,   id_decisions[LOOSE]);
   iEvent.getByToken(phoMediumIdMapToken_,  id_decisions[MEDIUM]);
   iEvent.getByToken(phoTightIdMapToken_ ,  id_decisions[TIGHT]);
   iEvent.getByToken(recHitsEBToken,recHitsEB);
   iEvent.getByToken(recHitsEEToken,recHitsEE);
   iSetup.get<CaloTopologyRecord>().get(caloTopology);
   subDetTopologyEB_ = caloTopology->getSubdetectorTopology(DetId::Ecal,EcalBarrel);
   subDetTopologyEE_ = caloTopology->getSubdetectorTopology(DetId::Ecal,EcalEndcap);
   rho_ = *rhoH;


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
   ExoDiPhotons::InitDiphotonInfo(fDiphotonInfo12);
   ExoDiPhotons::InitDiphotonInfo(fDiphotonInfo13);
   ExoDiPhotons::InitDiphotonInfo(fDiphotonInfo23);
   ExoDiPhotons::InitTriphotonInfo(fTriphotonInfo);
   //---Update
   ExoDiPhotons::FillBasicEventInfo(fEventInfo, iEvent);
   ExoDiPhotons::FillGenEventInfo(fEventInfo, &(*genInfo));

   if (islocal_){
     ExoDiPhotons::FillEventWeights(fEventInfo, xsec_, nEventsSample_);
     fillGenInfo(genParticles);
     //cout << "isGood: " << isGood_ << endl;
     fgenTree->Fill();
   }

   if (isDAS_){
     ExoDiPhotons::FillEventWeights(fEventInfo, outputFile_, nEventsSample_);
     fillInfo(genParticles, photons);
     fillPhotonInfo(genParticles, photons, recHitsEB, recHitsEE, &id_decisions[0], fPhoton1Info, fPhoton2Info, fPhoton3Info, fDiphotonInfo12, fDiphotonInfo13, fDiphotonInfo23, fTriphotonInfo);
     //cout << "isGood: " << isGood_ << endl;
     fTree->Fill();
   }
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

      //-- Loop over gen particles to store photons
      for (size_t i = 0; i < genParticles->size(); ++i){
       const auto gen = genParticles->ptrAt(i);
       //Pythia8 status 20-30 is Hard interaction
       if (gen->isHardProcess() && gen->pt() == 0)    interactingPartons.push_back(gen->pdgId());
       if (gen->isHardProcess() && gen->pdgId()==22){
         genPhotons.push_back(gen);
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

      if (genPhotons.size() > 3) exit(1);

      if (genPhotons.size() < 1) return;
      const reco::GenParticle *genPhoton1 = &(*genPhotons.at(0));
      if (genPhoton1) ExoDiPhotons::FillGenParticleInfo(fGenPhoton1Info, genPhoton1);

      if (genPhotons.size() < 2) return;
      const reco::GenParticle *genPhoton2 = &(*genPhotons.at(1));
      if (genPhoton2) ExoDiPhotons::FillGenParticleInfo(fGenPhoton2Info, genPhoton2);

      if (genPhotons.size() < 3) return;
      const reco::GenParticle *genPhoton3 = &(*genPhotons.at(2));
      if (genPhoton3) ExoDiPhotons::FillGenParticleInfo(fGenPhoton3Info, genPhoton3);


      //---- Diphoton/Triphoton Information
      if (genPhoton1 && genPhoton2) ExoDiPhotons::FillDiphotonInfo(fGenDiphotonInfo12,genPhoton1,genPhoton2);
      if (genPhoton1 && genPhoton3) ExoDiPhotons::FillDiphotonInfo(fGenDiphotonInfo13,genPhoton1,genPhoton3);
      if (genPhoton2 && genPhoton3) ExoDiPhotons::FillDiphotonInfo(fGenDiphotonInfo23,genPhoton2,genPhoton3);
      if (genPhoton1 && genPhoton2 && genPhoton3) ExoDiPhotons::FillTriphotonInfo(fGenTriphotonInfo,genPhoton1,genPhoton2,genPhoton3);
      // fill gen triphoton info
}//end of fillGenInfo

void nPhotonAnalyzer::fillInfo(const edm::Handle<edm::View<reco::GenParticle> > genParticles,
                                  const edm::Handle<edm::View<pat::Photon> >& photons){

      // Store Information in these vectors
      vector< edm::Ptr<const reco::GenParticle> > genPhotons;
      std::vector<edm::Ptr<pat::Photon>> patPhotons;
      vector<int> interactingPartons;

      //-- Loop over gen particles to store photons
      for (size_t i = 0; i < genParticles->size(); ++i){
       const auto gen = genParticles->ptrAt(i);
       //Pythia8 status 20-30 is Hard interaction
       if (gen->isHardProcess() && gen->pt() == 0)    interactingPartons.push_back(gen->pdgId());
       if (gen->isHardProcess() && gen->pdgId()==22){
         genPhotons.push_back(gen);
         // isRecoed = ExoDiPhotons::genRecoMatch(&(*gen), photons);
         // if (isRecoed && (gen->status()==1)){
         //   cout << "Hard Process genParticle: " << "status = "  << gen->status() << "; pdgId = " << gen->pdgId()
         //        << "; pt = "    << gen->pt()    << "; eta = "   << gen->eta()    << "; phi = "   << gen->phi()   << endl;
         // }
       }
     }//end for loop over gen particles

     //--- Loop over reco particles
     for (size_t i = 0; i < photons->size(); ++i){
       const auto pho = photons->ptrAt(i);
       patPhotons.push_back(pho);
       //std::cout << "Photon: pt = " << pho->pt() << "; eta = " << pho->eta() << "; phi = " << pho->phi() << std::endl;
     }

      sort(genPhotons.begin(), genPhotons.end(), ExoDiPhotons::comparePhotonsByPt);
      sort(patPhotons.begin(),patPhotons.end(), ExoDiPhotons::comparePhotonsByPt);

      if(interactingPartons.size() == 2){
        fEventInfo.interactingParton1PdgId = interactingPartons[0];
        fEventInfo.interactingParton2PdgId = interactingPartons[1];
      }

      else cout << "Exactly two interacting partons not found!" << endl;

      //---- Photon Information
      // Samples with only fakes may have no hard-process photons
      // Samples with one fake may have only one hard-process photon in diphoton pair
      // Samples with fakes may have only one hard-process photon in the triphoton system

      auto match_tuple = ExoDiPhotons::genpatmatchInfo(genPhotons, patPhotons);
      std::vector<bool> matchInfo       = std::get<0>(match_tuple);
      std::vector<double> minDRvec      = std::get<1>(match_tuple);
      std::vector<double> minDpTvec     = std::get<2>(match_tuple);
      std::vector<bool> ptdRmatchInfo   = std::get<3>(match_tuple);
      std::vector <std::tuple <int, int>> genpatindices = std::get<4>(match_tuple);
      std::vector<double> minDphivec    = std::get<5>(match_tuple);
      std::vector<double> minDetavec    = std::get<6>(match_tuple);
      std::vector<const pat::Photon *> patmatch       = std::get<7>(match_tuple); // the collection of pat photons that matched a gen photon
      std::vector<const reco::GenParticle *> genmatch = std::get<8>(match_tuple);

      if (genPhotons.size() > 3) exit(1);
      if(genPhotons.size() < 1) return;
      const reco::GenParticle *genPhoton1 = &(*genPhotons.at(0));
      if (genPhoton1){
        ExoDiPhotons::FillGenParticleInfo(fGenPhoton1Info, genPhoton1);
        ExoDiPhotons::FillGenPATmatchInfo(fGenPhoton1Info, matchInfo.at(0), minDRvec.at(0), minDpTvec.at(0),
                                          ptdRmatchInfo.at(0), genpatindices.at(0), minDphivec.at(0), minDetavec.at(0));
        if ( matchInfo.at(0) ) std::cout << "MATCH FOUND for genpho1 - minDR: " << minDRvec.at(0) << std::endl;
        if ( !matchInfo.at(0) ) std::cout << "MATCH not FOUND! for genpho1 - minDR: " << minDRvec.at(0) << std::endl;
      }

      if(genPhotons.size() < 2) return;
      const reco::GenParticle *genPhoton2 = &(*genPhotons.at(1));
      if (genPhoton2){
        ExoDiPhotons::FillGenParticleInfo(fGenPhoton2Info, genPhoton2);
        ExoDiPhotons::FillGenPATmatchInfo(fGenPhoton2Info, matchInfo.at(1), minDRvec.at(1), minDpTvec.at(1),
                                         ptdRmatchInfo.at(1), genpatindices.at(1), minDphivec.at(1), minDetavec.at(1));
        if ( matchInfo.at(1) ) std::cout << "MATCH FOUND for genpho2 - minDR: " << minDRvec.at(1) << std::endl;
        if ( !matchInfo.at(1) ) std::cout << "MATCH not FOUND! for genpho2 - minDR: " << minDRvec.at(1) << std::endl;
      }

      if(genPhotons.size() < 3) return;
      const reco::GenParticle *genPhoton3 = &(*genPhotons.at(2));
      if (genPhoton3){
        ExoDiPhotons::FillGenParticleInfo(fGenPhoton3Info, genPhoton3);
        ExoDiPhotons::FillGenPATmatchInfo(fGenPhoton3Info, matchInfo.at(2), minDRvec.at(2), minDpTvec.at(2),
                                          ptdRmatchInfo.at(2), genpatindices.at(2),  minDphivec.at(2), minDetavec.at(2));
       if ( matchInfo.at(2) ) std::cout << "MATCH FOUND for genpho2 - minDR: " << minDRvec.at(2) << std::endl;
       if ( !matchInfo.at(2) ) std::cout << "MATCH not FOUND! for genpho3 - minDR: " << minDRvec.at(2) << std::endl;
      }

      //---- Diphoton/Triphoton Information
      if (genPhoton1 && genPhoton2) ExoDiPhotons::FillDiphotonInfo(fGenDiphotonInfo12,genPhoton1,genPhoton2);
      if (genPhoton1 && genPhoton3) ExoDiPhotons::FillDiphotonInfo(fGenDiphotonInfo13,genPhoton1,genPhoton3);
      if (genPhoton2 && genPhoton3) ExoDiPhotons::FillDiphotonInfo(fGenDiphotonInfo23,genPhoton2,genPhoton3);
      if (genPhoton1 && genPhoton2 && genPhoton3) ExoDiPhotons::FillTriphotonInfo(fGenTriphotonInfo,genPhoton1,genPhoton2,genPhoton3);
      // fill gen triphoton info
}//end of fillInfo

void nPhotonAnalyzer::fillPhotonInfo(const edm::Handle<edm::View<reco::GenParticle> > genParticles,
                                     const edm::Handle<edm::View<pat::Photon> >& photons,
                                     const edm::Handle<EcalRecHitCollection>& recHitsEB,
                                     const edm::Handle<EcalRecHitCollection>& recHitsEE,
			                               const edm::Handle<edm::ValueMap<bool> >* id_decisions,
			                               ExoDiPhotons::photonInfo_t& photon1Info,
                                     ExoDiPhotons::photonInfo_t& photon2Info,
                                     ExoDiPhotons::photonInfo_t& photon3Info,
                                     ExoDiPhotons::diphotonInfo_t& diphotonInfo12,
                                     ExoDiPhotons::diphotonInfo_t& diphotonInfo13,
                                     ExoDiPhotons::diphotonInfo_t& diphotonInfo23,
                                     ExoDiPhotons::triphotonInfo_t& triphotonInfo){

      bool isSat = false;

      // Store Information in these vectors
      std::vector<edm::Ptr<pat::Photon>> goodPhotons;
      std::vector<edm::Ptr<pat::Photon>> selectedPhotons[2][2]; // combinations of real and fake for both leading candidates
      std::vector<std::pair<edm::Ptr<pat::Photon>, int> > realAndFakePhotons;

      for (size_t i = 0; i < photons->size(); ++i){
        const auto pho = photons->ptrAt(i);
        // cout << "Photon: " << "pt = " << pho->pt() << "; eta = " << pho->eta() << "; phi = " << pho->phi() << endl;
        isSat = ExoDiPhotons::isSaturated(&(*pho), &(*recHitsEB), &(*recHitsEE), &(*subDetTopologyEB_), &(*subDetTopologyEE_));

        //To-do: Apply high pT, VID (loose, medium, tight) here with flags
        bool pass_HighPtID = ExoDiPhotons::passHighPtID(&(*pho), rho_, isSat);
        bool passEGMLooseID  = (*id_decisions[LOOSE])[pho];
        bool passEGMMediumID = (*id_decisions[MEDIUM])[pho];
        bool passEGMTightID  = (*id_decisions[TIGHT])[pho];

        bool pass_ID_version = pass_HighPtID;

        if ( IDmode_ == "LOOSE"     ) pass_ID_version = passEGMLooseID;
        if ( IDmode_ == "MEDIUM"    ) pass_ID_version = passEGMMediumID;
        if ( IDmode_ == "TIGHT"     ) pass_ID_version = passEGMTightID;
        if ( IDmode_ == "hightPTID" ) pass_ID_version = pass_HighPtID;
        //if ( IDmode_ == "NOid"      ) pass_ID_version = 1;
        if ((IDmode_ != "LOOSE" && IDmode_ != "MEDIUM" && IDmode_ != "TIGHT" && IDmode_ != "highPTID") || IDmode_ == "Noid") {
          // cout << "Set to NO ID mode." << endl;
          // std::cout << "rho: " << rho_ << endl;
          pass_ID_version = 1;
          //return;
        }
        // pass_ID_version = 1;
        // if( pass_HighPtID ){
        if( pass_ID_version ){
          // cout << "PASS HPT Photon: " << "pt = " << pho->pt() << "; eta = " << pho->eta() << "; phi = " << pho->phi() << endl;
          // cout << endl;
          goodPhotons.push_back(pho);
          realAndFakePhotons.push_back(std::pair<edm::Ptr<pat::Photon>, int>(pho, TRUE));
        }
      }// end of photon loop

      sort(goodPhotons.begin(),goodPhotons.end(),ExoDiPhotons::comparePhotonsByPt);
      sort(realAndFakePhotons.begin(), realAndFakePhotons.end(), ExoDiPhotons::comparePhotonPairsByPt);

      if (goodPhotons.size() >= 3){
        isGood_ = true;
        // cout << "TRIPHOTON FOUND" << endl;
        photonFiller(goodPhotons, recHitsEB, recHitsEE, &id_decisions[0], photon1Info, photon2Info, photon3Info, diphotonInfo12, diphotonInfo13, diphotonInfo23, triphotonInfo);
        mcTruthFiller(&(*goodPhotons[0]), fPhoton1Info, genParticles);
        mcTruthFiller(&(*goodPhotons[1]), fPhoton2Info, genParticles);
        mcTruthFiller(&(*goodPhotons[2]), fPhoton3Info, genParticles);
      }
      if (goodPhotons.size() < 3){
        isGood_ = false;
        cout << "TRIPHOTON NOT FOUND in PAT for " << IDmode_ << endl;
      }
}//end of fillPhotonInfo

void nPhotonAnalyzer::photonFiller(const std::vector<edm::Ptr<pat::Photon>>& photons,
      				                         const edm::Handle<EcalRecHitCollection>& recHitsEB,
                                       const edm::Handle<EcalRecHitCollection>& recHitsEE,
      				                         const edm::Handle<edm::ValueMap<bool> >* id_decisions,
      				                         ExoDiPhotons::photonInfo_t& photon1Info,
                                       ExoDiPhotons::photonInfo_t& photon2Info,
                                       ExoDiPhotons::photonInfo_t& photon3Info,
                                       ExoDiPhotons::diphotonInfo_t& diphotonInfo12,
                                       ExoDiPhotons::diphotonInfo_t& diphotonInfo13,
                                       ExoDiPhotons::diphotonInfo_t& diphotonInfo23,
                                       ExoDiPhotons::triphotonInfo_t& triphotonInfo){

            // std::cout << "Photon 1 pt = " << photons[0]->pt() << "; eta = " << photons[0]->eta() << "; phi = " << photons[0]->phi() << std::endl;
            // std::cout << "Photon 2 pt = " << photons[1]->pt() << "; eta = " << photons[1]->eta() << "; phi = " << photons[1]->phi() << std::endl;
            // std::cout << "Photon 3 pt = " << photons[2]->pt() << "; eta = " << photons[2]->eta() << "; phi = " << photons[2]->phi() << std::endl;

            // Individual photon Information
            photon1Info.isSaturated = ExoDiPhotons::isSaturated(&(*photons[0]), &(*recHitsEB), &(*recHitsEE), &(*subDetTopologyEB_), &(*subDetTopologyEE_));
            // std::cout << "Photon 1 isSat: " << photon1Info.isSaturated << std::endl;
            ExoDiPhotons::FillBasicPhotonInfo(photon1Info, &(*photons[0]));
            ExoDiPhotons::FillPhotonIDInfo(photon1Info, &(*photons[0]), rho_, photon1Info.isSaturated);

            photon2Info.isSaturated = ExoDiPhotons::isSaturated(&(*photons[1]), &(*recHitsEB), &(*recHitsEE), &(*subDetTopologyEB_), &(*subDetTopologyEE_));
            // std::cout << "Photon 2 isSat: " << photon2Info.isSaturated << std::endl;
            ExoDiPhotons::FillBasicPhotonInfo(photon2Info, &(*photons[1]));
            ExoDiPhotons::FillPhotonIDInfo(photon2Info, &(*photons[1]), rho_, photon2Info.isSaturated);

            photon3Info.isSaturated = ExoDiPhotons::isSaturated(&(*photons[2]), &(*recHitsEB), &(*recHitsEE), &(*subDetTopologyEB_), &(*subDetTopologyEE_));
            // std::cout << "Photon 3 isSat: " << photon3Info.isSaturated << std::endl;
            ExoDiPhotons::FillBasicPhotonInfo(photon3Info, &(*photons[2]));
            ExoDiPhotons::FillPhotonIDInfo(photon3Info, &(*photons[2]), rho_, photon3Info.isSaturated);

            // EGM ID Info
            photon1Info.passEGMLooseID  = (*(id_decisions[LOOSE]))[photons[0]];
            photon1Info.passEGMMediumID = (*(id_decisions[MEDIUM]))[photons[0]];
            photon1Info.passEGMTightID  = (*(id_decisions[TIGHT]))[photons[0]];

            photon2Info.passEGMLooseID  = (*(id_decisions[LOOSE]))[photons[1]];
            photon2Info.passEGMMediumID = (*(id_decisions[MEDIUM]))[photons[1]];
            photon2Info.passEGMTightID  = (*(id_decisions[TIGHT]))[photons[1]];

            photon3Info.passEGMLooseID  = (*(id_decisions[LOOSE]))[photons[2]];
            photon3Info.passEGMMediumID = (*(id_decisions[MEDIUM]))[photons[2]];
            photon3Info.passEGMTightID  = (*(id_decisions[TIGHT]))[photons[2]];

            ExoDiPhotons::FillPhotonEGMidInfo(photon1Info, &(*photons[0]), rho_, effAreaChHadrons_, effAreaNeuHadrons_, effAreaPhotons_);
            ExoDiPhotons::FillPhotonEGMidInfo(photon2Info, &(*photons[1]), rho_, effAreaChHadrons_, effAreaNeuHadrons_, effAreaPhotons_);
            ExoDiPhotons::FillPhotonEGMidInfo(photon3Info, &(*photons[2]), rho_, effAreaChHadrons_, effAreaNeuHadrons_, effAreaPhotons_);

            // Fill Diphoton and Triphoton Information
            ExoDiPhotons::FillDiphotonInfo(diphotonInfo12,&(*photons[0]),&(*photons[1]));
            ExoDiPhotons::FillDiphotonInfo(diphotonInfo13,&(*photons[0]),&(*photons[2]));
            ExoDiPhotons::FillDiphotonInfo(diphotonInfo23,&(*photons[1]),&(*photons[2]));
            ExoDiPhotons::FillTriphotonInfo(triphotonInfo,&(*photons[0]),&(*photons[1]),&(*photons[2]));

}//end photonFiller

void nPhotonAnalyzer::mcTruthFiller (const pat::Photon *photon, ExoDiPhotons::photonInfo_t& photonInfo, const edm::Handle<edm::View<reco::GenParticle> > genParticles)
{
  bool printInfo = false;
  if (printInfo) std::cout << "Photon: pt = " << photon->pt() << "; eta = " << photon->eta() << "; phi = " << photon->phi() << std::endl;

  const double deltaR_cut = 0.1;
  double minDeltaR = deltaR_cut;
  const reco::GenParticle *photon_gen_match = NULL;

  // Find best gen-to-reco match in dR cone
  for (size_t i = 0; i < genParticles->size(); ++i){
    const auto gen = genParticles->ptrAt(i);
    double deltaR = reco::deltaR(photon->eta(), photon->phi(), gen->eta(), gen->phi());
    // status 1 Final State particle
    if (gen->status() == 1){
      if (printInfo && (gen->pdgId() == 22)) std::cout << "Gen. particle: status = " << gen->status() << "; pdgId = " << gen->pdgId()
		  << "; pt = " << gen->pt() << "; eta = " << gen->eta() << "; phi = " << gen->phi() << "; dR = " << deltaR << std::endl;
      if (deltaR <= minDeltaR){
        minDeltaR = deltaR;
        photon_gen_match = &(*gen);
      }
      }
    } // end genparticle loop

    bool is_fake = false;


    // if matched, look at gen. history to determine if reco photon is fake
    if (photon_gen_match){
      if (printInfo) std::cout << "Final state gen particle match: status = " << photon_gen_match->status() << "; pdgId = " << photon_gen_match->pdgId()
		<< "; pt = " << photon_gen_match->pt() << "; eta = " << photon_gen_match->eta() << "; phi = " << photon_gen_match->phi() << "; dR = " << minDeltaR << std::endl;

    // if matched to a photon, check mothers to determine if fake
    if (photon_gen_match->pdgId()==22){
      // FAKE if first non-photon mother does not come from hard interaction proton
      // I honestly don't like the name matchedMother for j = 0 since it's just the photon we're looking at
      const reco::GenParticle *matchedMother = &(*photon_gen_match);
      double minMotherMatchDeltaR = 1000000;
      int matchMotherIndex = 0;
      bool is_photon_mother = true;
      bool is_first_non_photon_mother = true;

      // loop through all mothers until we trace back to colliding proton
      while (matchedMother->mother()){
        for (unsigned int j=0; j < matchedMother->numberOfMothers(); j++){
          //calculated deltaR between particle and mother
          double deltaR = reco::deltaR(matchedMother->eta(),matchedMother->phi(),matchedMother->mother(j)->eta(),matchedMother->mother(j)->phi());
          // if current deltaR is smallest, save index
          if (deltaR < minMotherMatchDeltaR){
            minMotherMatchDeltaR = deltaR;
            matchMotherIndex = j;
          }
        }// mothers for loop
        // matched Particle is the mother with best deltaR
        matchedMother = (reco::GenParticle *) matchedMother->mother(matchMotherIndex);
        if (matchedMother->pdgId() != 22) is_photon_mother = false;
        if (!is_photon_mother && is_first_non_photon_mother && fabs(matchedMother->pdgId()) > 99 && matchedMother->pt() !=0.) {
          is_fake = true;
          is_first_non_photon_mother = false;
        }
        // reset
        minMotherMatchDeltaR = 1000000;
        matchMotherIndex = 0;
      } // mothers while loop
    }// if photon matched

    // if not matched to a photon, check match to determine if fake
    if (photon_gen_match->pdgId() != 22){
      if (fabs(photon_gen_match->pdgId()) > 99 && photon_gen_match->pt() != 0.) is_fake = true;
    }
    if (printInfo) std::cout << "Fake? " << is_fake << std::endl;

  }//if photon gen match

  if (is_fake) photonInfo.isMCTruthFake = true;
}// end mcTruthFiller

DEFINE_FWK_MODULE(nPhotonAnalyzer);
