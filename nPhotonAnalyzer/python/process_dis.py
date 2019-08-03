#!/usr/bin/env python
"""Process several files locally"""
import subprocess
import json
import os
import re



doAAAsherpa = True

DATASET = []

xsecdict = {}
nevtsdict = {}

if doAAAsherpa:
    #DATASET.append("sherpa_AAA_0j_pT20_13TeV_MASTER_cff_py_GEN.root")
    #DATASET.append("sherpa_AAA_1j_pT20_13TeV_MASTER_cff_py_GEN.root")
    DATASET.append("sherpa_AAA_2j_pT20_13TeV_MASTER_cff_py_GEN.root")
    # Cross-section hardcoded in pb
    xsecdict_temp = {
        "AAA_0j_pT20_13TeV" : 2.535e-02, #25.3500,    #  2.535e-02 +- 3.505e-04 pb
        "AAA_1j_pT20_13TeV" : 4.446e-02, #44.824, # 0.0446084 pb +- ( 0.000445437
	"AAA_2j_pT20_13TeV" : 5.722e-02, # +- 5.721e-04 pb
    }
    xsecdict.update(xsecdict_temp)

    nevtsdict_temp = {
        "AAA_0j_pT20_13TeV" : 5000,
        "AAA_1j_pT20_13TeV" : 10000,
  	"AAA_2j_pT20_13TeV" : 10000, 
     }
    xsecdict.update(xsecdict_temp)
    nevtsdict.update(nevtsdict_temp)
    keypattern = "sherpa_([^(]*)_MASTER_cff_py_GEN.root"

for dset in DATASET:

    matchkey = re.findall(keypattern, dset)
    key = matchkey[0]
    xsec, nevts = xsecdict[key], nevtsdict[key]

    print 'Processing ', dset
    cmssw_base = os.getenv("CMSSW_BASE")
    templatefile = cmssw_base + "/src/multiphoton-analysis/nPhotonAnalyzer/python/cmsRun_cfg_template.py"
    ConfigFile = "cmsRun_cfg_" + dset + ".py"

    s = open(templatefile).read()
    s = s.replace('DATASETNAME', dset)
    s = s.replace('CROSSSECTION', str(xsec))
    s = s.replace('NEVTS', str(nevts))
    f = open(ConfigFile, 'w')
    f.write(s)
    f.close()
    print "Wrote local configuration file ", ConfigFile

    cmd = "cmsRun " + ConfigFile
    os.system(cmd)
    print "Running ", dset
