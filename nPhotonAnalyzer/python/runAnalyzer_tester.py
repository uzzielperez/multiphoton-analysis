#!/usr/bin/env python
"""Process several files locally"""
import subprocess
import json
import os
import re

DATASET = []

xsecdict = {}
nevtsdict = {}

islocal = 'False'
isDAS = 'True' # Centrally Produced
# inputFile_ = '/store/mc/RunIIFall17MiniAODv2/GGJets_M-200To500_Pt-50_13TeV-sherpa/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/00000/FCBF2E5A-DC44-E811-984D-B496910A9790.root'
# inputFile_ = 'file:/uscms/home/cuperez/nobackup/tribosons/triphoton/CMSSW_10_3_0/src/GGJets_M-200To500_Pt-50_13TeV-sherpa_sample.root'
# outputFile_ = "GGJets_M-200To500_Pt-50_13TeV-sherpa_sampleOUT.root"
# nevts = 69803 # Found in DAS

#inputFile_ = 'file:/uscms/home/cuperez/nobackup/tribosons/CMSSW_10_2_8/src/GGGJets_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_sample.root'
#inputFile_ = 'file:/uscms_data/d3/cuperez/e2e/CMSSW_9_4_8/src/MY/PROJECT/test/sherpa_EEA_2j_pT20_13TeV_MASTER_cff_py_GEN.root'
#inputFile_ = 'file:/uscms/home/cuperez/nobackup/tribosons/CMSSW_10_2_8/src/GGGJets_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_spring.root'
inputFile_ = 'file:/uscms_data/d3/cuperez/e2e/CMSSW_9_4_8/src/MY/PROJECT/test/AAA_2j_pT20_13TeV/sherpa_AAA_2j_pT20_13TeV_MASTER_cff_py_GEN.root'
#outputFile_ = "GGGJets_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_sampleOUT_matching_minDRstore_diff.root"
#outputFile_ = "GGGJets_pT20_20_20_13TeV.root"
outputFile_ = "AAA_2j_pT20_13TeV_ntuples.root"
#nevts = 2118
nevts = 13433

print 'Processing ', inputFile_
cmssw_base = os.getenv("CMSSW_BASE")
templatefile = cmssw_base + "/src/multiphoton-analysis/nPhotonAnalyzer/python/cmsRun_cfg_tester.py"
ConfigFile = "cmsRun_cfg_DAStestinput.py"
xsec = 1.00 # Default

genParticles_ = "prunedGenParticles" # genParticles for GEN only
genInfo = "SIM"
# maxEvents_ = 10
idmode_ = "NOid"
# idmode_ = "highPTID"
# idmode_ = "MEDIUM" # LOOSE, MEDIUM or TIGHT
# idmode_ = "LOOSE"
# idmode_ = "TIGHT"


s = open(templatefile).read()
s = s.replace('ISLOCAL', islocal)
s = s.replace('ISDAS', isDAS)
s = s.replace('INPUTFILE', inputFile_)
s = s.replace('CROSSSECTION', str(xsec))
s = s.replace('NEVTS', str(nevts))
s = s.replace('GENPARTICLES', str(genParticles_))
s = s.replace('GENINFO', str(genInfo))
s = s.replace('OUTPUTFILE', str(outputFile_))
s = s.replace('IDMODE', str(idmode_))
f = open(ConfigFile, 'w')
f.write(s)
f.close()

print "Wrote local configuration file ", ConfigFile

cmd = "cmsRun " + ConfigFile
# cmd="cmsRun %s inputFiles=%s maxEvents=%d skipEvents=%d outputFile=%s"%(cfg,inputFiles_,maxEvents_,skipEvents_, outputFile_)
# cmd="cmsRun %s maxEvents=%d"%(ConfigFile, maxEvents_)

os.system(cmd)
