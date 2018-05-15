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

// Common Classes
#include "multiphoton-analysis/CommonClasses/interface/GenParticleInfo.h"

using namespace std;
using namespace edm;


class nGenPhotonAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit nGenPhotonAnalyzer(const edm::ParameterSet&);
      ~nGenPhotonAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      edm::Service<TFileService> fs; 
      edm::EDGetTokenT<vector<reco::GenParticle> > genParticlesToken_;
      edm::InputTag genParticles_;
      edm::InputTag particles_;
      
      //numPhotons = 2; //find a way to set this at the configuration file. For now, store at least 2 photons 

      TTree *fgenTree;

      bool isPythia8gen_; 

      ExoDiPhotons::genParticleInfo_t fGenPhoton1Info; 
      ExoDiPhotons::genParticleInfo_t fGenPhoton2Info;
      //ExoDiPhotons::genParticleInfo_t fGenNPhotonInfo[numPhotons-1];
      
};



#endif
