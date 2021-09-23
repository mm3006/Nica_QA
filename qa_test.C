/*
 * qa4.C
 *
 *  Created on: 3 mar 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef __CLING__
#include "FairTask.h"
#include "NicaDataFormat.h"
#include "NicaDataFormatManager.h"
#include "MpdPIDOnTheFly.h"
#include "MpdMiniEvent.h"
#include "MpdMiniDst.h"
#include "MpdMiniArrays.h"
#include "TChain.h"
#include "FairRunAna.h"
#include "Header.h"
#include "MpdBasicTrackCut.h"
#include "MpdPairDeltaPhiStarDeltaEtaCut.h"
#include "MpdTpcMonitor.h"
#include "NicaConst.h"
#include "NicaSplitedTrackToStatusTask.h"
#include "NicaTrackTpcToFCut.h"
#endif

class NicaMpdEventID : public FairTask{
        MpdMiniEvent *fEvent;
        MpdPid *fPID;
        TVector3 *fEventVector, *fMCVector;
        TClonesArray *fMiniEvent;
        Long64_t *off_vec;
public:
	NicaMpdEventID()  :fEvent(nullptr),fPID(nullptr),fMiniEvent(nullptr){
        fEventVector = new TVector3();
        fMCVector = new TVector3();

}

virtual InitStatus Init() {
        fMiniEvent =(TClonesArray*) FairRootManager::Instance()->GetObject("Event");
        fEvent = (MpdMiniEvent*) fMiniEvent->UncheckedAt(0);
        TChain* chain = FairRootManager::Instance()->GetInChain();
        off_vec = chain->GetTreeOffset();
	static Int_t entr = chain->GetTreeOffsetLen();
	return kSUCCESS;
}
	void Exec(Option_t* opt) {
 	static int k=0;
	k++;
	if (k>=off_vec[1]-off_vec[0]) k=0;
	fEvent->setEventId(k);   	
}

        virtual ~NicaMpdEventID(){ };
        ClassDef(NicaMpdEventID,1)
};

class NicaPrintEventId: public FairTask{
	MpdMiniEvent *fEvent;
        MpdPid *fPID;
        TVector3 *fEventVector, *fMCVector;
        TClonesArray *fMiniEvent;
        Long64_t *off_vec;	
public:
	NicaPrintEventId()  :fEvent(nullptr),fPID(nullptr),fMiniEvent(nullptr){
        fEventVector = new TVector3();
        fMCVector = new TVector3();

	}
	virtual InitStatus Init() {
        fMiniEvent =(TClonesArray*) FairRootManager::Instance()->GetObject("Event");
        fEvent = (MpdMiniEvent*) fMiniEvent->UncheckedAt(0);
        //cout << "Print Init" << endl;
        return kSUCCESS;
	}
                    	


};



enum Cutmode { kCombined, kTpcOnly, kTofOnly, kMCOnly };

NicaEvent* linkDummy;

NicaQAPlot GetTrackPlots() {
	TClonesArray* fMiniEvent =(TClonesArray*) FairRootManager::Instance()->GetObject("Event");
        if(fMiniEvent){ MpdMiniEvent* fEvent = (MpdMiniEvent*) fMiniEvent->UncheckedAt(0);
	if(fEvent) cout << "fevent id: " << fEvent->eventId() << endl;
}
	
	NicaQAPlot qa_track(ENicaCutUpdate::kTrackUpdate);
	const Int_t monitor_sim_data  = NicaDataFieldID::ImStep;
	const Int_t monitor_reco_data = NicaDataFieldID::ReStep;
	Int_t histo_id;
	// clang-format off
	
	//p-signed vs dedx
	histo_id = qa_track.AddTH2("tpc", NicaDataFieldID::ETrack::kP + monitor_reco_data, NicaDataFieldID::EExpTrack::kTpcDedx + monitor_reco_data);
	qa_track.SetAxis2D(histo_id, 200, 0, 4, 200, 0, 2E+4);
	//p-signed vs m2
	
	histo_id = qa_track.AddTH2("tof_m", monitor_reco_data + NicaDataFieldID::ETrack::kP, monitor_reco_data + NicaDataFieldID::EExpTrack::kTofM2);
	qa_track.SetAxis2D(histo_id, 200, 0, 4, 200, -0.1, 1.4);


	//Nsigma pi
	
	
	histo_id = qa_track.AddTH1("Nsigma pi", NicaDataFieldID::EExpTrack::kTpcNsigmaPi+monitor_reco_data);
	qa_track.SetAxis1D(histo_id,200,-5,5);
	

	//Nsigma ka
	
	histo_id = qa_track.AddTH1("Nsigma ka", NicaDataFieldID::EExpTrack::kTpcNsigmaKa+monitor_reco_data);
	qa_track.SetAxis1D(histo_id,200,-5,5);

	//Nsigma pr
	
	histo_id = qa_track.AddTH1("Nsigma pr", NicaDataFieldID::EExpTrack::kTpcNsigmaPr+monitor_reco_data);
	qa_track.SetAxis1D(histo_id,200,-5,5);

	//Nhits	
	
	histo_id = qa_track.AddTH1("Number of hits", NicaDataFieldID::EExpTrack::kTpcNHits+monitor_reco_data);
	qa_track.SetAxis1D(histo_id,200,0,60);
	
	//DCA
	
	histo_id = qa_track.AddTH1("DCA",NicaDataFieldID::EExpTrack::kDca+monitor_reco_data);
	qa_track.SetAxis1D(histo_id,200,0,3);

	//Pt
	
	histo_id = qa_track.AddTH1("Pt",NicaDataFieldID::ETrack::kPt);
	qa_track.SetAxis1D(histo_id,200,0,2.5);

	//Pz
	histo_id = qa_track.AddTH1("Pz",NicaDataFieldID::ETrack::kPz);	
	qa_track.SetAxis1D(histo_id,200,0,2.5);

	//Phi
	
	histo_id = qa_track.AddTH1("Phi",NicaDataFieldID::ETrack::kPhi);
	qa_track.SetAxis1D(histo_id,200,-3.2,3.2);

	//pt vs phi
	
	histo_id = qa_track.AddTH2("Pt vs Phi",NicaDataFieldID::ETrack::kPt,NicaDataFieldID::ETrack::kPhi);
	qa_track.SetAxis2D(histo_id,200,0,5,200,-3.2,3.2);

	//pt vs eta
	
	histo_id = qa_track.AddTH2("Pt vs Eta",NicaDataFieldID::ETrack::kPt, NicaDataFieldID::ETrack::kEta);
	qa_track.SetAxis2D(histo_id,200,0,2.5,200,-2.5,2.5);

	//dedx
	
	histo_id = qa_track.AddTH1("dedx",NicaDataFieldID::EExpTrack::kTpcDedx+monitor_reco_data);
	qa_track.SetAxis1D(histo_id,200,0,1E+4);
 
	return qa_track;
}


NicaQAPlot GetEventPlots() {
	Int_t histo_id;
	const Int_t monitor_sim_data  = NicaDataFieldID::ImStep;
	const Int_t monitor_reco_data = NicaDataFieldID::ReStep;
	NicaQAPlot qa_event(ENicaCutUpdate::kEventUpdate);
	
	//Event observables


	//simulated vs reconstructed multiplicity
	qa_event.AddTH2("impact vs multiplicity", NicaDataFieldID::EMcEvent::kB + monitor_sim_data, NicaDataFieldID::EEvent::kTracksNo + monitor_sim_data,200, 0,20,200,0,2000);

	//MC multiplicity vs impact parameter
	qa_event.AddTH2("reco vs sim track No", NicaDataFieldID::EEvent::kTracksNo + monitor_reco_data, NicaDataFieldID::EEvent::kTracksNo + monitor_sim_data, 200,0,2000,200,0,2000);

	//Reco tracks vs tof
	qa_event.AddTH2("reco tracks vs tof",NicaDataFieldID::EEvent::kTracksNo+monitor_reco_data, NicaDataFieldID::EExpEvent::kTofTracksNo+monitor_reco_data,200,0,2000,200,0,800);

	//vertexZ sim vs reco
	qa_event.AddTH2("vertexZ sim vs reco",NicaDataFieldID::EEvent::kVertexZ + monitor_reco_data,NicaDataFieldID::EEvent::kVertexZ + monitor_sim_data, 200,-0.01,0.01,200,-0.01,0.01);

	//vertexXY sim vs reco
	qa_event.AddTH2("vertexXY sim vs reco",NicaDataFieldID::EEvent::kVertexXY + monitor_reco_data,NicaDataFieldID::EEvent::kVertexXY+monitor_sim_data,200,0,0.15,200,0,0.15);

	//vertexX vs vertexY sim
	qa_event.AddTH2("vertexX vs vertexY sim",NicaDataFieldID::EEvent::kVertexX+monitor_sim_data,NicaDataFieldID::EEvent::kVertexY+monitor_sim_data,200,-0.15,0.15,200,-0.15,0.15);

	//vertexX vs vertexY reco
	qa_event.AddTH2("vertexX vs vertexY reco",NicaDataFieldID::EEvent::kVertexX+monitor_reco_data,NicaDataFieldID::EEvent::kVertexY+ monitor_reco_data,200,-0.15,0.15,200,-0.15,0.15);

	qa_event.AddTH2("MC vertex vs entry",NicaDataFieldID::EEvent::kVertexX+monitor_sim_data,NicaDataFieldID::EEvent::kEventId+monitor_reco_data,200,0,0.8,200,0,500);

	qa_event.AddTH2("Impact parameter vs entry",NicaDataFieldID::EMcEvent::kB+monitor_sim_data,NicaDataFieldID::EEvent::kEventId+monitor_reco_data,200,0,20,200,0,500);
	
	//kVertexZ
	qa_event.AddTH1("Vertex Z", NicaDataFieldID::EEvent::kVertexZ,200,-0.001,0.001);
	//kVertexXY
	qa_event.AddTH1("Vertex XY",NicaDataFieldID::EEvent::kVertexXY,200,0,0.30);

        

	return qa_event;
}



void AddCuts(NicaEventAna* ana, Cutmode mode) {
	ana->SetFormat(new NicaMpdMiniDstFullEvent());
	NicaTrackBasicMCCut mc;
	mc.SetEtaCut(-1E+9, 1E+9);
	mc.SetPdgCut(211);
	mc.SetPtCut(0, 1E+9);

	ana->AddCut(mc, "im+{0}");  // 0,3,6,9,12
	mc.SetPdgCut(321);
	mc.SetCollectionID(1);
	ana->AddCut(mc, "im+{1}");  // 1,4,7,10,13

	mc.SetCollectionID(2);
	mc.SetPdgCut(2212);
	ana->AddCut(mc, "im{2}");  // 2,5,8,12,14

	NicaTrackOnlyPrimariesCut prim;
	ana->AddCut(prim, "im{3x1}");
	if (mode == Cutmode::kMCOnly) return;

	TString pionOpt   = Form("re+{%i}", 0);
	TString kaonOpt   = Form("re+{%i}", 1);
	TString protonOpt = Form("re+{%i}", 2);
	MpdBasicTrackCut mpdPion;
	MpdBasicTrackCut mpdKaon;
	MpdBasicTrackCut mpdProton;
	mpdPion.SetOptionForAllCuts(pionOpt);
	mpdKaon.SetOptionForAllCuts(kaonOpt);
	mpdProton.SetOptionForAllCuts(protonOpt);
/*
	mpdPion.GetTpcTofCut()->SetActiveSigma("pi");
	mpdKaon.GetTpcTofCut()->SetActiveSigma("K");
	mpdProton.GetTpcTofCut()->SetActiveSigma("p");
	
	mpdPion.GetTpcTofCut()->SetSigma(-2, 2, "pi");
	mpdKaon.GetTpcTofCut()->SetSigma(-2, 2, "K");
	mpdProton.GetTpcTofCut()->SetSigma(-2, 2, "p");

	mpdPion.GetTpcTofCut()->SetModeNotBad();
	mpdKaon.GetTpcTofCut()->SetModeNotBad();
	mpdProton.GetTpcTofCut()->SetModeNotBad();
	
	mpdPion.GetTpcTofCut()->SetNHits(15, 60);
	mpdKaon.GetTpcTofCut()->SetNHits(15, 60);
	mpdProton.GetTpcTofCut()->SetNHits(15, 60);
	
	mpdPion.GetTpcTofCut()->SetM2(-0.3, 0.1);
	mpdKaon.GetTpcTofCut()->SetM2(0.15, 0.351);
	mpdProton.GetTpcTofCut()->SetM2(0.55, 1.4);
	
	
	if (mode == Cutmode::kCombined) {
		mpdPion.GetTpcTofCut()->SetThreshold(0.5);
		mpdKaon.GetTpcTofCut()->SetThreshold(0.5);
		mpdProton.GetTpcTofCut()->SetThreshold(0.9);
	} else if (mode == Cutmode::kTofOnly) {
		mpdPion.GetTpcTofCut()->SetThreshold(0);
		mpdKaon.GetTpcTofCut()->SetThreshold(0);
	mpdProton.GetTpcTofCut()->SetThreshold(0);
	} else {
		mpdPion.GetTpcTofCut()->SetThreshold(1E+3);
		mpdKaon.GetTpcTofCut()->SetThreshold(1E+3);
		mpdProton.GetTpcTofCut()->SetThreshold(1E+3);
	}
*/
	ana->AddCutsAndMonitors(mpdPion);
	ana->AddCutsAndMonitors(mpdKaon);
	ana->AddCutsAndMonitors(mpdProton);
}

void SetCuts(NicaQATrackTask* ana, Cutmode mod) {
	AddCuts(ana, mod);

	ana->SetQAPlot(GetTrackPlots());
	ana->SetQAPlot(GetEventPlots());

	ana->SetEventCollectionNames({"central", "semicentral"});
	ana->SetTrackCollectionNames({"pion", "kaon", "proton", "prim_pion", "prim_kaon", "prim_proton"});
}

void GetData(FairRunAna* run, TString dataList,int count) {

	const Int_t N_FILES = 400;
	TString id          = gSystem->Getenv("JOB_ID_NICA");
	Int_t job_id        = id.Atoi();
	TString inFile      = NicaJobs::GetParameter(dataList, job_id * N_FILES, 0);
	TString outList     = dataList;

	outList.Replace(0,8,"data",4);
	outList.Resize(outList.Length()-3);
	outList.Append("root");
	TString outFile =  outList;

	run->SetOutputFile(outFile);
	NicaMiniDstSource* source = new NicaMiniDstSource(inFile);

	for (int j = 1; j < N_FILES; j++) {

		TString name = NicaJobs::GetParameter(dataList, job_id * N_FILES + j, 0);
		source->AddFile(name);

	}
	run->SetSource(source);	
}

void qa_test(TString dataList) {

	FairRunAna* run = new FairRunAna();
	run->AddTask(new NicaMpdEventID());

	int count = 0;
        string line;
        ifstream file(dataList);
        if(file.is_open()){
                while(!file.eof()){
                        getline(file,line);
                        count++;
                }
                file.close();
        }

	GetData(run, dataList,count);
  
	NicaQATrackTask* t1 = new NicaQATrackTask();
	NicaQATrackTask* t2 = new NicaQATrackTask();
	NicaQATrackTask* t3 = new NicaQATrackTask();
	NicaQATrackTask* t4 = new NicaQATrackTask();
	
	SetCuts(t1, Cutmode::kCombined);
	SetCuts(t2, Cutmode::kTofOnly);
	SetCuts(t3, Cutmode::kTpcOnly);
	SetCuts(t4, Cutmode::kMCOnly);
	
	run->AddTask(t1);
	run->AddTask(t2);
	run->AddTask(t3);
	run->AddTask(t4);	
	run->AddTask(new NicaPrintEventId());
	run->Init();

	run->Run(50000);	     
}

