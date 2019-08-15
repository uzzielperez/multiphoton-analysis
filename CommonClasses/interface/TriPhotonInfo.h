#ifndef TRIPHOTON_INFO_INC
#define TRIPHOTON_INFO_INC

#include "TMath.h"
#include "TLorentzVector.h"
#include "multiphoton-analysis/CommonClasses/interface/GenParticleInfo.h"

namespace ExoDiPhotons
{

  struct triphotonInfo_t {
    // kinematics
    double Minv;
    double qt;
    // double deltaPhi;
    // double deltaEta;
    // double deltaR;
    // double cosThetaStar;
    // double cosThetaStar_old;
    // double chiDiphoton;

    // fiducial flags
    bool isEBEBEB;
    bool isEBEEEB;
    bool isEBEBEE;
    bool isEEEBEB;
    bool isEEEEEB;
    bool isEEEEEE;
  };

  std::string triphotonBranchDefString("Minv/D:qt");
  //std::string diphotonBranchDefString("Minv/D:qt:deltaPhi:deltaEta:deltaR:cosThetaStar:cosThetaStar_old:chiDiphoton:isEBEB/O:isEBEE:isEEEB:isEEEE");

  void InitTriphotonInfo(triphotonInfo_t &triphotonInfo) {
    // kinematics
    triphotonInfo.Minv             = -99999.99;
    triphotonInfo.qt               = -99999.99;
    // triphotonInfo.deltaPhi         = -99999.99;
    // triphotonInfo.deltaEta         = -99999.99;
    // triphotonInfo.deltaR           = -99999.99;
    // triphotonInfo.cosThetaStar     = -99999.99;
    // triphotonInfo.cosThetaStar_old = -99999.99;
    // triphotonInfo.chiDiphoton      = -99999.99;

    // fiducial flags
    triphotonInfo.isEBEBEB = false;
    triphotonInfo.isEBEEEB = false;
    triphotonInfo.isEBEBEE = false;
    triphotonInfo.isEEEBEB = false;
    triphotonInfo.isEEEEEB = false;
    triphotonInfo.isEEEEEE = false;

  }

  // the internal function which gets called by the others
  void FillTriphotonInfo(triphotonInfo_t &triphotonInfo, reco::LeafCandidate::LorentzVector photon_vector1, reco::LeafCandidate::LorentzVector photon_vector2, reco::LeafCandidate::LorentzVector photon_vector3)
  {
    triphotonInfo.Minv = (photon_vector1+photon_vector2+photon_vector3).M();
    // pt of the pai
    triphotonInfo.qt = (photon_vector1+photon_vector2+photon_vector3).pt();

    // What other quantities do we want to extract?
    // there's a CMS function for deltaPhi in DataFormats/Math
    //triphotonInfo.deltaPhi = reco::deltaPhi(photon_vector1.phi(),photon_vector2.phi());
    //triphotonInfo.deltaEta = photon_vector1.eta()-photon_vector2.eta(); // always highest pt - second
    // use CMS function for deltaR
    //triphotonInfo.deltaR = reco::deltaR(photon_vector1.eta(),photon_vector1.phi(),photon_vector2.eta(),photon_vector2.phi());
    // old deltaR - leave in to check
    //triphotonInfo.deltaROld = TMath::Sqrt(triphotonInfo.deltaPhi*triphotonInfo.deltaPhi+triphotonInfo.deltaEta*triphotonInfo.deltaEta);
    //    reco::LeafCandidate::LorentzVector triphotonInfo = photon_vector1 + photon_vector2;
    //    double mymom = triphotonInfo.P();

    // boost photon 1 into triphotonInfo rest frame
    // the Boost() function changes the acutal Lorentz vector that its called
    // first lets boost it, then call CosTheta() after boosting

    //reco::LeafCandidate::LorentzVector photon1_clone = photon_vector1.Clone();
    TLorentzVector photon1_clone(photon_vector1.px(),photon_vector1.py(),photon_vector1.pz(),photon_vector1.E());
    TLorentzVector photon2_clone(photon_vector2.px(),photon_vector2.py(),photon_vector2.pz(),photon_vector2.E());
    TLorentzVector photon3_clone(photon_vector3.px(),photon_vector3.py(),photon_vector3.pz(),photon_vector3.E());
    TLorentzVector triphoton_system = photon1_clone+photon2_clone+photon3_clone;
    photon1_clone.Boost(-triphoton_system.BoostVector());
    //photon1_clone.Boost(-(photon_vector1+photon_vector2).BoostVector());
    //triphotonInfo.cosThetaStar = photon1_clone.CosTheta();
    // this older implementation was provided by Yousi Ma - should check it sometime
    //triphotonInfo.cosThetaStar_old = fabs(photon_vector1.P() - photon_vector2.P())/(photon_vector1+photon_vector2).P();
    // chiDiphoton
    //triphotonInfo.chiDiphoton = exp(fabs(photon_vector1.Rapidity()-photon_vector2.Rapidity()));
  }

  // filling function for reco photons
  void FillTriphotonInfo(triphotonInfo_t &triphotonInfo, const reco::Photon *photon1, const reco::Photon *photon2, const reco::Photon *photon3)
  {
    reco::LeafCandidate::LorentzVector photon_vector1 = photon1->p4();
    reco::LeafCandidate::LorentzVector photon_vector2 = photon2->p4();
    reco::LeafCandidate::LorentzVector photon_vector3 = photon3->p4();

    FillTriphotonInfo(triphotonInfo,photon_vector1,photon_vector2,photon_vector3);
  }

  // filling function for pat photons
  void FillTriphotonInfo(triphotonInfo_t &triphotonInfo, const pat::Photon *photon1, const pat::Photon *photon2, const pat::Photon *photon3)
  {
    reco::LeafCandidate::LorentzVector photon_vector1 = photon1->p4();
    reco::LeafCandidate::LorentzVector photon_vector2 = photon2->p4();
    reco::LeafCandidate::LorentzVector photon_vector3 = photon3->p4();

    FillTriphotonInfo(triphotonInfo,photon_vector1,photon_vector2,photon_vector3);

    // photon1 is leading and photon2 is sub-leading
    double photon1_eta = std::abs(photon1->superCluster()->eta());
    double photon2_eta = std::abs(photon2->superCluster()->eta());
    double photon3_eta = std::abs(photon3->superCluster()->eta());

    triphotonInfo.isEBEBEB = (photon1_eta < 1.4442) && (photon2_eta < 1.4442) && (photon3_eta < 1.4442);
    triphotonInfo.isEBEEEB = (photon1_eta < 1.4442) && (1.566 < photon2_eta && photon2_eta < 2.5) && (photon3_eta < 1.4442);
    triphotonInfo.isEBEBEE = (photon1_eta < 1.4442) && (photon2_eta < 1.4442) && (1.566 < photon3_eta && photon3_eta < 2.5);
    triphotonInfo.isEEEBEB = (1.566 < photon1_eta && photon1_eta < 2.5) && (photon2_eta < 1.4442) && (photon3_eta < 1.4442);
    triphotonInfo.isEEEEEB = (1.566 < photon1_eta && photon1_eta < 2.5) && (1.566 < photon2_eta && photon2_eta < 2.5) && (photon3_eta < 1.4442);
    triphotonInfo.isEEEEEE = (1.566 < photon1_eta && photon1_eta < 2.5) && (1.566 < photon2_eta && photon2_eta < 2.5) && (1.566 < photon3_eta && photon3_eta < 2.5);
    // triphotonInfo.isEBEB = (photon1_eta < 1.4442) && (photon2_eta < 1.4442);
    // triphotonInfo.isEBEE = (photon1_eta < 1.4442) && (1.566 < photon2_eta && photon2_eta < 2.5);
    // triphotonInfo.isEEEB = (1.566 < photon1_eta && photon1_eta < 2.5) && (photon2_eta < 1.4442);
    // triphotonInfo.isEEEE = (1.566 < photon1_eta && photon1_eta < 2.5) && (1.566 < photon2_eta && photon2_eta < 2.5);
  }

  // same function, but for MC signal photons
  void FillTriphotonInfo(triphotonInfo_t &triphotonInfo, const reco::GenParticle *photon1, const reco::GenParticle *photon2, const reco::GenParticle *photon3)
  {
    reco::LeafCandidate::LorentzVector photon_vector1 = photon1->p4();
    reco::LeafCandidate::LorentzVector photon_vector2 = photon2->p4();
    reco::LeafCandidate::LorentzVector photon_vector3 = photon3->p4();

    FillTriphotonInfo(triphotonInfo,photon_vector1,photon_vector2,photon_vector3);
  }

  // same function, but with reco::Candidates
  void FillTriphotonInfo(triphotonInfo_t &triphotonInfo, const reco::Candidate *photon1, const reco::Candidate *photon2, const reco::Candidate *photon3)
  {
    reco::LeafCandidate::LorentzVector photon_vector1 = photon1->p4();
    reco::LeafCandidate::LorentzVector photon_vector2 = photon2->p4();
    reco::LeafCandidate::LorentzVector photon_vector3 = photon3->p4();

    FillTriphotonInfo(triphotonInfo,photon_vector1,photon_vector2,photon_vector3);
  }

  // // same function, but for local MC signal photons
  // void FillDiphotonInfo(triphotonInfo_t &triphotonInfo, reco::LeafCandidate::LorentzVector genPhoton1p4, reco::LeafCandidate::LorentzVector genPhoton2p4)
  // {
  //   reco::LeafCandidate::LorentzVector photon_vector1 = genPhoton1p4;
  //   reco::LeafCandidate::LorentzVector photon_vector2 = genPhoton2p4;
  //
  //   FillDiphotonInfo(triphotonInfo,photon_vector1,photon_vector2);
  // }

}

#endif
