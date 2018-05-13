#ifndef ngNtuplizer_h
#define ngNtuplizer_h

//System include files
#include <memory>
#include <vector>

//Standard Headers 
#include "TTree.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
//#include "HiggsAnalysis/HiggsTo2photons/interface/CiCPhotonID.h"

//Common Classes
#include "multiphoton-analysis/CommonClasses/interface/EventInfo.h"


//ROOT
#include "TLorentzVector.h"
#include "TH2D.h"


using namespace std;

class MultiPhotonAnalyzer : public edm::EDAnalyzer  {
   public:

      explicit MultiPhotonAnalyzer(const edm::ParameterSet&);
      ~MultiPhotonAnalyzer();

     // static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      //virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override; 
      //virtual void endJob() override;

      // ----------member data ---------------------------
      //edm::Service<TFileService> fs; 

};




#endif
