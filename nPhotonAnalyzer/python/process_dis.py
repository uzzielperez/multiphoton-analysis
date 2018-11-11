#!/usr/bin/env python
"""Process several files locally"""
import subprocess
import json
import os

# Update with CMSSW_VERSION
globalTag           = '80X_mcRun2_asymptotic_2016_miniAODv2'

DATASET = []
DATASET.append("STest1p9Unp1000p0_M_2000_py_GEN.root")
DATASET.append("STest1p9Unp1000p0_M_500-2000_py_GEN.root")
DATASET.append("STest1p9Unp1250p0_M_2000_py_GEN.root")
DATASET.append("STest1p9Unp1250p0_M_500-2000_py_GEN.root")
DATASET.append("STest1p9Unp1500p0_M_2000_py_GEN.root")
DATASET.append("STest1p9Unp1500p0_M_500-2000_py_GEN.root")
DATASET.append("STest1p9Unp1750p0_M_2000_py_GEN.root")
DATASET.append("STest1p9Unp1750p0_M_500-2000_py_GEN.root")
DATASET.append("STest1p9Unp2000p0_M_2000_py_GEN.root")
DATASET.append("STest1p9Unp2000p0_M_500-2000_py_GEN.root")
DATASET.append("STest1p9Unp2500p0_M_2000_py_GEN.root")
DATASET.append("STest1p9Unp2500p0_M_500-2000_py_GEN.root")
DATASET.append("STest1p9Unp3000p0_M_2000_py_GEN.root")
DATASET.append("STest1p9Unp3000p0_M_500-2000_py_GEN.root")
DATASET.append("STest1p9Unp3500p0_M_2000_py_GEN.root")
DATASET.append("STest1p9Unp3500p0_M_500-2000_py_GEN.root")
DATASET.append("STest1p9Unp4000p0_M_2000_py_GEN.root")
DATASET.append("STest1p9Unp4000p0_M_500-2000_py_GEN.root")
DATASET.append("STest1p9Unp4500p0_M_2000_py_GEN.root")
DATASET.append("STest1p9Unp4500p0_M_500-2000_py_GEN.root")
DATASET.append("STest1p9Unp5500p0_M_2000_py_GEN.root")
DATASET.append("STest1p9Unp5500p0_M_500-2000_py_GEN.root")

PATH      = '/uscms/home/cuperez/nobackup/CMSSW_8_0_25/src/'

for dset in DATASET:
    # inF = dset
    # INFILE    = PATH + inF
    # inputFile = 'file:%s' %(INFILE)
    # outName = 'Test%s' %(inF)

    print 'Processing ', dset
    cmssw_base = os.getenv("CMSSW_BASE")
    templatefile = cmssw_base + "/src/multiphoton-analysis/nPhotonAnalyzer/python/cmsRun_cfg_template.py"
    ConfigFile = "cmsRun_cfg_" + dset + ".py"

    s = open(templatefile).read()
    s = s.replace('DATASETNAME', dset)
    f = open(ConfigFile, 'w')
    f.write(s)
    f.close()
    print "Wrote local configuration file ", ConfigFile

    cmd = "cmsRun " + ConfigFile
    os.system(cmd)
    print "Running ", dset
