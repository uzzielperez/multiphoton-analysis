#include "multiphoton-analysis/CommonClasses/interface/nPhotonAnalyzer.h"

using namespace std;
using namespace edm;

nPhotonAnalyzer::nPhotonAnalyzer(const edm::ParameterSet& ps)

{
   usesResource("TFileService");

   fgenTree = fs->make<TTree>("fgenTree","GENDiphotonTree");

   fgenTree->Branch("GenPhoton1",  &fGenPhoton1Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenPhoton2",  &fGenPhoton2Info,  ExoDiPhotons::genParticleBranchDefString.c_str());
   fgenTree->Branch("GenDiPhoton", &fGenDiPhotonInfo, ExoDiPhotons::diphotonBranchDefString.c_str());

   //genParticlesToken_  = consumes<vector<reco::GenParticle>>    (ps.getParameter<InputTag>("genparticles"));
   genParticlesToken_  = consumes<edm::View<reco::GenParticle> >   (ps.getParameter<InputTag>("genparticles"));

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
   ExoDiPhotons::InitGenParticleInfo( fGenPhoton1Info );
   ExoDiPhotons::InitGenParticleInfo( fGenPhoton2Info );
   ExoDiPhotons::InitDiphotonInfo(    fGenDiPhotonInfo );

   //---Handle, getByToken
   //edm::Handle<vector<reco::GenParticle> > genParticles;
   edm::Handle<edm::View<reco::GenParticle> > genParticles;
   iEvent.getByToken(genParticlesToken_, genParticles);

   //Test
   const reco::GenParticle *genPho1 = NULL;
   const reco::GenParticle *genPho2 = NULL;

   for (size_t i = 0; i < genParticles->size(); ++i){
     const auto gen = genParticles->ptrAt(i);
     //print All Hard Interaction gen genParticles
     //is Status 3 == Hard Interaction
     cout << "HardProcess GenParticle: " << gen->isHardProcess() << endl;
     if (gen->status()==3) cout << "Status 3 genparticle: status " << gen->status() << "; pdgId: " << gen->pdgId()
                                << "; pt: " << gen->pt() << "; eta: " << gen->eta() << "; phi: " << gen->phi() << endl;

     //Print all finalstate photons
     //Is status 1 == final state in pythia?
     if (gen->status()==1 && gen->pdgId()==22){
       cout << "Status 1 photon: status " << gen->status() << "; pdgId: " << gen->pdgId()
                                               << "; pt: " << gen->pt() << "; eta: " << gen->eta() << "; phi: " << gen->phi() << endl;
        genPho1 = &(*gen);
        genPho2 = &(*gen);
     }//end 1, photon

   }//end for loop over gen particles

   cout << "GenPhoton1 Info: status " << genPho1->status() << "; pdgId: " << genPho1->pdgId()
                                           << "; pt: " << genPho1->pt() << "; eta: " << genPho1->eta() << "; phi: " << genPho1->phi() << endl;
   cout << "GenPhoton2 Info: status " << genPho2->status() << "; pdgId: " << genPho2->pdgId()
                                           << "; pt: " << genPho2->pt() << "; eta: " << genPho2->eta() << "; phi: " << genPho2->phi() << endl;

   // end test


   //---Update Information
   //ExoDiPhotons::fillGenDiPhoInfo(fGenPhoton1Info, fGenPhoton2Info, fGenDiPhotonInfo, genParticles);

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
