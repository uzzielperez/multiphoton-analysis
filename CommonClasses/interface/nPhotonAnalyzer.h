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
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include <vector>
#include "TLorentzVector.h"
#include "TH2D.h"
#include "TTree.h"

// Miscellaneous
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

// Common Classes
#include "multiphoton-analysis/CommonClasses/interface/GenParticleInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/DiPhotonInfo.h"

using namespace std;
using namespace edm;


class nPhotonAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit nPhotonAnalyzer(const edm::ParameterSet&);
      ~nPhotonAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::Service<TFileService> fs;
      edm::EDGetTokenT<vector<reco::GenParticle> > genParticlesToken_;
      edm::InputTag genParticles_;
      edm::InputTag particles_;

      TTree *fgenTree;

      bool isPythia8gen_;

      ExoDiPhotons::genParticleInfo_t   fGenPhoton1Info;
      ExoDiPhotons::genParticleInfo_t   fGenPhoton2Info;
      ExoDiPhotons::diphotonInfo_t      fGenDiPhotonInfo;
      //Working with Atom
};



#endif
