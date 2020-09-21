void makeClass(){
  TChain chain("demo/fTree");
  chain.Add("/uscms/home/cuperez/nobackup/tribosons/CMSSW_10_6_12/src/out_GGGJets_Pt-15_13TeV-sherpa_evt17999.root");
  chain.MakeClass("TempSignalStudies");
  return 0;
}
