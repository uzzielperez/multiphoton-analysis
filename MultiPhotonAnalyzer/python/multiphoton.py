import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/afs/cern.ch/user/c/ciperez/Generation/CMSSW_9_3_8/src/ADDGravToGG_NED-4_LambdaT-4000_M-500_13TeV-pythia8_cff_py_GEN.root'
    )
)


process.TFileService = cms.Service("TFileService",
                    fileName = cms.string("TestADDPythia.root")
                                                )

process.demo = cms.EDAnalyzer('MultiPhotonAnalyzer', 
    genparticles = cms.InputTag("genParticles"), 
    # Flags
    isGEN = cms.bool(True)
    )

process.p = cms.Path(process.demo)
