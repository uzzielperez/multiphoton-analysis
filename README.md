# multiphoton-analysis

## To build
### 2018
```bash

# setup cmssw release
cmsrel CMSSW_9_3_8
cd CMSSW_9_3_8/src
cmsenv

# clone repository
git clone git@github.com:uzzielperez/multiphoton-analysis

# compile code
cd multiphoton-analysis
scram b -j 16

# run locally
cmsRun nPhotonAnalyzer/python/ConfFile_cfg.py

# crab submission under development
cd $CMSSW_BASE
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init -voms cms -rfc
./nPhotonAnalyzer/test/submit_crab_cfg.py
```
