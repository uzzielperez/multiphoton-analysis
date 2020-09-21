//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Sep 21 08:16:38 2020 by ROOT version 6.14/09
// from TChain demo/fTree/
//////////////////////////////////////////////////////////

#ifndef TempSignalStudies_h
#define TempSignalStudies_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class TempSignalStudies {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Long64_t        Event_run;
   Long64_t        Event_LS;
   Long64_t        Event_evnum;
   Long64_t        Event_processid;
   Long64_t        Event_bx;
   Long64_t        Event_orbit;
   Float_t         Event_ptHat;
   Float_t         Event_alphaqcd;
   Float_t         Event_alphaqed;
   Float_t         Event_qscale;
   Float_t         Event_x1;
   Float_t         Event_x2;
   Float_t         Event_pdf1;
   Float_t         Event_pdf2;
   Float_t         Event_weight0;
   Float_t         Event_weight;
   Float_t         Event_weightPuUp;
   Float_t         Event_weightPu;
   Float_t         Event_weightPuDown;
   Float_t         Event_weightLumi;
   Float_t         Event_weightAll;
   Int_t           Event_interactingParton1PdgId;
   Int_t           Event_interactingParton2PdgId;
   Int_t           Event_pdf_id1;
   Int_t           Event_pdf_id2;
   Int_t           Event_npv_true;
   Bool_t          Event_beamHaloIDLoose;
   Bool_t          Event_beamHaloIDTight;
   Bool_t          Event_beamHaloIDTight2015;
   Double_t        GenPhoton1_pt;
   Double_t        GenPhoton1_eta;
   Double_t        GenPhoton1_phi;
   Double_t        GenPhoton1_deltaR_match;
   Double_t        GenPhoton1_deltaR_matchDau;
   Double_t        GenPhoton1_ptDiff_match;
   Double_t        GenPhoton1_minDeltaR;
   Double_t        GenPhoton1_minDeltapT;
   Double_t        GenPhoton1_minDeltaPhi;
   Double_t        GenPhoton1_minDeltaEta;
   Int_t           GenPhoton1_matchCategory;
   Int_t           GenPhoton1_matchType;
   Int_t           GenPhoton1_nPhotonMotherDaughters;
   Int_t           GenPhoton1_status;
   Int_t           GenPhoton1_motherStatus;
   Int_t           GenPhoton1_grandmotherStatus;
   Int_t           GenPhoton1_pdgId;
   Int_t           GenPhoton1_motherPdgId;
   Int_t           GenPhoton1_grandmotherPdgId;
   Int_t           GenPhoton1_genmatchindex;
   Int_t           GenPhoton1_patmatchindex;
   Char_t          GenPhoton1_isMatched;
   Char_t          GenPhoton1_isptdRmatched;
   Double_t        GenPhoton2_pt;
   Double_t        GenPhoton2_eta;
   Double_t        GenPhoton2_phi;
   Double_t        GenPhoton2_deltaR_match;
   Double_t        GenPhoton2_deltaR_matchDau;
   Double_t        GenPhoton2_ptDiff_match;
   Double_t        GenPhoton2_minDeltaR;
   Double_t        GenPhoton2_minDeltapT;
   Double_t        GenPhoton2_minDeltaPhi;
   Double_t        GenPhoton2_minDeltaEta;
   Int_t           GenPhoton2_matchCategory;
   Int_t           GenPhoton2_matchType;
   Int_t           GenPhoton2_nPhotonMotherDaughters;
   Int_t           GenPhoton2_status;
   Int_t           GenPhoton2_motherStatus;
   Int_t           GenPhoton2_grandmotherStatus;
   Int_t           GenPhoton2_pdgId;
   Int_t           GenPhoton2_motherPdgId;
   Int_t           GenPhoton2_grandmotherPdgId;
   Int_t           GenPhoton2_genmatchindex;
   Int_t           GenPhoton2_patmatchindex;
   Char_t          GenPhoton2_isMatched;
   Char_t          GenPhoton2_isptdRmatched;
   Double_t        GenPhoton3_pt;
   Double_t        GenPhoton3_eta;
   Double_t        GenPhoton3_phi;
   Double_t        GenPhoton3_deltaR_match;
   Double_t        GenPhoton3_deltaR_matchDau;
   Double_t        GenPhoton3_ptDiff_match;
   Double_t        GenPhoton3_minDeltaR;
   Double_t        GenPhoton3_minDeltapT;
   Double_t        GenPhoton3_minDeltaPhi;
   Double_t        GenPhoton3_minDeltaEta;
   Int_t           GenPhoton3_matchCategory;
   Int_t           GenPhoton3_matchType;
   Int_t           GenPhoton3_nPhotonMotherDaughters;
   Int_t           GenPhoton3_status;
   Int_t           GenPhoton3_motherStatus;
   Int_t           GenPhoton3_grandmotherStatus;
   Int_t           GenPhoton3_pdgId;
   Int_t           GenPhoton3_motherPdgId;
   Int_t           GenPhoton3_grandmotherPdgId;
   Int_t           GenPhoton3_genmatchindex;
   Int_t           GenPhoton3_patmatchindex;
   Char_t          GenPhoton3_isMatched;
   Char_t          GenPhoton3_isptdRmatched;
   Double_t        GenDiPhoton12_Minv;
   Double_t        GenDiPhoton12_qt;
   Double_t        GenDiPhoton12_deltaPhi;
   Double_t        GenDiPhoton12_deltaEta;
   Double_t        GenDiPhoton12_deltaR;
   Double_t        GenDiPhoton12_cosThetaStar;
   Double_t        GenDiPhoton12_cosThetaStar_old;
   Double_t        GenDiPhoton12_chiDiphoton;
   Bool_t          GenDiPhoton12_isEBEB;
   Bool_t          GenDiPhoton12_isEBEE;
   Bool_t          GenDiPhoton12_isEEEB;
   Bool_t          GenDiPhoton12_isEEEE;
   Double_t        GenDiPhoton13_Minv;
   Double_t        GenDiPhoton13_qt;
   Double_t        GenDiPhoton13_deltaPhi;
   Double_t        GenDiPhoton13_deltaEta;
   Double_t        GenDiPhoton13_deltaR;
   Double_t        GenDiPhoton13_cosThetaStar;
   Double_t        GenDiPhoton13_cosThetaStar_old;
   Double_t        GenDiPhoton13_chiDiphoton;
   Bool_t          GenDiPhoton13_isEBEB;
   Bool_t          GenDiPhoton13_isEBEE;
   Bool_t          GenDiPhoton13_isEEEB;
   Bool_t          GenDiPhoton13_isEEEE;
   Double_t        GenDiPhoton23_Minv;
   Double_t        GenDiPhoton23_qt;
   Double_t        GenDiPhoton23_deltaPhi;
   Double_t        GenDiPhoton23_deltaEta;
   Double_t        GenDiPhoton23_deltaR;
   Double_t        GenDiPhoton23_cosThetaStar;
   Double_t        GenDiPhoton23_cosThetaStar_old;
   Double_t        GenDiPhoton23_chiDiphoton;
   Bool_t          GenDiPhoton23_isEBEB;
   Bool_t          GenDiPhoton23_isEBEE;
   Bool_t          GenDiPhoton23_isEEEB;
   Bool_t          GenDiPhoton23_isEEEE;
   Double_t        GenTriPhoton_Minv;
   Double_t        GenTriPhoton_qt;
   Double_t        Photon1_pt;
   Double_t        Photon1_eta;
   Double_t        Photon1_phi;
   Double_t        Photon1_scEta;
   Double_t        Photon1_scPhi;
   Double_t        Photon1_rho;
   Double_t        Photon1_chargedHadIso03;
   Double_t        Photon1_neutralHadIso03;
   Double_t        Photon1_photonIso03;
   Double_t        Photon1_rhoCorChargedHadIso03;
   Double_t        Photon1_rhoCorNeutralHadIso03;
   Double_t        Photon1_rhoCorPhotonIso03;
   Double_t        Photon1_corPhotonIso03;
   Double_t        Photon1_hadTowerOverEm;
   Double_t        Photon1_hadronicOverEm;
   Double_t        Photon1_r9;
   Double_t        Photon1_r9_5x5;
   Double_t        Photon1_sigmaIetaIeta;
   Double_t        Photon1_sigmaIetaIeta5x5;
   Double_t        Photon1_sigmaEtaEta;
   Double_t        Photon1_sigmaIphiIphi;
   Double_t        Photon1_sigmaIphiIphi5x5;
   Double_t        Photon1_sigmaIetaIphi;
   Double_t        Photon1_sigmaIetaIphi5x5;
   Double_t        Photon1_maxEnergyXtal;
   Double_t        Photon1_iEta;
   Double_t        Photon1_iPhi;
   Double_t        Photon1_alphaHighPtID;
   Double_t        Photon1_kappaHighPtID;
   Double_t        Photon1_phoEAHighPtID;
   Double_t        Photon1_chEAegmID;
   Double_t        Photon1_nhEAegmID;
   Double_t        Photon1_phoEAegmID;
   Bool_t          Photon1_passEGMLooseID;
   Bool_t          Photon1_passEGMMediumID;
   Bool_t          Photon1_passEGMTightID;
   Bool_t          Photon1_isEB;
   Bool_t          Photon1_isEE;
   Bool_t          Photon1_isEBEtaGap;
   Bool_t          Photon1_isEBPhiGap;
   Bool_t          Photon1_isEERingGap;
   Bool_t          Photon1_isEEDeeGap;
   Bool_t          Photon1_isEBEEGap;
   Bool_t          Photon1_passElectronVeto;
   Bool_t          Photon1_passHTowOverE;
   Bool_t          Photon1_passChIso;
   Bool_t          Photon1_passCorPhoIso;
   Bool_t          Photon1_passSieie;
   Bool_t          Photon1_passHighPtID;
   Bool_t          Photon1_passChIsoDenom;
   Bool_t          Photon1_passCorPhoIsoDenom;
   Bool_t          Photon1_isFakeable;
   Bool_t          Photon1_isNumeratorObjCand;
   Bool_t          Photon1_isDenominatorObj;
   Bool_t          Photon1_isSaturated;
   Bool_t          Photon1_isMCTruthFake;
   Bool_t          Photon1_passSieieLowpTID;
   Bool_t          Photon1_passCorPhoIsoLowPTID;
   Bool_t          Photon1_passLowPtID;
   Bool_t          Photon1_hasConversionTracks;
   Double_t        Photon2_pt;
   Double_t        Photon2_eta;
   Double_t        Photon2_phi;
   Double_t        Photon2_scEta;
   Double_t        Photon2_scPhi;
   Double_t        Photon2_rho;
   Double_t        Photon2_chargedHadIso03;
   Double_t        Photon2_neutralHadIso03;
   Double_t        Photon2_photonIso03;
   Double_t        Photon2_rhoCorChargedHadIso03;
   Double_t        Photon2_rhoCorNeutralHadIso03;
   Double_t        Photon2_rhoCorPhotonIso03;
   Double_t        Photon2_corPhotonIso03;
   Double_t        Photon2_hadTowerOverEm;
   Double_t        Photon2_hadronicOverEm;
   Double_t        Photon2_r9;
   Double_t        Photon2_r9_5x5;
   Double_t        Photon2_sigmaIetaIeta;
   Double_t        Photon2_sigmaIetaIeta5x5;
   Double_t        Photon2_sigmaEtaEta;
   Double_t        Photon2_sigmaIphiIphi;
   Double_t        Photon2_sigmaIphiIphi5x5;
   Double_t        Photon2_sigmaIetaIphi;
   Double_t        Photon2_sigmaIetaIphi5x5;
   Double_t        Photon2_maxEnergyXtal;
   Double_t        Photon2_iEta;
   Double_t        Photon2_iPhi;
   Double_t        Photon2_alphaHighPtID;
   Double_t        Photon2_kappaHighPtID;
   Double_t        Photon2_phoEAHighPtID;
   Double_t        Photon2_chEAegmID;
   Double_t        Photon2_nhEAegmID;
   Double_t        Photon2_phoEAegmID;
   Bool_t          Photon2_passEGMLooseID;
   Bool_t          Photon2_passEGMMediumID;
   Bool_t          Photon2_passEGMTightID;
   Bool_t          Photon2_isEB;
   Bool_t          Photon2_isEE;
   Bool_t          Photon2_isEBEtaGap;
   Bool_t          Photon2_isEBPhiGap;
   Bool_t          Photon2_isEERingGap;
   Bool_t          Photon2_isEEDeeGap;
   Bool_t          Photon2_isEBEEGap;
   Bool_t          Photon2_passElectronVeto;
   Bool_t          Photon2_passHTowOverE;
   Bool_t          Photon2_passChIso;
   Bool_t          Photon2_passCorPhoIso;
   Bool_t          Photon2_passSieie;
   Bool_t          Photon2_passHighPtID;
   Bool_t          Photon2_passChIsoDenom;
   Bool_t          Photon2_passCorPhoIsoDenom;
   Bool_t          Photon2_isFakeable;
   Bool_t          Photon2_isNumeratorObjCand;
   Bool_t          Photon2_isDenominatorObj;
   Bool_t          Photon2_isSaturated;
   Bool_t          Photon2_isMCTruthFake;
   Bool_t          Photon2_passSieieLowpTID;
   Bool_t          Photon2_passCorPhoIsoLowPTID;
   Bool_t          Photon2_passLowPtID;
   Bool_t          Photon2_hasConversionTracks;
   Double_t        Photon3_pt;
   Double_t        Photon3_eta;
   Double_t        Photon3_phi;
   Double_t        Photon3_scEta;
   Double_t        Photon3_scPhi;
   Double_t        Photon3_rho;
   Double_t        Photon3_chargedHadIso03;
   Double_t        Photon3_neutralHadIso03;
   Double_t        Photon3_photonIso03;
   Double_t        Photon3_rhoCorChargedHadIso03;
   Double_t        Photon3_rhoCorNeutralHadIso03;
   Double_t        Photon3_rhoCorPhotonIso03;
   Double_t        Photon3_corPhotonIso03;
   Double_t        Photon3_hadTowerOverEm;
   Double_t        Photon3_hadronicOverEm;
   Double_t        Photon3_r9;
   Double_t        Photon3_r9_5x5;
   Double_t        Photon3_sigmaIetaIeta;
   Double_t        Photon3_sigmaIetaIeta5x5;
   Double_t        Photon3_sigmaEtaEta;
   Double_t        Photon3_sigmaIphiIphi;
   Double_t        Photon3_sigmaIphiIphi5x5;
   Double_t        Photon3_sigmaIetaIphi;
   Double_t        Photon3_sigmaIetaIphi5x5;
   Double_t        Photon3_maxEnergyXtal;
   Double_t        Photon3_iEta;
   Double_t        Photon3_iPhi;
   Double_t        Photon3_alphaHighPtID;
   Double_t        Photon3_kappaHighPtID;
   Double_t        Photon3_phoEAHighPtID;
   Double_t        Photon3_chEAegmID;
   Double_t        Photon3_nhEAegmID;
   Double_t        Photon3_phoEAegmID;
   Bool_t          Photon3_passEGMLooseID;
   Bool_t          Photon3_passEGMMediumID;
   Bool_t          Photon3_passEGMTightID;
   Bool_t          Photon3_isEB;
   Bool_t          Photon3_isEE;
   Bool_t          Photon3_isEBEtaGap;
   Bool_t          Photon3_isEBPhiGap;
   Bool_t          Photon3_isEERingGap;
   Bool_t          Photon3_isEEDeeGap;
   Bool_t          Photon3_isEBEEGap;
   Bool_t          Photon3_passElectronVeto;
   Bool_t          Photon3_passHTowOverE;
   Bool_t          Photon3_passChIso;
   Bool_t          Photon3_passCorPhoIso;
   Bool_t          Photon3_passSieie;
   Bool_t          Photon3_passHighPtID;
   Bool_t          Photon3_passChIsoDenom;
   Bool_t          Photon3_passCorPhoIsoDenom;
   Bool_t          Photon3_isFakeable;
   Bool_t          Photon3_isNumeratorObjCand;
   Bool_t          Photon3_isDenominatorObj;
   Bool_t          Photon3_isSaturated;
   Bool_t          Photon3_isMCTruthFake;
   Bool_t          Photon3_passSieieLowpTID;
   Bool_t          Photon3_passCorPhoIsoLowPTID;
   Bool_t          Photon3_passLowPtID;
   Bool_t          Photon3_hasConversionTracks;
   Double_t        DiPhoton12_Minv;
   Double_t        DiPhoton12_qt;
   Double_t        DiPhoton12_deltaPhi;
   Double_t        DiPhoton12_deltaEta;
   Double_t        DiPhoton12_deltaR;
   Double_t        DiPhoton12_cosThetaStar;
   Double_t        DiPhoton12_cosThetaStar_old;
   Double_t        DiPhoton12_chiDiphoton;
   Bool_t          DiPhoton12_isEBEB;
   Bool_t          DiPhoton12_isEBEE;
   Bool_t          DiPhoton12_isEEEB;
   Bool_t          DiPhoton12_isEEEE;
   Double_t        DiPhoton13_Minv;
   Double_t        DiPhoton13_qt;
   Double_t        DiPhoton13_deltaPhi;
   Double_t        DiPhoton13_deltaEta;
   Double_t        DiPhoton13_deltaR;
   Double_t        DiPhoton13_cosThetaStar;
   Double_t        DiPhoton13_cosThetaStar_old;
   Double_t        DiPhoton13_chiDiphoton;
   Bool_t          DiPhoton13_isEBEB;
   Bool_t          DiPhoton13_isEBEE;
   Bool_t          DiPhoton13_isEEEB;
   Bool_t          DiPhoton13_isEEEE;
   Double_t        DiPhoton23_Minv;
   Double_t        DiPhoton23_qt;
   Double_t        DiPhoton23_deltaPhi;
   Double_t        DiPhoton23_deltaEta;
   Double_t        DiPhoton23_deltaR;
   Double_t        DiPhoton23_cosThetaStar;
   Double_t        DiPhoton23_cosThetaStar_old;
   Double_t        DiPhoton23_chiDiphoton;
   Bool_t          DiPhoton23_isEBEB;
   Bool_t          DiPhoton23_isEBEE;
   Bool_t          DiPhoton23_isEEEB;
   Bool_t          DiPhoton23_isEEEE;
   Double_t        TriPhoton_Minv;
   Double_t        TriPhoton_qt;
   Bool_t          isGood;
   Int_t           nPV;

   // List of branches
   TBranch        *b_Event;   //!
   TBranch        *b_GenPhoton1;   //!
   TBranch        *b_GenPhoton2;   //!
   TBranch        *b_GenPhoton3;   //!
   TBranch        *b_GenDiPhoton12;   //!
   TBranch        *b_GenDiPhoton13;   //!
   TBranch        *b_GenDiPhoton23;   //!
   TBranch        *b_GenTriPhoton;   //!
   TBranch        *b_Photon1;   //!
   TBranch        *b_Photon2;   //!
   TBranch        *b_Photon3;   //!
   TBranch        *b_DiPhoton12;   //!
   TBranch        *b_DiPhoton13;   //!
   TBranch        *b_DiPhoton23;   //!
   TBranch        *b_TriPhoton;   //!
   TBranch        *b_isGood;   //!
   TBranch        *b_nPV;   //!

   TempSignalStudies(TTree *tree=0);
   virtual ~TempSignalStudies();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef TempSignalStudies_cxx
TempSignalStudies::TempSignalStudies(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f || !f->IsOpen()) {
         f = new TFile("Memory Directory");
      }
      f->GetObject("demo/fTree",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("demo/fTree","");
      chain->Add("/uscms/home/cuperez/nobackup/tribosons/CMSSW_10_6_12/src/out_GGGJets_Pt-15_13TeV-sherpa_evt17999.root/demo/fTree");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

TempSignalStudies::~TempSignalStudies()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TempSignalStudies::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TempSignalStudies::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void TempSignalStudies::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Event", &Event_run, &b_Event);
   fChain->SetBranchAddress("GenPhoton1", &GenPhoton1_pt, &b_GenPhoton1);
   fChain->SetBranchAddress("GenPhoton2", &GenPhoton2_pt, &b_GenPhoton2);
   fChain->SetBranchAddress("GenPhoton3", &GenPhoton3_pt, &b_GenPhoton3);
   fChain->SetBranchAddress("GenDiPhoton12", &GenDiPhoton12_Minv, &b_GenDiPhoton12);
   fChain->SetBranchAddress("GenDiPhoton13", &GenDiPhoton13_Minv, &b_GenDiPhoton13);
   fChain->SetBranchAddress("GenDiPhoton23", &GenDiPhoton23_Minv, &b_GenDiPhoton23);
   fChain->SetBranchAddress("GenTriPhoton", &GenTriPhoton_Minv, &b_GenTriPhoton);
   fChain->SetBranchAddress("Photon1", &Photon1_pt, &b_Photon1);
   fChain->SetBranchAddress("Photon2", &Photon2_pt, &b_Photon2);
   fChain->SetBranchAddress("Photon3", &Photon3_pt, &b_Photon3);
   fChain->SetBranchAddress("DiPhoton12", &DiPhoton12_Minv, &b_DiPhoton12);
   fChain->SetBranchAddress("DiPhoton13", &DiPhoton13_Minv, &b_DiPhoton13);
   fChain->SetBranchAddress("DiPhoton23", &DiPhoton23_Minv, &b_DiPhoton23);
   fChain->SetBranchAddress("TriPhoton", &TriPhoton_Minv, &b_TriPhoton);
   fChain->SetBranchAddress("isGood", &isGood, &b_isGood);
   fChain->SetBranchAddress("nPV", &nPV, &b_nPV);
   Notify();
}

Bool_t TempSignalStudies::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TempSignalStudies::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TempSignalStudies::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TempSignalStudies_cxx
