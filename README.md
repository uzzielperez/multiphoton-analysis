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

```

