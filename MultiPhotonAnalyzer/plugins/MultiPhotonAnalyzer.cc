#include "multiphoton-analysis/CommonClasses/interface/ngNtuplizer.h"

using namespace std;
using namespace std;

MultiPhotonAnalyzer::MultiPhotonAnalyzer(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   //usesResource("TFileService");
  
}


MultiPhotonAnalyzer::~MultiPhotonAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MultiPhotonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
  
}


// ------------ method called once each job just before starting event loop  ------------
//void 
//MultiPhotonAnalyzer::beginJob()
//{
//}
//
//// ------------ method called once each job just after ending the event loop  ------------
//void 
//MultiPhotonAnalyzer::endJob() 
//{
//}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
//void
//MultiPhotonAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
//  //The following says we do not know what parameters are allowed so do no validation
//  // Please change this to state exactly what you do use, even if it is no parameters
//  edm::ParameterSetDescription desc;
//  desc.setUnknown();
//  descriptions.addDefault(desc);
//}

//define this as a plug-in
DEFINE_FWK_MODULE(MultiPhotonAnalyzer);
