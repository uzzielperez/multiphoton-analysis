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

// Miscellaneous
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

// Common Classes
#include "multiphoton-analysis/CommonClasses/interface/EventInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/GenParticleInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/DiPhotonInfo.h"


using namespace std;
using namespace edm;


class nPhotonAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit nPhotonAnalyzer(const edm::ParameterSet&);
      ~nPhotonAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      void fillGenInfo(const edm::Handle<edm::View<reco::GenParticle> > genParticles);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::Service<TFileService> fs;

      edm::EDGetTokenT<edm::View<reco::GenParticle> > genParticlesToken_;
      //edm::EDGetTokenT<vector<reco::GenParticle> >    genParticlesToken_;
      edm::EDGetTokenT<edm::View<reco::GenParticle> > genParticlesMiniAODToken_;
      edm::EDGetToken                                 photonsMiniAODToken_;
      //edm::EDGetTokenT<double>                        rhoToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >          phoLooseIdMapToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >          phoMediumIdMapToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >          phoTightIdMapToken_;
      edm::EDGetTokenT<GenEventInfoProduct>           genInfoToken_;

      edm::InputTag genParticles_;
      edm::InputTag particles_;

      enum {LOOSE = 0, MEDIUM = 1, TIGHT = 2};
      enum {FAKE = 0,  TRUE = 1};

      //EffectiveAreas effAreaChHadrons_;
      //EffectiveAreas effAreaNeuHadrons_;
      //EffectiveAreas effAreaPhotons_;

      TTree *fgenTree;
      TTree *fSherpaGenTree;

      ExoDiPhotons::eventInfo_t         fEventInfo;
      ExoDiPhotons::genParticleInfo_t   fGenPhoton1Info;
      ExoDiPhotons::genParticleInfo_t   fGenPhoton2Info;
      ExoDiPhotons::diphotonInfo_t      fGenDiphotonInfo;
      ExoDiPhotons::genParticleInfo_t   fSherpaGenPhoton1Info;
      ExoDiPhotons::genParticleInfo_t   fSherpaGenPhoton2Info;
      ExoDiPhotons::diphotonInfo_t      fSherpaGenDiphotonInfo;

      //Put flags in cfg later
      int nPV_;
      //int rho_;
      bool isMC_;
      bool isGood_;
      bool isPythia8gen_;
      bool isSherpaDiphoton_;
      //double WeightAll_;
      double SherpaWeightAll_;
      double SherpaGenPhoton0_iso_;
      double SherpaGenPhoton1_iso_;
      uint32_t nEventsSample_;
      TString outputFile_;

};



#endif
