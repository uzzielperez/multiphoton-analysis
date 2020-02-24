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

    // If Gen Photon is reconstructed these information are stored:
    int genmatchindex;
    int patmatchindex;
    bool isptdRmatched;
    bool isMatched;
    double minDeltaR;
    double minDeltapT;
  };

  std::string genParticleBranchDefString("pt/D:eta:phi:deltaR_match:deltaR_matchDau:ptDiff_match:matchCategory/I:matchType:nPhotonMotherDaughters:status:motherStatus:grandmotherStatus:pdgId:motherPdgId:grandmotherPdgId:genmatchindex:patmatchindex:isMatched/B:isptdRmatched:minDeltaR/D:minDeltapT");

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

    // matching info
    genParticleInfo.genmatchindex = -99999;
    genParticleInfo.patmatchindex = -99999;
    genParticleInfo.isMatched  = false;
    genParticleInfo.minDeltaR  = 999999;
    genParticleInfo.minDeltapT = 999999;
  }

  void FillGenParticleInfo(genParticleInfo_t &genParticleInfo,
                           const reco::GenParticle *genParticle,
                           const edm::Handle<edm::View<pat::Photon> >& photons) {
    // gen info
    genParticleInfo.status = genParticle->status();
    genParticleInfo.pdgId  = genParticle->pdgId();

    //Matching Info

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

//Putting these here to avoid unecessary header PhotonInfo.h
  // sort two photons by highest pt with vector of structs
  bool comparePhotonsByPts(genParticleInfo_t photon1, genParticleInfo_t photon2) {
         return(photon1.pt>=photon2.pt);
  }
  // sort two photons by highest pt
  bool comparePhotonsByPt(const edm::Ptr<const reco::Candidate> photon1, const edm::Ptr<const reco::Candidate> photon2) {
    return(photon1->pt()>=photon2->pt());
  }

std::tuple< std::vector<bool>, std::vector<double>, std::vector<double>, std::vector<bool>,
            std::vector<std::tuple<int,int>> > genpatmatchInfo(
                         std::vector< edm::Ptr<const reco::GenParticle> > genPhotons_sorted,
                         std::vector<edm::Ptr<pat::Photon>> patPhotons_sorted){

      std::vector<bool> matchingInfo;
      std::vector<double> minDRvec;
      std::vector<double> minDpTvec;
      std::vector<bool> ptdRmatchInfo;
      std::vector<std::tuple<int, int>> genpatindexvec;

      // std::vector<const pat::Photon> patmatchedcollection;
      // std::vector<const reco::GenParticle> genmatchedcollection;
      // Loop over sorted collections to calculate deltaR
      // std::cout << "Looping over genphotons collection of size " << genPhotons_sorted.size() << std::endl;
      // if (genPhotons_sorted.size() > 3)  std::cout << "MORE THAN 3 PHOTONS found " << std::endl;
      for (std::vector<int>::size_type i = 0; i != genPhotons_sorted.size(); i++)
      {
          // const auto gen = genPho->ptrAt(i);
          const reco::GenParticle *genPho = &(*genPhotons_sorted.at(i));
          double minDeltaR = 99999.99;
          double minDeltapT = 99999.99;
          double deltaPT;
          bool isptmatched = false;
          bool ismatched = false;
          bool isptdRmatched = false;
          std::tuple <int, int> genpatindices;
          genpatindices = std::make_tuple(-99999,-99999);
          int gen_index = std::get<0>(genpatindices);
          int pat_index = std::get<1>(genpatindices);
          // int gen_index = 100;
          // int pat_index = 100;


          const pat::Photon *photon_reco_match = NULL;
          const reco::GenParticle *photon_gen_match = NULL;
          std::cout << "GenPho: pt = " << genPho->pt() << "; eta = " << genPho->eta() << "; phi = " << genPho->phi() << std::endl;

          for(std::vector<int>::size_type j = 0; j != patPhotons_sorted.size(); j++)
          {
              const pat::Photon *patPho = &(*patPhotons_sorted.at(j));
              double deltaR = reco::deltaR(genPho->eta(), genPho->phi(), patPho->eta(), patPho->phi());
              deltaPT = fabs(genPho->pt() - patPho->pt());

              std::cout << "Pho: pt = " << patPho->pt() << "; eta = " << patPho->eta() << "; phi = " << patPho->phi() << "; deltaR = " << deltaR << "; deltaPT = " << deltaPT <<std::endl;

              if (deltaPT <= 0.2*genPho->pt()) isptmatched = true;
              if (deltaR <= minDeltaR)
              {
                  minDeltaR = deltaR;
                  minDeltapT = deltaPT;
                  if (minDeltaR<0.10) ismatched = true;
                  gen_index = i;
                  pat_index = j;
                  genpatindices = std::make_tuple( gen_index , pat_index );
                  photon_reco_match = &(*patPho);
                  photon_gen_match = &(*genPho);
                  if (isptmatched) isptdRmatched = true;
              }
          }
              if (ismatched) cout << "MATCH FOUND! minDR: " << minDeltaR  << "; dPT: " <<  minDeltapT
                                  << "; gen:pat pt = " << photon_gen_match->pt() << " : " << photon_reco_match->pt()
                                  << "; gen:pat eta = " << photon_gen_match->eta() << " : " << photon_reco_match->eta()
                                  << "; gen:pat phi = " << photon_gen_match->phi() << " : " << photon_reco_match->phi()
                                  << "; gen:pat index = " << gen_index <<  " : " << pat_index
                                  << "; ptdRmatched = " << isptdRmatched
                                  << std::endl;

           matchingInfo.push_back(ismatched);
           minDRvec.push_back(minDeltaR);
           minDpTvec.push_back(minDeltapT);
           ptdRmatchInfo.push_back(isptdRmatched);
           genpatindexvec.push_back(genpatindices);

           // Later if I want to retrieve the full matching information:
           // patmatchedcollection.push_back(photon_reco_match);
           // genmatchedcollection.push_back(photon_gen_match);
      }
      return {matchingInfo, minDRvec, minDpTvec, ptdRmatchInfo, genpatindexvec};
}

void FillGenPATmatchInfo(genParticleInfo_t &genParticleInfo,
                         bool matchInfo,
                         double minDeltaR,
                         double minDeltapT,
                         bool ptdRmatchInfo,
                         std::tuple <int, int> genpatindices
                         ){
      genParticleInfo.genmatchindex = std::get<0>(genpatindices);
      genParticleInfo.patmatchindex = std::get<1>(genpatindices);
      genParticleInfo.isMatched = matchInfo;
      genParticleInfo.isptdRmatched = ptdRmatchInfo;
      genParticleInfo.minDeltaR = minDeltaR;
      genParticleInfo.minDeltapT = minDeltapT;
  }
}// end of namespace

#endif
