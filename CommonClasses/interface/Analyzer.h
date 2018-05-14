#ifndef Analyzer_h
#define Analyzer_h

#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// from our CommonClasses
#include "multiphoton-analysis/CommonClasses/interface/EventInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/BeamSpotInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/VertexInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/TriggerInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/JetInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/PhotonID.h"
#include "multiphoton-analysis/CommonClasses/interface/PhotonInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/GenParticleInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/DiPhotonInfo.h"
#include "multiphoton-analysis/CommonClasses/interface/PileupInfo.h"

// for TFileService, trees
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TTree.h"

// for ECAL topology
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

// for EGM ID
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"

// for photons
#include "DataFormats/PatCandidates/interface/Photon.h"

// for jets
#include "DataFormats/PatCandidates/interface/Jet.h"

// for genParticles
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

// for genEventInfo
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

// for deltaR
#include "DataFormats/Math/interface/deltaR.h"

// for trigger and filter decisions
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"





#endif
