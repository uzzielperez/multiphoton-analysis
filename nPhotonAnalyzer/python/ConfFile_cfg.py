import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
from os.path import basename
import os
import sys

#Configure
isMC                = True
isPythia8gen        = True
isSherpaDiphoton    = False
islocal             = True

# Update with CMSSW_VERSION
globalTag           = '80X_mcRun2_asymptotic_2016_miniAODv2'

if islocal:
    PATH      = '/uscms/home/cuperez/nobackup/CMSSW_8_0_25/src/'
    #PATH      = '/uscms/home/cuperez/nobackup/CMSSW_9_3_8/src/'


    #inF = 'Unparticles_du106_15TeV_pythia8_py_GEN.root'
    #inF = 'Unparticles_du2x1_15TeV_pythia8_py_GEN.root'
    #inF = 'Unparticles_du109_15TeV_pythia8_py_GEN.root'

    #inF = 'Unparticles_du2x1_TuneCUEP8M1_15TeV_pythia8_py_GEN.root'
    #inF = 'Unparticles_du109_TuneCUEP8M1_15TeV_pythia8_py_GEN.root'
    #inF = 'Unparticles_du106_TuneCUEP8M1_15TeV_pythia8_py_GEN.root'
    # RS Graviton
    #inF = 'RSGravitonToGammaGamma_kMpl02_M_750_TuneCUEP8M1_13TeV_pythia8_cfi_py_GEN.root'
    inF = 'RSGravitonToGammaGamma_kMpl02_M_750_TuneCUEP8M1_13TeV_pythia8_cfi_py_GEN.root'

    # Heavy Higgs
    #inF = 'GluGluSpin0ToGammaGamma_W_5p6_M_750_TuneCUEP8M1_13TeV_pythia8_cfi_py_GEN.root'

    # RS Graviton
    #inF = 'RSGravitonToGammaGamma_kMpl02_M_750_TuneCUEP8M1_13TeV_pythia8_cfi_py_GEN.root'

    # Hewett Test
    #inF       = 'ADDGravToGG_NED-2_LambdaT-3572_M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF       = 'ADDGravToGG_NED-2_LambdaT-4478_M-500-13TeV-pythia8_cff_py_GEN.root'

    #inF      = 'ADDGravToGG_NED-4_LambdaT-4000_13TeV-pythia8_cff_py_GEN.root'
    #inF       = 'ADDGravToGG_NED-4_LambdaT-4000_13TeV-pythia8_cff_py_GEN.root'
    #inF       = 'ADDGravToGG_NED-4_MD-LambdaT-4000_13TeV-pythia8_cff_py_GEN.root'
    #noMD
    #inF       = 'ADDGravToGG_NED-4_LambdaT-10000_M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF       = 'ADDGravToGG_NED-4_LambdaT-4000_M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF       = 'ADDGravToGG_NED-4_LambdaT-5000_M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF       = 'ADDGravToGG_NED-4_LambdaT-7000_M-500-13TeV-pythia8_cff_py_GEN.root'

    #wMD
    #inF       = 'ADDGravToGG_NED-4_MD-1128_LambdaT-4000_M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF       = 'ADDGravToGG_NED-4_MD-1410_LambdaT-4000_M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF       = 'ADDGravToGG_NED-4_MD-1974_LambdaT-4000_M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF       = 'ADDGravToGG_NED-4_MD-2820_LambdaT-4000_M-500-13TeV-pythia8_cff_py_GEN.root'

    #MD Extremes Knob
    #inF        = 'ADDGravToGG_NED-4_MD-100_LambdaT-4000_M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF        = 'ADDGravToGG_NED-4_MD-10000_LambdaT-4000_M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF        = 'ADDGravToGG_NED-4_MD-11000_LambdaT-4000_M-500-13TeV-pythia8_cff_py_GEN.root'
    # Use MD-1128 LambdaT-4000

    # Cutoff Comparisons
    #inF        = 'ADDGravToGG_NED-4_LambdaT-4000_CO-0_M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF        = 'ADDGravToGG_NED-4_LambdaT-4000_CO-1_M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF        = 'ADDGravToGG_NED-4_LambdaT-4000_CO-2_M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF        = 'ADDGravToGG_NED-4_LambdaT-4000_CO-3_M-500-13TeV-pythia8_cff_py_GEN.root'

    # NegInt
    #inF         = 'ADDGravToGG_NED-4_LambdaT-4000_NegInt-M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF          = 'ADDGravToGG_NED-4_LambdaT-4000_NegInt-nomhat-M-500-13TeV-pythia8_cff_py_GEN.root'
    #inF         = 'ADDGravToGG_LT-4000_NegInt-M-500-13TeV-pythia8_cff_py_GEN.root'
    # GG Standard Model Only
    #inF         = 'GG_M-500-1100-13TeV-pythia8_cff_py_GEN.root'

    # Sherpa
    #Sherpa Samples
    #inF         = 'ADDGravToGG_MS-4000_NED-4_KK-1_M-1000To2000_13TeV-sherpa.root'



    INFILE    = PATH + inF
    inputFile = 'file:%s' %(INFILE)
    outName = 'Test%s' %(inF)
else:
    print "Logical FileName Provided: "
    inputFile = '/store/mc/RunIISummer16MiniAODv2/ADDGravToGG_MS-4000_NED-4_KK-1_M-1000To2000_13TeV-sherpa/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/6C12F491-0BB7-E611-B418-0025904FE658.root'
    #Provide Logical Filename
    #inputFile = '/store/....'
    outName = "ADDGravToGG_MS-4000_NED-4_KK-1_M-1000To2000_13TeV-sherpa.root"

#------------------------------------------
print 'Configuration file Run with the following settings: '
print 'isMC = ', isMC
if isPythia8gen:
    print 'Pythia GEN'
if isSherpaDiphoton:
    print 'Sherpa GEN'
print 'processing ', inputFile
print 'Writing output to file ', outName


#------------------------------------------

options = VarParsing ('python')
options.register('nEventsSample',
                 1000,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "Total number of events in dataset for event weight calculation.")
## 'maxEvents' is already registered by the Framework, changing default value
options.setDefault('maxEvents', 1000)

print 'nEventsSample: ', options.nEventsSample
process = cms.Process("Demo")

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#globalTag ='notset'
#options.parseArguments()

if isMC:
   version = os.getenv("CMSSW_VERSION")
   if "CMSSW_8" in version:
       if "Spring16" in outName:
           globalTag = '80X_mcRun2_asymptotic_2016_miniAODv2'
       if "Summer16" in outName:
           # globalTag = '80X_mcRun2_asymptotic_2016_TrancheIV_v6'
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




process.load("FWCore.MessageService.MessageLogger_cfi")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:myfile.root'
        #'file:/afs/cern.ch/user/c/ciperez/Generation/CMSSW_9_3_8/src/ADDGravToGG_NED-4_LambdaT-4000_M-500_13TeV-pythia8_cff_py_GEN.root'
        inputFile
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
        genparticles = cms.InputTag(inTag),
        #photonsMiniAOD = cms.InputTag("slimmedPhotons"),
        minPhotonPt = cms.double(75.),
        # genParticle tag
        #genParticlesMiniAOD = cms.InputTag("prunedGenParticles"),
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
process.xsec = cms.EDAnalyzer("GenXSecAnalyzer")
process.p = cms.Path(process.demo * process.xsec)
