#include "multiphoton-analysis/CommonClasses/interface/FakeAnalyzer.h"

using namespace std;
using namespace edm;

FakeAnalyzer::FakeAnalyzer(const edm::ParameterSet& ps)
   :effAreaChHadrons_( (ps.getParameter<edm::FileInPath>("effAreaChHadFile")).fullPath() ),
    effAreaNeuHadrons_( (ps.getParameter<edm::FileInPath>("effAreaNeuHadFile")).fullPath() ),
    effAreaPhotons_( (ps.getParameter<edm::FileInPath>("effAreaPhoFile")).fullPath() )
{
   usesResource("TFileService");
   edm::Service<TFileService> fs;

   rhoToken_                 = consumes<double>                      (ps.getParameter<edm::InputTag>("rho"));
   phoLooseIdMapToken_       = consumes<edm::ValueMap<bool> >         (ps.getParameter<edm::InputTag>("phoLooseIdMap"));
   phoMediumIdMapToken_      = consumes<edm::ValueMap<bool> >         (ps.getParameter<edm::InputTag>("phoMediumIdMap"));
   phoTightIdMapToken_       = consumes<edm::ValueMap<bool> >         (ps.getParameter<edm::InputTag>("phoTightIdMap"));
   outputFile_               =                                 TString(ps.getParameter<std::string>("outputFile"));
   isReMINIAOD_              =                                         ps.getParameter<bool>("isReMINIAOD_");

   fTree = fs->make<TTree>("fTree","PhotonTree");
   fTree->Branch("Event",&fEventInfo,ExoDiPhotons::eventBranchDefString.c_str());
   fTree->Branch("Jet",&fJetInfo,ExoDiPhotons::jetBranchDefString.c_str());
   fTree->Branch("TriggerBit",&fTriggerBitInfo,ExoDiPhotons::triggerBranchDefString.c_str());
   fTree->Branch("TriggerPrescale",&fTriggerPrescaleInfo,ExoDiPhotons::triggerBranchDefString.c_str());
   fTree->Branch("Photon",&fPhotonInfo,ExoDiPhotons::photonBranchDefString.c_str());
   fTree->Branch("VertexCollInfo.vx","std::vector<double>",&fVertexCollInfo.vx);
   fTree->Branch("VertexCollInfo.vy","std::vector<double>",&fVertexCollInfo.vy);
   fTree->Branch("VertexCollInfo.vz","std::vector<double>",&fVertexCollInfo.vz);
   fTree->Branch("VertexCollInfo.vxError","std::vector<double>",&fVertexCollInfo.vxError);
   fTree->Branch("VertexCollInfo.vyError","std::vector<double>",&fVertexCollInfo.vyError);
   fTree->Branch("VertexCollInfo.vzError","std::vector<double>",&fVertexCollInfo.vzError);
   fTree->Branch("VertexCollInfo.ndof","std::vector<double>",&fVertexCollInfo.ndof);
   fTree->Branch("VertexCollInfo.d0","std::vector<double>",&fVertexCollInfo.d0);
   fTree->Branch("VertexCollInfo.nTracks","std::vector<int>",&fVertexCollInfo.nTracks);
   fTree->Branch("VertexCollInfo.isFake","std::vector<int>",&fVertexCollInfo.isFake);

   photonsMiniAODToken_  = mayConsume<edm::View<pat::Photon> >(ps.getParameter<edm::InputTag>("photonsMiniAOD"));
   jetsMiniAODToken_     = mayConsume< edm::View<pat::Jet> >(ps.getParameter<edm::InputTag>("jetsMiniAOD"));
   jetPtThreshold        = ps.getParameter<double>("jetPtThreshold");
   jetEtaThreshold       = ps.getParameter<double>("jetEtaThreshold");
   recHitsEBTag_         = ps.getUntrackedParameter<edm::InputTag>("RecHitsEBTag",edm::InputTag("reducedEgamma:reducedEBRecHits"));
   recHitsEETag_         = ps.getUntrackedParameter<edm::InputTag>("RecHitsEETag",edm::InputTag("reducedEgamma:reducedEERecHits"));
   recHitsEBToken        = consumes <edm::SortedCollection<EcalRecHit> > (recHitsEBTag_);
   recHitsEEToken        = consumes <edm::SortedCollection<EcalRecHit> > (recHitsEETag_);
   beamHaloSummaryToken_ = consumes<reco::BeamHaloSummary>( edm::InputTag("BeamHaloSummary") );

   processNameData_ =  "RECO";

   if(isReMINIAOD_) {
   if(outputFile_.Contains("17Jul2018")) processNameData_ = "DQM";
   if(outputFile_.Contains("31Mar2018")) processNameData_ = "PAT";
   }

   filterDecisionToken_   = consumes<edm::TriggerResults>( edm::InputTag("TriggerResults","",isReMINIAOD_?(processNameData_):("RECO")) );
   triggerDecisionToken_  = consumes<edm::TriggerResults>( edm::InputTag("TriggerResults","","HLT") );
   prescalesToken_        = consumes<pat::PackedTriggerPrescales>( edm::InputTag("patTrigger","",isReMINIAOD_?(processNameData_):("RECO")) );
   verticesToken_         = consumes<reco::VertexCollection>( edm::InputTag("offlineSlimmedPrimaryVertices") );

}

FakeAnalyzer::~FakeAnalyzer()
{


}

// member functions

// ------------ method called for each event  ------------
void
FakeAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;
   using namespace pat;

   edm::Handle< reco::BeamHaloSummary > bhsHandle;
   edm::Handle< edm::View<pat::Jet> > jets;
   edm::Handle< double > rhoH;
   edm::Handle<edm::ValueMap<bool> > loose_id_decisions;
   edm::Handle<edm::ValueMap<bool> > medium_id_decisions;
   edm::Handle<edm::ValueMap<bool> > tight_id_decisions;
   edm::Handle<EcalRecHitCollection> recHitsEB;
   edm::Handle<EcalRecHitCollection> recHitsEE;
   edm::Handle<edm::TriggerResults> filtHandle;
   edm::Handle<edm::TriggerResults> triggerHandle;
   edm::Handle<pat::PackedTriggerPrescales> prescalesHandle;
   edm::Handle<reco::VertexCollection> vertices;

   iEvent.getByToken(phoMediumIdMapToken_,medium_id_decisions);
   iEvent.getByToken(beamHaloSummaryToken_,bhsHandle);
   iEvent.getByToken(jetsMiniAODToken_,jets);
   iEvent.getByToken(rhoToken_,rhoH);
   iEvent.getByToken(phoLooseIdMapToken_ ,loose_id_decisions);
   iEvent.getByToken(phoMediumIdMapToken_,medium_id_decisions);
   iEvent.getByToken(phoTightIdMapToken_ ,tight_id_decisions);
   iEvent.getByToken(recHitsEBToken,recHitsEB);
   iEvent.getByToken(recHitsEEToken,recHitsEE);
   iEvent.getByToken(filterDecisionToken_,filtHandle);
   iEvent.getByToken(triggerDecisionToken_,triggerHandle);
   iEvent.getByToken(prescalesToken_,prescalesHandle);
   iEvent.getByToken(verticesToken_,vertices);

   const reco::BeamHaloSummary* bhs = &(*bhsHandle);

   ExoDiPhotons::InitEventInfo(fEventInfo);
   ExoDiPhotons::InitJetInfo(fJetInfo);

   ExoDiPhotons::FillBasicEventInfo(fEventInfo, iEvent);
   ExoDiPhotons::FillBeamHaloEventInfo(fEventInfo, bhs);
   ExoDiPhotons::FillJetInfo(fJetInfo, &(*jets), jetPtThreshold, jetEtaThreshold);

   const CaloSubdetectorTopology* subDetTopologyEB_;
   const CaloSubdetectorTopology* subDetTopologyEE_;
   edm::ESHandle<CaloTopology> caloTopology;
   iSetup.get<CaloTopologyRecord>().get(caloTopology);
   subDetTopologyEB_ = caloTopology->getSubdetectorTopology(DetId::Ecal,EcalBarrel);
   subDetTopologyEE_ = caloTopology->getSubdetectorTopology(DetId::Ecal,EcalEndcap);

   rho_ = *rhoH;

   // =====================
   // FILTER DECISION INFO
   // =====================

   const edm::TriggerResults *filterResults = filtHandle.product();
   std::vector<std::string> trigNames = iEvent.triggerNames(*filterResults).triggerNames();

   int EEBadScFilterNum = -1;
   for (unsigned int i=0; i < trigNames.size(); i++){
     std::string tempName = trigNames.at(i);
     if (tempName == "Flag_eeBadScFilter"){
       EEBadScFilterNum = i;
       break;
     }
   }

   if (EEBadScFilterNum < 0) throw cms::Exception("FilterDecisionInfo") << "Cannot find Flag_eeBadScFilter in the filter decision object!! This must be investigated!!!";
   bool passEEBadScFilter = filterResults->accept(EEBadScFilterNum);

   // Go to next photon if this filter is not passed
  if (!passEEBadScFilter){
    cout << "Event did not pass the EE Bad Supercrystal Filter, skip it!" << endl;
    return;
  }

  // =====================
  // TRIGGER DECISION INFO
  // =====================

  const edm::TriggerResults* triggerResults = triggerHandle.product();
  const pat::PackedTriggerPrescales* prescales = prescalesHandle.product();

  ExoDiPhotons::FillTriggerBits(fTriggerBitInfo, triggerResults, iEvent); // fill trigger bits into trigger bit branch
  ExoDiPhotons::FillTriggerPrescales(fTriggerPrescaleInfo, triggerResults, prescales, iEvent); // fill trigger prescale info in trigger prescale branch

  // ========
  // VERTICES
  // ========

  const reco::VertexCollection* vtxColl = vertices.product();
  ExoDiPhotons::FillVertexCollInfo(fVertexCollInfo,vtxColl);

  // =======
  // PHOTONS
  // =======

  ExoDiPhotons::InitPhotonInfo(fPhotonInfo);

  edm::Handle<edm::View<pat::Photon> > photons;
  iEvent.getByToken(photonsMiniAODToken_,photons);

  for (size_t i = 0; i < photons->size(); ++i){
       const auto pho = photons->ptrAt(i);
       cout << "Photon: " << "pt = " << pho->pt() << "; eta = " << pho->eta() << "; phi = " << pho->phi() << endl;

       fPhotonInfo.isSaturated = ExoDiPhotons::isSaturated(&(*pho), &(*recHitsEB), &(*recHitsEE), &(*subDetTopologyEB_), &(*subDetTopologyEE_));
       cout << "isSat: " << fPhotonInfo.isSaturated << endl;

       ExoDiPhotons::FillBasicPhotonInfo(fPhotonInfo, &(*pho));
       ExoDiPhotons::FillPhotonIDInfo(fPhotonInfo, &(*pho), rho_, fPhotonInfo.isSaturated);
       ExoDiPhotons::FillPhotonEGMidInfo(fPhotonInfo, &(*pho), rho_, effAreaChHadrons_, effAreaNeuHadrons_, effAreaPhotons_);
       fPhotonInfo.passEGMLooseID  = true;
       fPhotonInfo.passEGMMediumID = true;
       fPhotonInfo.passEGMTightID  = true;

       // fill our tree
       if ( ExoDiPhotons::passNumeratorCandCut(&(*pho), rho_) ||
         ExoDiPhotons::passDenominatorCut(&(*pho), rho_, fPhotonInfo.isSaturated)
       ) fTree->Fill();

  } //end photon loop 

  cout << "rho: " << rho_ << endl;
  cout <<  "Run: " << iEvent.id().run() << ", LS: " <<  iEvent.id().luminosityBlock() << ", Event: " << iEvent.id().event() << endl;

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

DEFINE_FWK_MODULE(FakeAnalyzer);
