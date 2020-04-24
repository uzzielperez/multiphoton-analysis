#include "multiphoton-analysis/CommonClasses/interface/PhoEfficiencyAnalyzer.h"

using namespace std;
using namespace edm;

PhoEfficiencyAnalyzer::PhoEfficiencyAnalyzer(const edm::ParameterSet& ps)
   :effAreaChHadrons_( (ps.getParameter<edm::FileInPath>("effAreaChHadFile")).fullPath() ),
    effAreaNeuHadrons_( (ps.getParameter<edm::FileInPath>("effAreaNeuHadFile")).fullPath() ),
    effAreaPhotons_( (ps.getParameter<edm::FileInPath>("effAreaPhoFile")).fullPath() )
{
   usesResource("TFileService");
   isGood_           = false;
   nPV_                 = 0;
   GenPhoton0_iso_      = 9999.99;
   GenPhoton1_iso_      = 9999.99;

   genParticlesToken_        = consumes<edm::View<reco::GenParticle> > (ps.getParameter<InputTag>("genparticles"));
   rhoToken_                 = consumes<double>                      (ps.getParameter<edm::InputTag>("rho"));
   phoLooseIdMapToken_       = consumes<edm::ValueMap<bool> >         (ps.getParameter<edm::InputTag>("phoLooseIdMap"));
   phoMediumIdMapToken_      = consumes<edm::ValueMap<bool> >         (ps.getParameter<edm::InputTag>("phoMediumIdMap"));
   phoTightIdMapToken_       = consumes<edm::ValueMap<bool> >         (ps.getParameter<edm::InputTag>("phoTightIdMap"));
   nEventsSample_            =                                        (ps.getParameter<uint32_t>("nEventsSample"));
   genInfoToken_             = consumes<GenEventInfoProduct>          (ps.getParameter<edm::InputTag>("genInfo"));
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

   fTree = fs->make<TTree>("fTree", "DiphotonTree");
   fTree->Branch("Event",         &fEventInfo,       ExoDiPhotons::eventBranchDefString.c_str());
   fTree->Branch("GenPhoton",     &fGenPhotonInfo,   ExoDiPhotons::genParticleBranchDefString.c_str());
   fTree->Branch("GenPhotonNum",  &fGenPhotonNumber, "num/I");
   fTree->Branch("Photon",        &fPhotonInfo,      ExoDiPhotons::photonBranchDefString.c_str());
   fTree->Branch("PhotonNum",     &fPhotonNumber, "num/I");
   fTree->Branch("isGood",           &isGood_);
   fTree->Branch("nPV", &nPV_);
   // For
}

PhoEfficiencyAnalyzer::~PhoEfficiencyAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

// member functions

// ------------ method called for each event  ------------
void
PhoEfficiencyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;
   using namespace pat;

   //---Handle, getByToken
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
   ExoDiPhotons::InitGenParticleInfo(fGenPhotonInfo);
   ExoDiPhotons::InitPhotonInfo(fPhotonInfo);
   fGenPhotonNumber = -1;
   fPhotonNumber    = -1;


   //---Update
   ExoDiPhotons::FillBasicEventInfo(fEventInfo, iEvent);
   ExoDiPhotons::FillGenEventInfo(fEventInfo, &(*genInfo));
   ExoDiPhotons::FillEventWeights(fEventInfo, outputFile_, nEventsSample_);
   fillInfo(genParticles, photons, recHitsEB, recHitsEE, &id_decisions[0]);
   //fTree->Fill(); // put this in fillInfo to do it per photon

}


// ------------ method called once each job just before starting event loop  ------------
void
PhoEfficiencyAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
PhoEfficiencyAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PhoEfficiencyAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


void PhoEfficiencyAnalyzer::fillInfo(const edm::Handle<edm::View<reco::GenParticle> > genParticles,
                                     const edm::Handle<edm::View<pat::Photon> >& photons,
                                     const edm::Handle<EcalRecHitCollection>& recHitsEB,
                                     const edm::Handle<EcalRecHitCollection>& recHitsEE,
                                     const edm::Handle<edm::ValueMap<bool> >* id_decisions)
      {

      bool debug = true;
      // Store Information in these vectors
      vector< edm::Ptr<const reco::GenParticle> > genPhotons;
      std::vector<edm::Ptr<pat::Photon>> patPhotons;
      vector<int> interactingPartons;

      // Store photons from genParticles
      for (size_t i = 0; i < genParticles->size(); ++i){
       const auto gen = genParticles->ptrAt(i);
       //Pythia8 status 20-30 is Hard interaction
       if (gen->isHardProcess() && gen->pt() == 0)    interactingPartons.push_back(gen->pdgId());
       if (gen->isHardProcess() && gen->pdgId()==22){
         genPhotons.push_back(gen);
       }
      }

     // Store PAT photons in vector
     for (size_t i = 0; i < photons->size(); ++i){
       const auto pho = photons->ptrAt(i);
       patPhotons.push_back(pho);
     }

      sort(genPhotons.begin(), genPhotons.end(), ExoDiPhotons::comparePhotonsByPt);
      sort(patPhotons.begin(),patPhotons.end(), ExoDiPhotons::comparePhotonsByPt);

      if(interactingPartons.size() == 2){
        fEventInfo.interactingParton1PdgId = interactingPartons[0];
        fEventInfo.interactingParton2PdgId = interactingPartons[1];
      }

      else cout << "Exactly two interacting partons not found!" << endl;

      //---- Photon Information
      // Now show go through the sorted pat and gen photons
      const reco::GenParticle *genPho;
      const pat::Photon *patPho;
      const pat::Photon *photon_reco_match = NULL;
      const reco::GenParticle *photon_gen_match = NULL;
      edm::Ptr<pat::Photon> patMatch;
      int pat_i = -1;

      if (debug) std::cout << patPhotons.size() << std::endl;

      //------------------------------ DeltaR matching
      // Find PAT deltaR match for each Gen photon
      // Store Information for each DR match in Tree


      for (std::vector<int>::size_type i = 0; i != genPhotons.size(); i++)
      {
        if ( genPhotons.size() < 1) return;
        genPho = &(*genPhotons.at(i));
        double minDeltaR = 99999.99;

        for ( std::vector<int>::size_type j = 0; j != patPhotons.size(); j++ )
        {
            patPho = &(*patPhotons.at(j));
            const auto pho = patPhotons.at(j);
            double deltaR  = reco::deltaR(genPho->eta(), genPho->phi(), patPho->eta(), patPho->phi());

            if ( deltaR <= minDeltaR )
            {
                minDeltaR = deltaR;
                if ( minDeltaR < 0.5 )
                {
                  photon_reco_match = &(*patPho);
                  photon_gen_match  = &(*genPho);
                  patMatch = pho;
                  pat_i = j;
                }

            }
        } // patPho loop
        if ( minDeltaR < 0.5 && debug ) cout << "MATCH FOUND! minDR: " << minDeltaR
                                    << "; pt: "   << photon_gen_match->pt()  << " : " << photon_reco_match->pt()
                                    << "; eta = " << photon_gen_match->eta() << " : " << photon_reco_match->eta()
                                    << "; phi = " << photon_gen_match->phi() << " : " << photon_reco_match->phi() << std::endl;

        // Update structs information
        fillgenPhoIDInfo( fGenPhotonInfo, genPho, minDeltaR );
        fGenPhotonNumber = i + 1;

        if ( minDeltaR < 0.5 ){
          fillpatPhoIDInfo( fPhotonInfo, photon_reco_match, patMatch, recHitsEB, recHitsEE, &id_decisions[0] );
          fPhotonNumber = pat_i + 1;
        }

        fTree->Fill(); // filling per Gen Photon not just per event
      } // genPho loop

      //--------------------------- End DeltaR matching
}//end of fillInfo

void PhoEfficiencyAnalyzer::fillgenPhoIDInfo( ExoDiPhotons::genParticleInfo_t &genParticleInfo,
                                              const reco::GenParticle *genPho,
                                              double minDeltaR)
{

  ExoDiPhotons::FillGenParticleInfo(genParticleInfo, genPho);
  genParticleInfo.minDeltaR = minDeltaR;

}

void PhoEfficiencyAnalyzer::fillpatPhoIDInfo( ExoDiPhotons::photonInfo_t& photonInfo, const pat::Photon *photon, edm::Ptr<pat::Photon> patMatch,
                                              const edm::Handle<EcalRecHitCollection>& recHitsEB,
                                              const edm::Handle<EcalRecHitCollection>& recHitsEE,
                                              const edm::Handle<edm::ValueMap<bool> >* id_decisions)
{

  photonInfo.passEGMLooseID  = (*(id_decisions[LOOSE]))[patMatch];
  photonInfo.passEGMMediumID = (*(id_decisions[MEDIUM]))[patMatch];
  photonInfo.passEGMTightID  = (*(id_decisions[TIGHT]))[patMatch];
  photonInfo.isSaturated = ExoDiPhotons::isSaturated(photon, &(*recHitsEB), &(*recHitsEE), &(*subDetTopologyEB_), &(*subDetTopologyEE_));
  ExoDiPhotons::FillBasicPhotonInfo(photonInfo, photon);
  ExoDiPhotons::FillPhotonIDInfo(photonInfo, photon, rho_, photonInfo.isSaturated);
  ExoDiPhotons::FillPhotonEGMidInfo(photonInfo, photon, rho_, effAreaChHadrons_, effAreaNeuHadrons_, effAreaPhotons_);

  bool debug = true;
  if (debug) std::cout << "LOOSE: " << photonInfo.passEGMLooseID << std::endl;

}




DEFINE_FWK_MODULE(PhoEfficiencyAnalyzer);
