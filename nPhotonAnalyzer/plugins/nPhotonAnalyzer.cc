#include "multiphoton-analysis/CommonClasses/interface/nPhotonAnalyzer.h"

using namespace std;
using namespace edm;

nPhotonAnalyzer::nPhotonAnalyzer(const edm::ParameterSet& ps)

{
   usesResource("TFileService");
   
   fgenTree = fs->make<TTree>("fgenTree","GENDiphotonTree");
   
   fgenTree->Branch("GenPhoton1", &fGenPhoton1Info, ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenPhoton2", &fGenPhoton2Info, ExoDiPhotons::genParticleBranchDefString.c_str());
    
   genParticlesToken_  = consumes<vector<reco::GenParticle>>    (ps.getParameter<InputTag>("genparticles")); 
   

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
   
   //---Initialize
   ExoDiPhotons::InitGenParticleInfo(fGenPhoton1Info);
   ExoDiPhotons::InitGenParticleInfo(fGenPhoton2Info);
  
   //---Handle, getByToken
   edm::Handle<vector<reco::GenParticle> > genParticles;
   iEvent.getByToken(genParticlesToken_, genParticles);

   //structs container 
   vector<ExoDiPhotons::genParticleInfo_t> fGenStructsInfo;
   fGenStructsInfo.push_back(fGenPhoton1Info); 
   fGenStructsInfo.push_back(fGenPhoton2Info); 
  
   ExoDiPhotons::fillGenInfo(fGenStructsInfo,  genParticles);
   fGenPhoton1Info = fGenStructsInfo[0];
   fGenPhoton2Info = fGenStructsInfo[1];
   
   //Fill
   fgenTree->Fill(); 

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
DEFINE_FWK_MODULE(nPhotonAnalyzer);
