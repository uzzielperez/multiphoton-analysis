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
    double minDeltaR;
    double minDeltapT;
    double minDeltaPhi;
    double minDeltaEta;
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
    bool isMatched;
    bool isptdRmatched;


  };

  std::string genParticleBranchDefString("pt/D:eta:phi:deltaR_match:deltaR_matchDau:ptDiff_match:minDeltaR:minDeltapT:minDeltaPhi:minDeltaEta:matchCategory/I:matchType:nPhotonMotherDaughters:status:motherStatus:grandmotherStatus:pdgId:motherPdgId:grandmotherPdgId:genmatchindex:patmatchindex:isMatched/B:isptdRmatched");

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
    genParticleInfo.minDeltaR  = 999999;
    genParticleInfo.minDeltapT = 999999;
    genParticleInfo.minDeltaPhi = 999999;
    genParticleInfo.minDeltaEta = 999999;

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
    genParticleInfo.isMatched     = false;
    genParticleInfo.isptdRmatched = false;

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

// Returns a tuple of genpat matching information
// match bool, minDR, mindpT, ptdRmatch bool, genpat indices, and dphi, deta
std::tuple< std::vector<bool>,
            std::vector<double>,
            std::vector<double>,
            std::vector<bool>,
            std::vector<std::tuple<int,int>>,
            std::vector<double>,
            std::vector<double>,
            std::vector<const pat::Photon *>,
            std::vector<const reco::GenParticle *>> genpatmatchInfo(  std::vector< edm::Ptr<const reco::GenParticle> > genPhotons_sorted,
                                                    std::vector<edm::Ptr<pat::Photon>> patPhotons_sorted)
  {
      bool printInfo = false;
      bool pairsorted = false; // do not loop over pat collection, do pairwise calculation of dR
      bool debug = true;

      std::vector<bool> matchingInfo;
      std::vector<double> minDRvec;
      std::vector<double> minDpTvec;
      std::vector<bool> ptdRmatchInfo;
      std::vector<std::tuple<int, int>> genpatindexvec;
      std::vector<double> minDphivec;
      std::vector<double> minDetavec;
      std::vector<const pat::Photon *> patmatchedcollection;
      std::vector<const reco::GenParticle *> genmatchedcollection;

      double deltaR = 99999.99;
      double deltaPT = 99999.99;
      double deltaPhi = 99999.99;
      double deltaEta = 99999.99;
      double minDeltaR  = 99999.99;
      double minDeltapT  = 99999.99;
      double minDeltaPhi = 99999.99;
      double minDeltaEta = 99999.99;
      double isptmatched   = false;
      double ismatched     = false;
      double isptdRmatched = false;
      std::tuple <int, int> genpatindices = std::make_tuple(0,0);
      int gen_index = std::get<0>(genpatindices);
      int pat_index = std::get<1>(genpatindices);

      const reco::GenParticle *genPho;
      const pat::Photon *patPho;
      const pat::Photon *photon_reco_match = NULL;
      const reco::GenParticle *photon_gen_match = NULL;

      // Loop over sorted collections to calculate deltaR
      // std::cout << "Looping over genphotons collection of size " << genPhotons_sorted.size() << std::endl;
      // if (genPhotons_sorted.size() > 3)  std::cout << "MORE THAN 3 PHOTONS found " << std::endl;
      if (pairsorted){
        // std::vector<int>::size_type loopsize_;
        // if (patPhotons_sorted.size() < genPhotons_sorted.size()) loopsize_ = patPhotons_sorted.size();
        // else loopsize_ = genPhotons_sorted.size();
        // for(std::vector<int>::size_type i = 0; i != loopsize_; i++){
        for (std::vector<int>::size_type i = 0; i != genPhotons_sorted.size(); i++){
                deltaR = 99999.99;
                deltaPT = 99999.99;
                deltaPhi = 99999.99;
                deltaEta = 99999.99;
                minDeltaPhi = 99999.99;
                minDeltaEta = 99999.99;
                isptmatched   = false;
                ismatched     = false;
                isptdRmatched = false;
                genpatindices = std::make_tuple(0,0);
                gen_index = std::get<0>(genpatindices);
                pat_index = std::get<1>(genpatindices);

                 if (patPhotons_sorted.size() == genPhotons_sorted.size()){

                    patPho = &(*patPhotons_sorted.at(i));
                    genPho = &(*genPhotons_sorted.at(i));

                    deltaR = reco::deltaR(genPho->eta(), genPho->phi(), patPho->eta(), patPho->phi());
                    deltaPT = fabs(genPho->pt() - patPho->pt());
                    if (debug) std::cout << "Sorted Pair deltaR : " <<  deltaR << std::endl;
                    if (deltaR < 0.5) ismatched = true;
                    if (deltaPT <= 0.2*genPho->pt()) isptmatched = true;
                    if (ismatched && isptmatched) isptdRmatched = true;
                    deltaPhi = fabs(genPho->phi() - patPho->phi());
                    deltaEta = fabs(genPho->eta() - patPho->eta());

                 }

                  matchingInfo.push_back(ismatched);
                  minDRvec.push_back(deltaR);
                  minDpTvec.push_back(deltaPT);
                  ptdRmatchInfo.push_back(isptdRmatched);
                  genpatindexvec.push_back(genpatindices); //just default
                  minDphivec.push_back(deltaPhi);
                  minDetavec.push_back(deltaEta);
                  patmatchedcollection.push_back(photon_reco_match);
                  genmatchedcollection.push_back(photon_gen_match);

        }
      }

      if (!pairsorted){
        for (std::vector<int>::size_type i = 0; i != genPhotons_sorted.size(); i++)
        {
            // const auto gen = genPho->ptrAt(i);
            genPho = &(*genPhotons_sorted.at(i));
            minDeltaR = 99999.99;
            minDeltapT = 99999.99;
            minDeltaPhi = 99999.99;
            minDeltaEta = 99999.99;
            isptmatched = false;
            ismatched = false;
            isptdRmatched = false;
            std::tuple <int, int> genpatindices;
            genpatindices = std::make_tuple(0,0);
            gen_index = std::get<0>(genpatindices);
            pat_index = std::get<1>(genpatindices);
            // int gen_index = 100;
            // int pat_index = 100;

            if (printInfo) std::cout << "GenPho: pt = " << genPho->pt() << "; eta = " << genPho->eta() << "; phi = " << genPho->phi() << std::endl;

            for(std::vector<int>::size_type j = 0; j != patPhotons_sorted.size(); j++)
            {
                patPho = &(*patPhotons_sorted.at(j));
                double deltaR = reco::deltaR(genPho->eta(), genPho->phi(), patPho->eta(), patPho->phi());
                deltaPT = fabs(genPho->pt() - patPho->pt());
                double deltaPhi = fabs(genPho->phi() - patPho->phi());
                double deltaEta = fabs(genPho->eta() - patPho->eta());
                // bool isinGap = false;
                // if (patPho->isEBEtaGap() || patPho->isEBPhiGap() || patPho->isEERingGap() || patPho->isEEDeeGap() || patPho->isEBEEGap() ) isinGap = true;
                // bool hasconversiontracks = patPho->hasConversionTracks();

                //if (printInfo) std::cout << "Pho: pt = " << patPho->pt() << "; eta = " << patPho->eta() << "; phi = " << patPho->phi() << "; deltaR = " << deltaR << "; GAP? " << isinGap << "; hasConversionTracks: " << hasconversiontracks << "; deltaPT = " << deltaPT <<std::endl;

                if (deltaPT <= 0.2*genPho->pt()) isptmatched = true;
                if (deltaR <= minDeltaR)
                {
                    minDeltaR = deltaR;
                    minDeltapT = deltaPT;
                    minDeltaPhi = deltaPhi;
                    minDeltaEta = deltaEta;
                    if (minDeltaR<0.5) ismatched = true;
                    gen_index = i;
                    pat_index = j;
                    genpatindices = std::make_tuple( gen_index , pat_index );
                    photon_reco_match = &(*patPho);
                    photon_gen_match = &(*genPho);
                    if (isptmatched) isptdRmatched = true;
                }
            } // end loop over pat photons
                if (ismatched && printInfo) cout << "MATCH FOUND! minDR: " << minDeltaR  << "; dPT: " <<  minDeltapT
                                    << "; gen:pat pt = " << photon_gen_match->pt() << " : " << photon_reco_match->pt()
                                    << "; eta = " << photon_gen_match->eta() << " : " << photon_reco_match->eta()
                                    << "; phi = " << photon_gen_match->phi() << " : " << photon_reco_match->phi()
                                    << "; indices = " << gen_index <<  " : " << pat_index
                                    << "; ptdRmatched = " << isptdRmatched
                                    << std::endl;

             matchingInfo.push_back(ismatched);
             minDRvec.push_back(minDeltaR);
             minDpTvec.push_back(minDeltapT);
             ptdRmatchInfo.push_back(isptdRmatched);
             genpatindexvec.push_back(genpatindices);
             minDphivec.push_back(minDeltaPhi);
             minDetavec.push_back(minDeltaEta);
             // Also want to keep the pat photon that matched to that particular gen photon
             patmatchedcollection.push_back(photon_reco_match);
             genmatchedcollection.push_back(photon_gen_match);

             // Later if I want to retrieve the full matching information:
             // patmatchedcollection.push_back(photon_reco_match);
             // genmatchedcollection.push_back(photon_gen_match);
        }

      } // end if on loop over

      return {matchingInfo, minDRvec, minDpTvec, ptdRmatchInfo, genpatindexvec, minDphivec, minDetavec, patmatchedcollection, genmatchedcollection};
}
void FillGenPATmatchInfo(genParticleInfo_t &genParticleInfo,
                         bool matchInfo,
                         double minDeltaR,
                         double minDeltapT,
                         bool ptdRmatchInfo,
                         std::tuple <int, int> genpatindices,
                         double minDeltaPhi,
                         double minDeltaEta
                         ){
      genParticleInfo.genmatchindex = std::get<0>(genpatindices);
      genParticleInfo.patmatchindex = std::get<1>(genpatindices);
      genParticleInfo.isMatched = matchInfo;
      genParticleInfo.isptdRmatched = ptdRmatchInfo;
      genParticleInfo.minDeltaR = minDeltaR;
      genParticleInfo.minDeltapT = minDeltapT;
      genParticleInfo.minDeltaPhi = minDeltaPhi;
      genParticleInfo.minDeltaEta = minDeltaEta;
  }
}// end of namespace

#endif
