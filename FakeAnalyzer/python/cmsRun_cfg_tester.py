# runtime options

import FWCore.ParameterSet.Config as cms
from RecoEgamma.EgammaTools.EgammaPostRecoTools import setupEgammaPostRecoSeq
from FWCore.ParameterSet.VarParsing import VarParsing
import os
import importlib
submit_utils = importlib.import_module("multiphoton-analysis.CommonClasses.submit_utils")

#Configure
isMC           = True

islocal        = bool(ISLOCAL)
isDAS          = bool(ISDAS)

print "Logical FileName Provided: "
# inputFile = '/store/mc/RunIISummer16MiniAODv2/ADDGravToGG_MS-4000_NED-4_KK-1_M-1000To2000_13TeV-sherpa/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/6C12F491-0BB7-E611-B418-0025904FE658.root'
# #Provide Logical Filename
# #inputFile = '/store/....'
# outName = "ADDGravToGG_MS-4000_NED-4_KK-1_M-1000To2000_13TeV-sherpa.root"
inputFile = 'INPUTFILE'
outName = 'OUTPUTFILE'

#------------------------------------------
print 'Configuration file Run with the following settings: '
print 'isMC = ', isMC
if islocal:
    print 'Pythia GEN'
if isDAS:
    print 'DAS GEN'
print 'processing ', inputFile
print 'Writing output to file ', outName
#------------------------------------------

options = VarParsing ('python')
options.parseArguments()

outName = options.outputFile
print("Default output name: " + outName)
if "output" in outName: # if an input file name is specified, event weights can be determined
    outName = "out_" + basename(options.inputFiles[0])
    print("Output root file name: " + outName)

else:
    options.inputFiles = "file:GGJets_M-1000To2000_Pt-50_13TeV-sherpa.root"

globalTag = submit_utils.get_global_tag(outName)

reapplyJEC = False #Jet Energy Corrections
jetCollection = "slimmedJets"
JEC = cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual'])
if reapplyJEC:
    jetCollection = "updatedPatJetsUpdatedJEC"
isMC = True
# data always has "Run201" in its filename
if "Run201" in outName:
    isMC = False
# override options for MC
if isMC:
    JEC = cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute'])

process = cms.Process("ExoDiPhoton")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.options.allowUnscheduled = cms.untracked.bool(True)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( options.maxEvents ) )

# for input file
process.source = cms.Source(
    "PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        # options.inputFiles
        inputFile
        )
    )

# for global tag
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = globalTag
print("Using global tag: " + globalTag)

# geometry for saturation
process.load("Configuration.StandardSequences.GeometryDB_cff")

# for output file
process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("FakeAnalyzer.root")
    )

## update AK4PFchs jet collection in MiniAOD JECs
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

updateJetCollection(
   process,
   jetSource = cms.InputTag('slimmedJets'),
   labelName = 'UpdatedJEC',
   jetCorrections = ('AK4PFchs', JEC, 'None')  # Do not forget 'L2L3Residual' on data!
)

# summary of information needed for e/gamma corrections
egm_info = submit_utils.egamma_info(outName)
setupEgammaPostRecoSeq(process,
                       applyEnergyCorrections=True,
                       applyVIDOnCorrectedEgamma=True,
                       runVID=True,
                       runEnergyCorrections=True,
                       era=egm_info['era'])

# analyzer and inputs
process.demo = cms.EDAnalyzer(
    'FakeAnalyzer',
    # photon tag
    photonsMiniAOD = cms.InputTag("slimmedPhotons"),
    # ak4 jets
    jetsMiniAOD = cms.InputTag(jetCollection),
    jetPtThreshold = cms.double(30.),
    jetEtaThreshold = cms.double(2.5),
    # rho tag
    rho = cms.InputTag("fixedGridRhoAll"),
    # EGM eff. areas
    effAreaChHadFile = cms.FileInPath(egm_info['effAreaChHad']),
    effAreaNeuHadFile = cms.FileInPath(egm_info['effAreaNeuHad']),
    effAreaPhoFile = cms.FileInPath(egm_info['effAreaPhoHad']),
    # EGM ID decisions
    phoLooseIdMap = cms.InputTag("egmPhotonIDs:" + egm_info['loosePhoId']),
    phoMediumIdMap = cms.InputTag("egmPhotonIDs:" + egm_info['mediumPhoId']),
    phoTightIdMap = cms.InputTag("egmPhotonIDs:" + egm_info['tightPhoId']),
    # output file name
    outputFile = cms.string(outName),
    isReMINIAOD = cms.bool(True)
    )

if reapplyJEC:
    process.p = cms.Path(process.egammaPostRecoSeq * process.patJetCorrFactorsUpdatedJEC * process.updatedPatJetsUpdatedJEC * process.demo)
else:
    process.p = cms.Path(process.egammaPostRecoSeq * process.demo)
