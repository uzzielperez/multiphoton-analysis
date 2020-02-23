#ifndef nPhotonAnalyzer_h
#define nPhotonAnalyzer_h

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


class nPhotonAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit nPhotonAnalyzer(const edm::ParameterSet&);
      ~nPhotonAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      void fillGenInfo(const edm::Handle<edm::View<reco::GenParticle> > genParticles,
                       const edm::Handle<edm::View<pat::Photon> >& photons);
      void fillPhotonInfo(const edm::Handle<edm::View<reco::GenParticle> > genParticles,
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
                    ExoDiPhotons::triphotonInfo_t& triphotonInfo);
      void photonFiller(const std::vector<edm::Ptr<pat::Photon>>& photons,
                    const edm::Handle<EcalRecHitCollection>& recHitsEB,
                    const edm::Handle<EcalRecHitCollection>& recHitsEE,
                		const edm::Handle<edm::ValueMap<bool> >* id_decisions,
                    ExoDiPhotons::photonInfo_t& photon1Info,
                    ExoDiPhotons::photonInfo_t& photon2Info,
                    ExoDiPhotons::photonInfo_t& photon3Info,
                    ExoDiPhotons::diphotonInfo_t& diphotonInfo12,
                    ExoDiPhotons::diphotonInfo_t& diphotonInfo13,
                    ExoDiPhotons::diphotonInfo_t& diphotonInfo23,
                    ExoDiPhotons::triphotonInfo_t& triphotonInfo);
      void mcTruthFiller(const pat::Photon *photon,
                    ExoDiPhotons::photonInfo_t& photonInfo,
                    const edm::Handle<edm::View<reco::GenParticle> > genParticles);
      void genRecoMatchInfo(const edm::Handle<edm::View<reco::GenParticle> > genParticles,
                            const edm::Handle<edm::View<pat::Photon> >& photons);


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
      ExoDiPhotons::genParticleInfo_t   fGenPhoton1Info;
      ExoDiPhotons::genParticleInfo_t   fGenPhoton2Info;
      ExoDiPhotons::genParticleInfo_t   fGenPhoton3Info;
      ExoDiPhotons::diphotonInfo_t      fGenDiphotonInfo12;
      ExoDiPhotons::diphotonInfo_t      fGenDiphotonInfo13;
      ExoDiPhotons::diphotonInfo_t      fGenDiphotonInfo23;
      ExoDiPhotons::triphotonInfo_t     fGenTriphotonInfo;

      ExoDiPhotons::genParticleInfo_t   fGenMatchPhoton1Info;
      ExoDiPhotons::genParticleInfo_t   fGenMatchPhoton2Info;
      ExoDiPhotons::genParticleInfo_t   fGenMatchPhoton3Info;
      ExoDiPhotons::genParticleInfo_t   fGenMatchPhoton4Info;
      ExoDiPhotons::genParticleInfo_t   fGenMatchPhoton5Info;
  

      ExoDiPhotons::photonInfo_t        fPhoton1Info;
      ExoDiPhotons::photonInfo_t        fPhoton2Info;
      ExoDiPhotons::photonInfo_t        fPhoton3Info;
      ExoDiPhotons::diphotonInfo_t      fDiphotonInfo12;
      ExoDiPhotons::diphotonInfo_t      fDiphotonInfo13;
      ExoDiPhotons::diphotonInfo_t      fDiphotonInfo23;
      ExoDiPhotons::triphotonInfo_t     fTriphotonInfo;

      // ExoDiPhotons::genParticleInfo_t   fSherpaGenPhoton1Info;
      // ExoDiPhotons::genParticleInfo_t   fSherpaGenPhoton2Info;
      // ExoDiPhotons::diphotonInfo_t      fSherpaGenDiphotonInfo;

      //Put flags in cfg later
      int nPV_;
      bool isMC_;
      bool isClosureTest_;
      bool isGood_;
      bool is3GenRecoed_;
      bool isgenPho1_recoed_;
      bool isgenPho2_recoed_;
      bool isgenPho3_recoed_;
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
