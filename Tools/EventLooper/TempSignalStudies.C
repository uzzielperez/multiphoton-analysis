#define TempSignalStudies_cxx
#include "TempSignalStudies.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void TempSignalStudies::Loop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TH1D* genPho1Pt = new TH1D("genPho1Pt", "", 20, 0, 100);
   TH1D* genPho2Pt = new TH1D("genPho2Pt", "", 20, 0, 100);
   TH1D* genPho3Pt = new TH1D("genPho3Pt", "", 20, 0, 100);
   TH1D* genTriPhotonMinv = new TH1D("genTriPhotonMinv", "", 20, 0, 300);

   TH1D* Pho1PtLoose = new TH1D("Pho1PtLoose", "", 20, 0, 100);
   TH1D* Pho2PtLoose = new TH1D("Pho2PtLoose", "", 20, 0, 100);
   TH1D* Pho3PtLoose = new TH1D("Pho3PtLoose", "", 20, 0, 100);

   genPho1Pt->Sumw2();
   genPho2Pt->Sumw2();
   genPho3Pt->Sumw2();
   genTriPhotonMinv->Sumw2();

   Pho1PtLoose->Sumw2();
   Pho2PtLoose->Sumw2();
   Pho3PtLoose->Sumw2();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if (jentry%1000 == 0) cout << "Number of processed events: " << jentry << endl;

      genPho1Pt->Fill(GenPhoton1_pt, Event_weightAll);
      genPho2Pt->Fill(GenPhoton2_pt, Event_weightAll);
      genPho3Pt->Fill(GenPhoton3_pt, Event_weightAll);
      genTriPhotonMinv->Fill(GenTriPhoton_Minv, Event_weightAll);

      if (Photon1_passEGMLooseID) Pho1PtLoose->Fill(Photon1_pt, Event_weightAll);
      if (Photon2_passEGMLooseID) Pho2PtLoose->Fill(Photon2_pt, Event_weightAll);
      if (Photon3_passEGMLooseID) Pho3PtLoose->Fill(Photon3_pt, Event_weightAll);

   }
   TFile file_out("TriphotonSignalKinematics.root", "RECREATE");
   genPho1Pt->Write();
   genPho2Pt->Write();
   genPho3Pt->Write();
   genTriPhotonMinv->Write();

   Pho1PtLoose->Write();
   Pho2PtLoose->Write();
   Pho3PtLoose->Write();
}
