# based on Pythia8_A14_NNPDF23LO_Common.py
EventsNumber=500
Random:setSeed = on
Random:seed = 0
Beams:idA = 2212
Beams:idB = 2212
Beams:eCM = 14000.
#physics processes
HardQCD:all = on
#
#PDF:pSet = LHAPDF6:MSTW2008lo68cl.LHgrid
PDF:pSet = LHAPDF6:NNPDF23_lo_as_0130_qed
PDF:useHard = on

Tune:ee = 7 
Tune:pp = 14

SpaceShower:rapidityOrder = on
SigmaProcess:alphaSvalue = 0.140
SpaceShower:pT0Ref = 1.56
SpaceShower:pTmaxFudge = 0.91
SpaceShower:pTdampFudge = 1.05
SpaceShower:alphaSvalue = 0.127
TimeShower:alphaSvalue = 0.127
BeamRemnants:primordialKThard = 1.88
MultipartonInteractions:pT0Ref = 2.09
MultipartonInteractions:alphaSvalue = 0.126
ColourReconnection:reconnect=on
ColourReconnection:range=1.71


#Pythia settings 
PhaseSpace:mHatMin = 40.
PhaseSpace:mHatMax = 10000
PhaseSpace:pTHatMin = 40
PhaseSpace:pTHatMax = 10000
#set K_S, Lambda stable
ParticleDecays:limitTau0 = on
#Makes particles with c*tau>10 mm stable
ParticleDecays:tau0Max = 10

# fill high-pT tail and add weights to events
PhaseSpace:bias2Selection = on
PhaseSpace:bias2SelectionPow = 5.0;
