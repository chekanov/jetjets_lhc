// S.Chekanov ANL
// dijet studies using FatsJet 
// created to debug Pythia8 features 

#include <TROOT.h>
#include <TFile.h>
#include <TH1D.h>
#include <TRandom2.h>
#include <map>
#include <limits>       // std::numeric_limits
#include "TMath.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include <vector>
#include <string>
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include <TMath.h>
#include "Pythia8/Pythia.h"

using namespace std;
using namespace Pythia8;
using namespace fastjet;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while(std::getline(ss, item, delim)) {
                elems.push_back(item);
        }
        return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        return split(s, delim, elems);
}


int main(int argc, char* argv[]) {


	// Check that correct number of command-line arguments
	if (argc != 5) {
		cerr << " Unexpected number of command-line arguments. \n You are"
		<< " Arguments: input, output, nr_events, seed (or -1 for automatic timestamps)\n"
		<< " Program stopped! " << endl;
		return 1;
	}


	cout << "HepSim:  Pythia8 Input Configuration =" << argv[1] << endl;
	cout << "HepSim:  ProMC Output =" << argv[2] << endl;
	cout << "HepSim:  Input events =" << argv[3] << endl;
        cout << "HepSim:  Input seed =" << argv[4] << endl;

	string infile("-"), outfile("-"),  sevents("-"), strseed("-");
	infile = argv[1];
	outfile = argv[2];
	sevents = argv[3];
        strseed = argv[4];
	int Ntot = atoi(sevents.c_str());
        int Iseed = atoi( strseed.c_str());
        string sseed="Random:seed = "+strseed;
        if (Iseed<0)  cout << "HepSim:  CPU timestamp will be used for seed" << endl; 
        else          cout << "HepSim:  Pythia uses  =" << strseed << " as a seed " << endl;

	// Generator. Process selection. Tevatron initialization. Histogram.
	Pythia pythia;

	/////////// read config files ////////////////////
	string sets="";
	string sets1="";
	vector<string> configs;
	string events;
	ifstream myfile;
	myfile.open(infile.c_str(), ios::in);
	if (!myfile) {
		cerr << "Can't open input file:  " << infile.c_str() << endl;
		exit(1);
	} else {
		string line;
		while(getline(myfile,line))
		{
			//the following line trims white space from the beginning of the string
			line.erase(line.begin(), find_if(line.begin(), line.end(), not1(ptr_fun<int, int>(isspace))));
			if(line[0] == '#') continue;
			if (line.length()<3) continue;
			string tmp=string(line);
			// no empty spaces inside string
			std::string::iterator end_pos = std::remove(tmp.begin(), tmp.end(), ' ');
			tmp.erase(end_pos, tmp.end());
			bool special=false;
			int found1=tmp.find("EventsNumber");
		        if (found1!=(int)std::string::npos) {events=tmp; special=true;}
			if (!special)  {sets1=sets1+tmp+"; "; pythia.readString(line); }
			configs.push_back(line);
		}
		myfile.close();
		vector<string> readnum=split(events,'=');
		//Ntot= atoi(readnum[1].c_str());
		cout << "Reading events. " << events << " Total number is=" << Ntot<< endl;
		for (unsigned int i=0; i<configs.size(); i++) {
			cout << ".. input ="+configs[i] << endl;
			sets=sets+configs[i]+";";
		}
	} // end else


        if (Iseed>0) { 
           pythia.readString("Random:setSeed = on");
           pythia.readString(sseed.c_str());
        }

	pythia.init();

	pythia.settings.listChanged(); // Show changed settings
	double versionNumber = pythia.settings.parm("Pythia:versionNumber");
	pythia.particleData.listChanged(); // Show changed particle data
	std::stringstream s;
	s << versionNumber;
	string version=s.str();

	const double ptJet=50;
	const double etaJet=2.4;
	const double R = 0.4;

	cout << "min PT jet=" << ptJet << endl;
	cout << "max ETA jet=" << etaJet << endl;
	cout << "jet R=" << R << endl;
	JetDefinition jet_def(antikt_algorithm, R);

	// book a histogram, make sure that the output name is Analysis.root
	cout << "\n -> Output file is =" << outfile.c_str() << endl;
	TFile * RootFile = new TFile(outfile.c_str(), "RECREATE", "Histogram file");
	TH1D * h_debug = new TH1D("debug", "debug", 5, 0, 5.);
        TH1D * h_info = new TH1D("info", "version,CM,seed", 5, 0, 5.);
        TH1D * h_cross = new TH1D("cross", "cross,events,lumi", 10, 0, 10.);
	TH1D * h_weightsFine = new TH1D("weightsFine", "weights fine", 10000, 0, 0.001);
        TH1D * h_weightsCorse = new TH1D("weightsCorse", "weights corse", 100, 0, 5);
	TH1D * h_pt = new TH1D("jet_pt", "pt",1000,0,30000);
	TH1D * h_pt_lead = new TH1D("jet_pt_lead", "pt of lead jet",1000,0,30000);
	TH1D * h_eta = new TH1D("jet_eta", "eta", 20, -3, 3);

/*
// range extensions
ranges=[]
R=13156
deltaStart=246
for i in range(60):
    delta=deltaStart+5+i
    R=R+delta
    ranges.append(R)

print ranges
*/
	double mjjBins[] = {99,112,125,138,151,164,177,190, 203, 216, 229, 243, 257, 272, 287, 303, 319, 335, 352, 369, 387, 405, 424, 443, 462, 482, 502, 523, 544, 566, 588, 611, 634, 657, 681, 705, 730, 755, 781, 807, 834, 861, 889, 917, 946, 976, 1006, 1037, 1068, 1100, 1133, 1166, 1200, 1234, 1269, 1305, 1341, 1378, 1416, 1454, 1493, 1533, 1573, 1614, 1656, 1698, 1741, 1785, 1830, 1875, 1921, 1968, 2016, 2065, 2114, 2164, 2215, 2267, 2320, 2374, 2429, 2485, 2542, 2600, 2659, 2719, 2780, 2842, 2905, 2969, 3034, 3100, 3167, 3235, 3305, 3376, 3448, 3521, 3596, 3672, 3749, 3827, 3907, 3988, 4070, 4154, 4239, 4326, 4414, 4504, 4595, 4688, 4782, 4878, 4975, 5074, 5175, 5277, 5381, 5487, 5595, 5705, 5817, 5931, 6047, 6165, 6285, 6407, 6531, 6658, 6787, 6918, 7052, 7188, 7326, 7467, 7610, 7756, 7904, 8055, 8208, 8364, 8523, 8685, 8850, 9019, 9191, 9366, 9544, 9726, 9911, 10100, 10292, 10488, 10688, 10892, 11100, 11312, 11528, 11748, 11972, 12200, 12432, 12669, 12910, 13156, 13407, 13659, 13912, 14166, 14421, 14677, 14934, 15192, 15451, 15711, 15972, 16234, 16497, 16761, 17026, 17292, 17559, 17827, 18096, 18366, 18637, 18909, 19182, 19456, 19731, 20007, 20284, 20562, 20841, 21121, 21402, 21684, 21967, 22251, 22536, 22822, 23109, 23397, 23686, 23976, 24267, 24559, 24852, 25146, 25441, 25737, 26034, 26332, 26631, 26931, 27232, 27534, 27837, 28141, 28446, 28752, 29059, 29367, 29676, 29986};  

	const int nBins=sizeof(mjjBins)/sizeof(double);
	double xbins[nBins];
        double xbins_tev[nBins];
        for (int j=0; j<nBins; j++){xbins[j]=mjjBins[j]; xbins_tev[j]=0.001*mjjBins[j]; };

	TH1D* jetjetmass_2jet=new TH1D( "JetJetMass_2jet", "JetJet Mass > 1 jet", nBins-1, xbins);jetjetmass_2jet->Sumw2();
	TH1D* jetjetmass_2jet_ystar=new TH1D( "JetJetMass_2jet_ystar", "JetJet Mass > 1 jet with |y*|<0.6", nBins-1, xbins);jetjetmass_2jet_ystar->Sumw2();
        TH1D * binsM = new TH1D("bins_m", "bins_m", nBins-1, xbins);
	binsM->Sumw2();
        TH1D * binsM_tev = new TH1D("bins_m_tev", "bins_m_tev", nBins-1, xbins_tev);
        binsM_tev->Sumw2();


	for (Int_t j=0; j<nBins-1; j++) {
		float x=xbins[j+1]-xbins[j];
		binsM->Fill(xbins[j]+0.5*x,x);
                float xx=xbins_tev[j+1]-xbins_tev[j];
                binsM_tev->Fill(xbins_tev[j]+0.5*xx,xx);
                // cout << j << " " << xbins[j+1] << " bin size=" << x << endl;
	}
	// set bin errors to 0 (we use it to divide the bin width!)
          for (int i=0 ; i<(binsM->GetNbinsX()); i++) {
                                binsM->SetBinError(  i+1, 0.0);
                                binsM_tev->SetBinError(  i+1, 0.0);
           }


	// Begin event loop. Generate event. Skip if error. List first one.
	for (int n = 0; n < Ntot; n++) {
		if (!pythia.next()) continue;
		// if (n < 1) {pythia.info.list(); pythia.event.list();}
		// Loop over particles in event. Find last Z0 copy. Fill its pT.

		// get weights
		double weight=pythia.info.weight();
		h_weightsFine->Fill(weight);
                h_weightsCorse->Fill(weight);
   
		if (      ((n<=100 && (n%10) == 0)) ||
		          ((n<=1000 && (n%100) == 0))  ||
		          ((n>=1000 && (n%1000) == 0)) ) {
			cout << "No events= " << n << " passed"  << endl; };

		vector<PseudoJet> avec;
		h_debug->Fill("Generated",1.);
		for (int i =0; i<pythia.event.size(); i++) {

			int status=pythia.event[i].statusHepMC();
			int pdgid=pythia.event[i].id();
			double ee=pythia.event[i].e();
			double px=pythia.event[i].px();
			double py=pythia.event[i].py();
			double pz=pythia.event[i].pz();
			double pt=sqrt(px*px+py*py);
			double eta=-log(tan(atan2(pt,(double)pz)/2));
			if ( pt < 0.2)                   continue;
			if ( fabs(eta)> 3.0 )            continue;
			// only final states, no neutrinos
			if (status !=1) continue;
			if (abs(pdgid)==12 || abs(pdgid)==14 || abs(pdgid)==16 ) continue;
			// fill other particles (including leptons)
			avec.push_back( PseudoJet(px,py,pz,ee) );

		} // end particle loop


		ClusterSequence clust_seq(avec, jet_def);
		vector<PseudoJet> inclusive_jets = clust_seq.inclusive_jets(0.9*ptJet);
		vector<PseudoJet> sorted_jets = sorted_by_pt(inclusive_jets);
		vector<PseudoJet> jets;
               
	        // select jets	
		for (unsigned int k = 0; k<sorted_jets.size(); k++) {
			double eta=sorted_jets[k].pseudorapidity();
			//double phi=sorted_jets[k].phi();
			double pt = sorted_jets[k].perp();
			//double e = sorted_jets[k].e();
                        if (abs(eta)>etaJet) continue;
                        if (pt<ptJet)        continue;
                        h_pt->Fill(pt,weight);
                        h_eta->Fill(eta,weight);
			jets.push_back(sorted_jets[k]);
                  }


                // fill 2-jet inv. masses
                if (jets.size()>1) {
                        h_debug->Fill("2 jets",1.);
                        PseudoJet p1=jets.at(0);
                        PseudoJet p2=jets.at(1);
                        h_pt_lead->Fill(p1.perp(),weight);
                        TLorentzVector LP1(p1.px(),p1.py(),p1.pz(),p1.e());
                        TLorentzVector LP2(p2.px(),p2.py(),p2.pz(),p2.e());
                        TLorentzVector PP=LP1+LP2;
                        double mass_jj=PP.M();
                        jetjetmass_2jet->Fill(mass_jj,weight);
                        if(abs(LP1.Rapidity()-LP2.Rapidity())/2<0.6){
                                        jetjetmass_2jet_ystar->Fill(mass_jj,weight);}

                       } 

 
	} // endl loop over events


	// To check which changes have actually taken effect
	pythia.settings.listChanged();

	pythia.stat();


	// Output histograms
	double sigmapb = pythia.info.sigmaGen() * 1.0E9;
	double sigmapb_err = pythia.info.sigmaErr() * 1.0E9;

	cout << "== Run statistics: " << endl;
        cout << "== Seed number   =" <<  Iseed << endl;
	cout << "== Cross section    =" <<  sigmapb << " +- " << sigmapb_err << " pb" << endl;
	cout << "== Generated Events =" <<  Ntot << endl;
	double lumi=(Ntot/sigmapb);
	cout << "== Luminosity       =" <<  lumi  << " pb-1" << endl;
	cout << "\n\n";

	h_cross->Fill("Nr jobs",1.0); // calibration check
	h_cross->Fill("Cross section [pb]",sigmapb);
	h_cross->Fill("Events",Ntot);
	h_cross->Fill("Lumi [pb]",lumi);
        h_cross->Fill("weightSum",pythia.info.weightSum());
        h_cross->Fill("mergingWeight",pythia.info.mergingWeight());
	h_cross->Fill("ptjet",ptJet);

        h_info->Fill("PythiaVErsion",versionNumber); // calibration check
        h_info->Fill("CM",pythia.info.eCM()); // calibration check
        h_info->Fill("Random Seed",Iseed);

	RootFile->Write();
	RootFile->Print();
	RootFile->Close();

	return 0;
}
