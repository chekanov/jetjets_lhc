# jetjets_lhc
Example code for studies of jet-jet invariant masses
# Dijet program for ANL HL-LHC studies 

This program was created using HepSim (http://atlaswww.hep.anl.gov/hepsim/)
software and Pythia8 control
cards as used for the HepSim repository.
It is designed to study dijet masses for HL- and HE-LHC using Pythia8 model.


## Running on Linux using HepSim image 

You can run this program on any Linux platform (including HPC)
using the HepSim singularity/Docker file.
For more information see http://atlaswww.hep.anl.gov/hepsim/doc/doku.php?id=hepsim:dev_singularity

To run this program, use

```
singularity  exec centos7hepsim.img  bash -l
make clean
make
./A_RUN_ALL
```


S.Chekanov (ANL)
