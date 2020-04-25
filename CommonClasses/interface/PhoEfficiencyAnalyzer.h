#ifndef PhoEfficiencyAnalyzer_h
#define PhoEfficiencyAnalyzer_h

#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Essentials
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
//#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"
#include <vector>
#include "TLorentzVector.h"
#include "TH2D.h"
#include "TTree.h"

// for EGM ID
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"

// Miscellaneous
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

// Common Classes
#include "multiphoton-analysis/CommonClasses/interface/EventInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/GenParticleInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/DiPhotonInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/TriPhotonInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/PhotonInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/PhotonID.h"

// ECAL topology
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

using namespace std;
using namespace edm;


class PhoEfficiencyAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit PhoEfficiencyAnalyzer(const edm::ParameterSet&);
      ~PhoEfficiencyAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      // void fillInfo(const edm::Handle<edm::View<reco::GenParticle> > genParticles,
      //               const edm::Handle<edm::View<pat::Photon> >& photons);
      void fillInfo(const edm::Handle<edm::View<reco::GenParticle> > genParticles,
                       const edm::Handle<edm::View<pat::Photon> >& photons,
                       const edm::Handle<EcalRecHitCollection>& recHitsEB,
                       const edm::Handle<EcalRecHitCollection>& recHitsEE,
                       const edm::Handle<edm::ValueMap<bool> >* id_decisions);
      void fillgenPhoIDInfo( ExoDiPhotons::genParticleInfo_t &genParticleInfo,
                       const reco::GenParticle *genPho,
                       double minDeltaR );
      void fillpatPhoIDInfo( ExoDiPhotons::photonInfo_t& photonInfo, const pat::Photon *photon, edm::Ptr<pat::Photon> patMatch,
                             const edm::Handle<EcalRecHitCollection>& recHitsEB,
                             const edm::Handle<EcalRecHitCollection>& recHitsEE,
                             const edm::Handle<edm::ValueMap<bool> >* id_decisions  );
      // void fillpatPhoIDInfo(photonInfo_t &photonInfo, const pat::Photon *photon,
      //                       double rho, EffectiveAreas eaCH, EffectiveAreas eaNH, EffectiveAreas eaPho,
      //                       const edm::Handle<EcalRecHitCollection>& recHitsEB,
      //                       const edm::Handle<EcalRecHitCollection>& recHitsEE,
      //                       const edm::Handle<edm::ValueMap<bool> >* id_decisions)

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::Service<TFileService> fs;

      edm::EDGetTokenT<edm::View<reco::GenParticle> > genParticlesToken_;
      //edm::EDGetTokenT<vector<reco::GenParticle> >    genParticlesToken_;
      edm::EDGetTokenT<edm::View<reco::GenParticle> > genParticlesMiniAODToken_;
      edm::EDGetToken                                 photonsMiniAODToken_;
      edm::EDGetTokenT<double>                        rhoToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >          phoLooseIdMapToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >          phoMediumIdMapToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >          phoTightIdMapToken_;
      edm::EDGetTokenT<GenEventInfoProduct>           genInfoToken_;
      edm::EDGetTokenT<EcalRecHitCollection>          recHitsEBToken;
      edm::EDGetTokenT<EcalRecHitCollection>          recHitsEEToken;

      edm::InputTag genParticles_;
      edm::InputTag particles_;
      edm::InputTag recHitsEBTag_;
      edm::InputTag recHitsEETag_;


      enum {LOOSE = 0, MEDIUM = 1, TIGHT = 2};
      enum {FAKE = 0,  TRUE = 1};

      EffectiveAreas effAreaChHadrons_;
      EffectiveAreas effAreaNeuHadrons_;
      EffectiveAreas effAreaPhotons_;

      TTree *fgenTree;
      TTree *fTree;

      ExoDiPhotons::eventInfo_t         fEventInfo;
      ExoDiPhotons::genParticleInfo_t   fGenPhotonInfo; // Each entry is an in an individual photons not whole event
      int fGenPhotonNumber;

      ExoDiPhotons::photonInfo_t        fPhotonInfo;
      int fPhotonNumber; 



      //Put flags in cfg later
      int nPV_;
      bool isMC_;
      bool isClosureTest_;
      bool isGood_;
      bool islocal_;
      bool isDAS_;
      double rho_;
      double xsec_; // For local generation
      double WeightAll_;
      double GenPhoton0_iso_;
      double GenPhoton1_iso_;
      double GenPhoton2_iso_;
      uint32_t nEventsSample_;
      TString outputFile_;
      std::string IDmode_;  // VID (Loose, Medium, Tight) or hightPTID

      const CaloSubdetectorTopology* subDetTopologyEB_;
      const CaloSubdetectorTopology* subDetTopologyEE_;
};



#endif
