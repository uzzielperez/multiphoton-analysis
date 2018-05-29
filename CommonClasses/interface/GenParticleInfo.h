#ifndef GEN_PARTICLE_INFO
#define GEN_PARTICLE_INFO

#include "multiphoton-analysis/CommonClasses/interface/DiPhotonInfo.h"
#include "TMath.h"
#include "TLorentzVector.h"
//#include <typeinfo>


using namespace std;

namespace ExoDiPhotons
{

  struct GenMatchingFlags {

    enum MatchCategoryFlags {
      // match category:
      kNoMatchCategory = 0,      // 0 - no match category according to:
      kFinalStatePhotonMatch,    // 1 - final state photon match in desired DeltaR core
      kFinalStateNonPhotonMatch, // 2 - final state match NOT to photon in same DeltaR cone
      kGenParticlePhotonMatch,   // 3 - if no final state match, genParticle match of any status photon in desired DeltaR cone
      kGenParticleNonPhotonMatch // 4 - if no final state match, non-photon genParticle match of any status in desired DeltaR cone
    };

    enum MatchTypeFlags {
      // match type:
      kNoMatchType = 0,          // 0 - no match type
      kHadronMother,             // 1 - is hadron final state match or has hadron mother (fake)
      kISRfromProton,            // 2 - photon ISR directly from interacting proton (real)
      kOtherPhotonRadiation,     // 3 - other photon radiation (real)
      kGluonMotherPhotonFrag,    // 4 - photon fragmentation when photon's first mother is a gluon (fake)
      kOtherPhotonFragmentation, // 5 - other photon fragmentation (fake)
      kElectronPairMatch,        // 6 - final state electron match and electron pair found with no hadron mother (real or fake?)
      kPhotonMother,             // 7 - real photon with hard interaction photon mother
      kRealTemplatePhoton        // 8 - real template photon
    };

  };

  struct genParticleInfo_t {
    // kinematics
    double pt;
    double eta;
    double phi;

    // for matching
    double deltaR_match;
    double deltaR_matchDau;
    double ptDiff_match;
    int matchCategory;
    int matchType;
    int nPhotonMotherDaughters;

    // gen info
    int status;
    int motherStatus;
    int grandmotherStatus;
    int pdgId;
    int motherPdgId;
    int grandmotherPdgId;

    // Additional Information (Not yet included in Branch)
    //TLorentzVector p4;
    //reco::LeafCandidate::LorentzVector p4;

  };

  std::string genParticleBranchDefString("pt/D:eta:phi:deltaR_match:deltaR_matchDau:ptDiff_match:matchCategory/I:matchType:nPhotonMotherDaughters:status:motherStatus:grandmotherStatus:pdgId:motherPdgId:grandmotherPdgId");

  void InitGenParticleInfo(genParticleInfo_t &genParticleInfo) {
    // kinematics
    genParticleInfo.pt  = -999999.99;
    genParticleInfo.eta = -999999.99;
    genParticleInfo.phi = -999999.99;

    // for matching
    genParticleInfo.deltaR_match           = -999999.99;
    genParticleInfo.deltaR_matchDau        = -999999.99;
    genParticleInfo.ptDiff_match           = -999999.99;
    genParticleInfo.matchCategory          = -999999;
    genParticleInfo.matchType              = -999999;
    genParticleInfo.nPhotonMotherDaughters = -999999;

    // gen info
    genParticleInfo.status            = -999999;
    genParticleInfo.motherStatus      = -999999;
    genParticleInfo.grandmotherStatus = -999999;
    genParticleInfo.pdgId             = -999999;
    genParticleInfo.motherPdgId       = -999999;
    genParticleInfo.grandmotherPdgId  = -999999;
  }

  void FillGenParticleInfo(genParticleInfo_t &genParticleInfo, const reco::GenParticle *genParticle) {
    // gen info
    genParticleInfo.status = genParticle->status();
    genParticleInfo.pdgId  = genParticle->pdgId();

    // gen info
    // mother and grandmother info
    double minMotherDeltaR      = 100000; // consider all mothers
    double minGrandmotherDeltaR = 100000;
    int motherIndex      = 0;
    int grandmotherIndex = 0;

    // if mother exists
    if (genParticle->numberOfMothers() > 0) {
      // find best match in deltaR among all mothers
      for (unsigned int j = 0; j < genParticle->numberOfMothers(); j++) {
	double deltaR = reco::deltaR(genParticle->eta(),genParticle->phi(),genParticle->mother(j)->eta(),genParticle->mother(j)->phi());
	if (deltaR < minMotherDeltaR) {
	  minMotherDeltaR = deltaR;
	  motherIndex = j;
	}
      }
      genParticleInfo.motherStatus = genParticle->mother(motherIndex)->status();
      genParticleInfo.motherPdgId  = genParticle->mother(motherIndex)->pdgId();

      // if grandMother exists
      if (genParticle->mother(motherIndex)->numberOfMothers() > 0) {
	// find best match in deltaR among all mothers
	for (unsigned int j = 0; j < genParticle->mother(motherIndex)->numberOfMothers(); j++) {
	  double deltaR = reco::deltaR(genParticle->mother(motherIndex)->eta(),genParticle->mother(motherIndex)->phi(),
				       genParticle->mother(motherIndex)->mother(j)->eta(),genParticle->mother(motherIndex)->mother(j)->phi());
	  if (deltaR < minGrandmotherDeltaR) {
	    minGrandmotherDeltaR = deltaR;
	    grandmotherIndex = j;
	  }
	}
	genParticleInfo.grandmotherStatus = genParticle->mother(motherIndex)->mother(grandmotherIndex)->status();
	genParticleInfo.grandmotherPdgId  = genParticle->mother(motherIndex)->mother(grandmotherIndex)->pdgId();
      }
      else {
	genParticleInfo.grandmotherStatus = -9999999;
	genParticleInfo.grandmotherPdgId  = -9999999;
      }
    }
    else {
      genParticleInfo.motherStatus      = -9999999;
      genParticleInfo.grandmotherStatus = -9999999;
      genParticleInfo.motherPdgId       = -9999999;
      genParticleInfo.grandmotherPdgId  = -9999999;
    }

    // kinematics
    genParticleInfo.pt  = genParticle->pt();
    genParticleInfo.eta = genParticle->eta();
    genParticleInfo.phi = genParticle->phi();

  }//miniAOD

// sort two photons by highest pt
bool comparePhotonsByPt(genParticleInfo_t photon1, genParticleInfo_t photon2) {
       return(photon1.pt>=photon2.pt);
}


void fillGenInfo(const edm::Handle<edm::View<reco::GenParticle> > genParticles){

      // Store Information in these vectors
      vector< edm::Ptr<const reco::GenParticle> > genPhotons;
      vector<int> interactingPartons;

      //Test
      //const reco::GenParticle *genPho1 = NULL;
      //const reco::GenParticle *genPho2 = NULL;


      for (size_t i = 0; i < genParticles->size(); ++i){
        const auto gen = genParticles->ptrAt(i);

        //print All Hard Interaction gen genParticles
        //is Status 3 == Hard Interaction in Pythia6
        //if (gen->status()==3)
        //Pythia8 status 20-30 is Hard interaction
        if (gen->isHardProcess() && gen->pt() == 0)   interactingPartons.push_back(gen->pdgId());
        if (gen->isHardProcess() && gen->pdgId()==22) genPhotons.push_back(gen);

        //if (gen->status()<20 && gen->status()>30) This is HardProcess
        if () cout << "Status: "                       << gen->status()
                                                       << ";HardProcess?:    "            << gen->isHardProcess()
                                                       << "; pdgId: "                     << gen->pdgId()
                                                       << "; pt: "                        << gen->pt()
                                                       << "; eta: "                       << gen->eta()
                                                       << "; phi: "                       << gen->phi() << endl;
      }//end for loop over gen particles

/*
      cout << "GenPhoton1 Info: status " << genPho1->status()
           << "; pdgId: "                << genPho1->pdgId()
           << "; pt: "                   << genPho1->pt()
           << "; eta: "                  << genPho1->eta()
           << "; phi: "                  << genPho1->phi() << endl;
      cout << "GenPhoton2 Info: status " << genPho2->status()
           << "; pdgId: "                << genPho2->pdgId()
           << "; pt: "                   << genPho2->pt()
           << "; eta: "                  << genPho2->eta()
           << "; phi: "                  << genPho2->phi() << endl;
*/
      // end test

      //---Update Information

      //ExoDiPhotons::FillBasicEventInfo(fEventInfo, iEvent);
      //ExoDiPhotons::fillGenDiPhoInfo(  fGenPhoton1Info, fGenPhoton2Info, fGenDiPhotonInfo, genParticles);
}//end of fillGenInfo



//void fillGenDiPhoInfo(genParticleInfo_t& fGenPhoton1Info, genParticleInfo_t& fGenPhoton2Info,
//                      diphotonInfo_t &diphotonInfo, const edm::Handle<vector<reco::GenParticle> > genParticles)
//=======
/*

void fillGenDiPhoInfo(genParticleInfo_t& fGenPhoton1Info, genParticleInfo_t& fGenPhoton2Info,
  //diphotonInfo_t &diphotonInfo, const reco::GenParticle *genParticles)
                    diphotonInfo_t &diphotonInfo, const edm::Handle<vector<reco::GenParticle> > genParticles)
>>>>>>> 28a21ae37cb6c5d3dec90d7f0219692e7894fd80
{
   int photoncount = 0;
   //Temporary storage
   vector<genParticleInfo_t> genPhoInfoStructs;
   genParticleInfo_t genParticleInfo;
   InitGenParticleInfo(genParticleInfo);

   //Loop over the genParticles. Handle in plugin nPhotonAnalyzer.cc
   for(vector<reco::GenParticle>::const_iterator ip = genParticles->begin(); ip != genParticles->end(); ++ip)
<<<<<<< HEAD
   {
      int status = ip->status();
      int PID    = ip->pdgId();
      reco::LeafCandidate::LorentzVector p4     = ip->p4();
=======
  //for(edm::View<reco::GenParticle>::const_iterator ip = genParticles->begin(); ip != genParticles->end(); ++ip)
   {
      int status                            = ip->status();
      int PID                               = ip->pdgId();
      double pt                             = ip->pt();
      double eta                            = ip->eta();
      double phi                            = ip->phi();
      reco::LeafCandidate::LorentzVector p4 = ip->p4();
>>>>>>> 28a21ae37cb6c5d3dec90d7f0219692e7894fd80

      //else throw cms::Exception("Should always have exactly two photons with status==3 in the diphoton sample");
      //if(ip->isHardProcess()){
        if((status==1 || status ==2) && PID==22){
      //cout << "Photon end state found" << endl;
        photoncount = photoncount + 1;

        //Kinematics
<<<<<<< HEAD
        genParticleInfo.pt  = pt;
        genParticleInfo.eta = eta;
        genParticleInfo.phi = phi;
=======
        genParticleInfo.pt     = pt;
        genParticleInfo.eta    = eta;
        genParticleInfo.phi    = phi;
>>>>>>> 28a21ae37cb6c5d3dec90d7f0219692e7894fd80

        //Matching (Later)

        //genInfo
        genParticleInfo.status = status;
        genParticleInfo.pdgId  = PID;

        //Additional Info
        genParticleInfo.p4     = p4;

        //Store infoStruct in vector
        genPhoInfoStructs.push_back(genParticleInfo);

        //}//end HardProcess Condition
      }//end photon end state condition

  }//end loop over gen particles

    //Sort Photon objects by pt
    sort(genPhoInfoStructs.begin(), genPhoInfoStructs.end(), comparePhotonsByPt);
    fGenPhoton1Info = genPhoInfoStructs[0];
    fGenPhoton2Info = genPhoInfoStructs[1];


    //If there are two photon objects then store DiPhotonInfo
    if(genPhoInfoStructs.size()>2){
      cout << "Storing DiPhotonInfo" << endl;
      ExoDiPhotons::FillDiphotonInfo(diphotonInfo, fGenPhoton1Info.p4, fGenPhoton2Info.p4);
    }

    //Debugging
    vector<genParticleInfo_t>::const_iterator iter;

    cout << "Number of photons in event: " << photoncount << endl;
    int jcounter = 0;
    for (iter = genPhoInfoStructs.begin(); iter != genPhoInfoStructs.end(); ++iter){
      if (jcounter<2){
          cout << "photonobjects_pt: " << (*iter).pt
               << "; Status: "         << (*iter).status
               << endl;
       jcounter = jcounter + 1;
      }//endfilling
    }//end loop
    //End Debugging

}//end of fillGenDiPhoInfo
*/
}//end of namespace


#endif
