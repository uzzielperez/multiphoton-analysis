#!/usr/bin/env python
import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
from os.path import basename
import os
import sys
from string import Template


DATASETS = []
PATH      = '/uscms/home/cuperez/nobackup/CMSSW_8_0_25/src/'
isMC      = True

DATASETS.append('Unparticles_du1p06_LambdaU-0p4_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du1p06_LambdaU-1p0_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du1p06_LambdaU-4p0_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du1p06_LambdaU-15000p0_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du1p06_LambdaU-100000p0_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du1p09_LambdaU-0p4_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du1p09_LambdaU-1p0_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du1p09_LambdaU-4p0_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du1p09_LambdaU-15000p0_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du1p09_LambdaU-100000p0_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du2p1_LambdaU-0p4_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du2p1_LambdaU-1p0_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du2p1_LambdaU-4p0_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du2p1_LambdaU-15000p0_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')
DATASETS.append('Unparticles_du2p1_LambdaU-100000p0_TuneCUEP8M1_13TeV_pythia8_py_GEN.root')



for dataset in DATASETS:
    print dataset
    INFILE    = PATH + dataset
    inputFile = 'file:%s' %(INFILE)
    outName = 'Test%s' %(dataset)
    #print 'processing ', inputFile
    print 'Writing output to file ', outName

    # Dictionary
    d={'inFile':dataset}

    Config_file = "multi_cfg.py"
    filein = open('/uscms_data/d3/cuperez/CMSSW_8_0_25/src/multiphoton-analysis/nPhotonAnalyzer/python/multiConfFile_cfg.py')
    multiCfg = open(Config_file, "w+")
    src    = Template(filein.read())
    sub    = src.substitute(d)
    multiCfg.write(sub)
    filein.close()
    print "Generated %s" %(Config_file)
    os.system('cmsRun %s' %(Config_file))
