import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
from os.path import basename
import os
import sys

#Configure
isMC    = True
islocal = True

if islocal:
    inputFile = '/afs/cern.ch/user/c/ciperez/Generation/CMSSW_9_3_8/src/ADDGravToGG_NED-4_LambdaT-4000_M-500_13TeV-pythia8_cff_py_GEN.root'
    inFile = 'file:'+ inputFile


process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:myfile.root'
        inFile
    )
)


process.TFileService = cms.Service("TFileService",
                    fileName = cms.string("TestADDPythia.root")
)

process.demo = cms.EDAnalyzer('nPhotonAnalyzer',
        genparticles = cms.InputTag("genParticles")
)

process.p = cms.Path(process.demo)
