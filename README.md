# Dijet invariant mass studies for HL-LHC 
Example code for studies of jet-jet invariant masses using Pythia8.

To compile it, install PYTHIA8, ROOT, FASTJET and LHAPDF6.
You can also use the HepSim singularity image (http://atlaswww.hep.anl.gov/hepsim/) which
include all these packages.


## Running on Linux using the HepSim image 

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

You can change the number of generated events inside the script A_RUN_ALL.
The output of this program is "out_qcd.root".
The main histogram with dijet invariant mass is "JetJetMass_2jet" (note it has variable bins).

To speed up the calculation, we use the phase-space reweighting.  Since we are using weights,
the statistic uncertainty should be redefined using counting statistics.

To get the proper dijet distribution, divide the histogram "JetJetMass_2jet" by the (variable) bin width
which is stored in the output ROOT file. Then, use the stored cross section to calculate the differential cross section.

This example is provided with the goal to study the shape of the dijet mass distribution in Pythia8. 
 
S.Chekanov (ANL)
