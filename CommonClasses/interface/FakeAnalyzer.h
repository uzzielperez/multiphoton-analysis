#ifndef FakeAnalyzer_h
#define FakeAnalyzer_h

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


class FakeAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit FakeAnalyzer(const edm::ParameterSet&);
      ~FakeAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::EDGetToken                          photonsMiniAODToken_;
      edm::EDGetToken                          jetsMiniAODToken_;
      edm::EDGetToken                          beamHaloSummaryToken_;
      edm::EDGetTokenT<EcalRecHitCollection>   recHitsEBToken;
      edm::EDGetTokenT<EcalRecHitCollection>   recHitsEEToken;
      edm::EDGetTokenT<double>                 rhoToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> > phoLooseIdMapToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> > phoMediumIdMapToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> > phoTightIdMapToken_;

      edm::EDGetToken filterDecisionToken_;
      edm::EDGetToken triggerDecisionToken_;
      edm::EDGetToken prescalesToken_;
      edm::EDGetTokenT<reco::VertexCollection> verticesToken_;

      edm::InputTag recHitsEBTag_;
      edm::InputTag recHitsEETag_;

      ExoDiPhotons::photonInfo_t fPhotonInfo;
      ExoDiPhotons::jetInfo_t fJetInfo;
      ExoDiPhotons::eventInfo_t fEventInfo;
      ExoDiPhotons::triggerInfo_t fTriggerBitInfo;
      ExoDiPhotons::triggerInfo_t fTriggerPrescaleInfo;
      ExoDiPhotons::vertexCollInfo_t fVertexCollInfo;

      TString outputFile_;
      double rho_;
      double jetPtThreshold;
      double jetEtaThreshold;

      enum {LOOSE = 0, MEDIUM = 1, TIGHT = 2};
      enum {FAKE = 0,  TRUE = 1};

      EffectiveAreas effAreaChHadrons_;
      EffectiveAreas effAreaNeuHadrons_;
      EffectiveAreas effAreaPhotons_;
      bool isReMINIAOD_;
      std::string processNameData_;

      TTree *fTree;

};


#endif
