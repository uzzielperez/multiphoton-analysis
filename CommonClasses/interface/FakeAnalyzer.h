#ifndef FakeAnalyzer_h
#define FakeAnalyzer_h

#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Common Classes
#include "multiphoton-analysis/CommonClasses/interface/EventInfo.h"
// #include "multiphoton-analysis/CommonClasses/interface/TriggerInfo.h"
// #include "multiphoton-analysis/CommonClasses/interface/JetInfo.h"
// #include "multiphoton-analysis/CommonClasses/interface/PhotonID.h"
// #include "multiphoton-analysis/CommonClasses/interface/PhotonInfo.h"
// #include "multiphoton-analysis/CommonClasses/interface/VertexInfo.h"

using namespace std;
using namespace edm;


class FakeAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit FakeAnalyzer(const edm::ParameterSet&);
      ~FakeAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

};


#endif
