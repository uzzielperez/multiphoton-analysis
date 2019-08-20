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
inputFile_ = 'file:/uscms/home/cuperez/nobackup/tribosons/triphoton/CMSSW_10_3_0/src/GGJets_M-200To500_Pt-50_13TeV-sherpa_sample.root'
outputFile_ = "GGJets_M-200To500_Pt-50_13TeV-sherpa_sampleOUT.root"

print 'Processing ', inputFile_
cmssw_base = os.getenv("CMSSW_BASE")
templatefile = cmssw_base + "/src/multiphoton-analysis/nPhotonAnalyzer/python/cmsRun_cfg_tester.py"
ConfigFile = "cmsRun_cfg_DAStestinput.py"
xsec = 1.00 # Default
nevts = 69803 # Found in DAS
genParticles_ = "prunedGenParticles" # genParticles for GEN only
genInfo = "SIM"

s = open(templatefile).read()
s = s.replace('ISLOCAL', islocal)
s = s.replace('ISDAS', isDAS)
s = s.replace('INPUTFILE', inputFile_)
s = s.replace('CROSSSECTION', str(xsec))
s = s.replace('NEVTS', str(nevts))
s = s.replace('GENPARTICLES', str(genParticles_))
s = s.replace('GENINFO', str(genInfo))
s = s.replace('OUTPUTFILE', str(outputFile_))
f = open(ConfigFile, 'w')
f.write(s)
f.close()

print "Wrote local configuration file ", ConfigFile

cmd = "cmsRun " + ConfigFile
os.system(cmd)
