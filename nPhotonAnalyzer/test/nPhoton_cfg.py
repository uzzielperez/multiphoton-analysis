import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
from os.path import basename
import os
import sys

#------------------------------------------
isMC                = True
isPythia8gen        = False
isSherpaDiphoton    = True
isReMINIAOD         = False

options = VarParsing ('python')

options.register('nEventsSample',
                 100,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "Total number of events in dataset for event weight calculation.")
## 'maxEvents' is already registered by the Framework, changing default value
options.setDefault('maxEvents', 100)

options.parseArguments()

outName = options.outputFile
print "Default output name: " + outName
if "output" in outName: # if an input file name is specified, event weights can be determined
    outName = "out_" + basename(options.inputFiles[0])
    print "Output root file name: " + outName
else:
    options.inputFiles = 'file:/uscms/home/cuperez/nobackup/CMSSW_8_0_25/src/ADDGravToGG_NED-4_LambdaT-4000_13TeV-pythia8_cff_py_GEN.root'
    outName = 'TestSummer16ADDGravToGG_NED4-4_LambdaT-4000_13TeV-pythia8.root'
#    outName = "ExoDiphotonAnalyzer.root"

# to avoid processing with an incorrect globalTag
globalTag ='notset'

# override options for MC
if isMC:
    version = os.getenv("CMSSW_VERSION")
    if "CMSSW_8" in version:
        if "Spring16" in outName:
            globalTag = '80X_mcRun2_asymptotic_2016_miniAODv2'
        if "Summer16" in outName:
            #globalTag = '80X_mcRun2_asymptotic_2016_TrancheIV_v6'
            # the previous tag should only be used when to process
            # samples intended to match data previous to the
            # 03Feb2017 re-miniAOD
            globalTag = '80X_mcRun2_asymptotic_2016_TrancheIV_v8'
    elif "CMSSW_7" in version:
        globalTag = '76X_mcRun2_asymptotic_v12'
    else:
        print "Could not determine appropriate MC global tag from filename"
        sys.exit()
    JEC = cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute'])


process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.suppressWarning.append('nPhotonAnalyzer')

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.options.allowUnscheduled = cms.untracked.bool(True)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( options.maxEvents ) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        #'file:myfile.root'
        options.inputFiles
    )
)

# for global tag
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = globalTag
print "Using global tag: " + globalTag

# geometry for saturation
process.load("Configuration.StandardSequences.GeometryDB_cff")

process.TFileService = cms.Service("TFileService",
                    fileName = cms.string(outName)
)

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# Setup VID for EGM ID
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
switchOnVIDPhotonIdProducer(process, DataFormat.MiniAOD)
# define which IDs we want to produce
my_id_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Spring15_25ns_V1_cff']
#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)

if isPythia8gen:
	inTag = "genParticles"
elif isSherpaDiphoton:
    inTag = "prunedGenParticles"
else:
    print "cannot determine proper input type"

process.demo = cms.EDAnalyzer('nPhotonAnalyzer',
        #isPythia8gen        = False
        #Diphoton    = True
        genparticles = cms.InputTag(inTag),
        # genParticle tag
        #genParticlesMiniAOD = cms.InputTag("prunedGenParticles"),
        #photonsMiniAOD = cms.InputTag("slimmedPhotons"),
        minPhotonPt = cms.double(75.),

        # vertex tag
        #vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
        # beam spot tag
        #beamSpot = cms.InputTag("offlineBeamSpot", "", "RECO"),
        # ak4 jets
        #jetsMiniAOD = cms.InputTag(jetLabel),
        #jetPtThreshold = cms.double(30.),
        #jetEtaThreshold = cms.double(2.5),
        # rho tag
        #rho = cms.InputTag("fixedGridRhoAll"),
        # EGM eff. areas
        #effAreaChHadFile = cms.FileInPath("RecoEgamma/PhotonIdentification/data/Spring15/effAreaPhotons_cone03_pfChargedHadrons_25ns_NULLcorrection.txt"),
        #effAreaNeuHadFile = cms.FileInPath("RecoEgamma/PhotonIdentification/data/Spring15/effAreaPhotons_cone03_pfNeutralHadrons_25ns_90percentBased.txt"),
        #effAreaPhoFile = cms.FileInPath("RecoEgamma/PhotonIdentification/data/Spring15/effAreaPhotons_cone03_pfPhotons_25ns_90percentBased.txt"),
        # EGM ID decisions
        phoLooseIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-25ns-V1-standalone-loose"),
        phoMediumIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-25ns-V1-standalone-medium"),
        phoTightIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-25ns-V1-standalone-tight"),
        # gen event info
        genInfo = cms.InputTag("generator", "", "SIM"),
        # output file name
        outputFile = cms.string(outName),
        # number of events in the sample (for calculation of event weights)
        nEventsSample = cms.uint32(options.nEventsSample),
        isMC = cms.bool(isMC),
        isPythia8gen = cms.bool(isPythia8gen),
        isSherpaDiphoton = cms.bool(isSherpaDiphoton),
        #isClosureTest = cms.bool(False),
        #isReMINIAOD = cms.bool(isReMINIAOD),
        isolationConeR = cms.double(0.3)

)

process.p = cms.Path(process.demo)
