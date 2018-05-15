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

//   //---Go through Collection
//   int photoncount = 0; 
//   for(vector<reco::GenParticle>::const_iterator ip = genParticles->begin(); ip != genParticles->end(); ++ip){
//      if(ip->status()==1 && ip->pdgId()==22){
//         //cout << "Photon end state found" << endl;
//        photoncount = photoncount + 1;
//        double pt = ip->pt();
//        double eta = ip->eta();
//        double phi = ip->phi();
//        //double E = ip->energy();
//
//        //Ordering photons
//        if (pt > fGenPhoton1Info.pt){
//            fGenPhoton2Info.pt = fGenPhoton1Info.pt;
//            fGenPhoton2Info.eta = fGenPhoton1Info.eta;
//            fGenPhoton2Info.phi = fGenPhoton1Info.phi;
//            
//            fGenPhoton1Info.pt = pt;
//            fGenPhoton1Info.eta = eta;
//            fGenPhoton1Info.phi = phi;
//        }      
//        if ((pt < fGenPhoton1Info.pt) && (pt > fGenPhoton2Info.pt)){
//            fGenPhoton2Info.pt = pt;
//            fGenPhoton2Info.eta = eta;
//            fGenPhoton2Info.phi = phi;
//        }
//      }//end photon end state condition
//  }//end loop over gen particles 
//  cout << "Number of photons in event: " << photoncount << endl;
//  
   //structs container 
   vector<ExoDiPhotons::genParticleInfo_t> fGenStructsInfo;
   fGenStructsInfo.push_back(fGenPhoton1Info); 
   fGenStructsInfo.push_back(fGenPhoton2Info); 
  
   ExoDiPhotons::fillGenInfo(fGenStructsInfo,  genParticles);
   //ExoDiPhotons::fillGenInfo(fGenStructsInfo,  genParticles);  
   //ExoDiPhotons::fillGenInfo(fGenPhoton1Info,  genParticles);        
   //ExoDiPhotons::fillGenInfo(fGenPhoton2Info,  genParticles);

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
