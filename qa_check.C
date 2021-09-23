#ifndef __CLING__

#include "TChain.h"
#include "FairRunAna.h"
#include "Header.h"
#include "MpdBasicTrackCut.h"
#include "MpdPairDeltaPhiStarDeltaEtaCut.h"
#include "MpdTpcMonitor.h"
#include "NicaConst.h"
#include "NicaSplitedTrackToStatusTask.h"
#include "NicaTrackTpcToFCut.h"
#include "NicaHtmlFile.h"
#include "NicaHtmlObject.h"
#endif



void qa_check(TString dataList){
	
	TString outList =dataList;
	outList.Replace(0,7,"datagood",8);
	TString file_name = outList;
	file_name.Resize(file_name.Length()-4);
	string str_temp = (string)file_name;
	file_name = Form("%s%d.txt",str_temp.c_str(),0);

	ofstream goodfiles;
	goodfiles.open(file_name);
	goodfiles << "====NIcA_MINIDST_PROD===" << endl;
	goodfiles << "NVAR 1 NJOBS 10000" << endl;
	
	TChain ch;
        int j=1, temp=0, temp2=0,max=0;
        ifstream infile;
        ofstream errf("err_files.txt",std::ios_base::app);
        infile.open(dataList);
        errf.open("err_files.txt",fstream::app);
        
	string st;
        std::vector<int> offs;
        getline(infile,st);
        getline(infile,st);
        getline(infile,st);

        ch.AddFile(st.c_str(),-1,Form("MiniDst"));
        offs.push_back(ch.GetEntries());
        max=offs[0];

	outList.Resize(outList.Length()-3);
	outList.Append("html");
	outList.Replace(0,8,"report",6);

	NicaHtmlFile* fil = new NicaHtmlFile(outList,true);
	NicaHtmlTable* table = new NicaHtmlTable();
	table->SetClass("nicatable");
	NicaHtmlRow* row = new NicaHtmlRow(1,"Title","green_");
	row->SetClass(NicaHtmlTableRowStyles::DarkBlue());
	NicaHtmlCell* cell1 = new NicaHtmlCell("File path");
	NicaHtmlCell* cell2 = new NicaHtmlCell("Size");
	NicaHtmlCell* cell3 = new NicaHtmlCell("Good/bad");
	
	int n_files = 500;
	int file_no = 0;
	int txt_no = 0;
	
	row->AddContent(*cell1);
	row->AddContent(*cell2);
	row->AddContent(*cell3);
	table->AddContent(*row);
        
	while(infile){
        	temp=ch.GetEntries();
		getline(infile,st);
                const char* cstr = st.c_str();
                ch.AddFile(cstr,-1,Form("MiniDst"));
                
		offs.push_back(ch.GetEntries()-temp);
                if(offs[j]>max) max=offs[j];
                if (ch.GetEntries()-temp<max){
                        
			cout <<j << "\t" << ch.GetEntries() - temp<<endl;
			
			NicaHtmlRow* row = new NicaHtmlRow(ch.GetEntries()-temp,"NicaHtmlFile","red_");
			NicaHtmlCell* cell1 = new NicaHtmlCell(st);
			NicaHtmlCell* cell2 = new NicaHtmlCell(std::to_string(ch.GetEntries()-temp));
			NicaHtmlCell* cell3 = new NicaHtmlCell("bad");
			
			row->SetClass(NicaHtmlTableRowStyles::Red());
			row->AddContent(*cell1);
			row->AddContent(*cell2);
			row->AddContent(*cell3);
			table->AddContent(*row);

			cout << st << endl;
                        errf << st << endl;
	
                } else {
 
			goodfiles << st << endl;
			NicaHtmlRow* row = new NicaHtmlRow(ch.GetEntries()-temp,"NicaHtmlFile","green_");
			NicaHtmlCell* cell1 = new NicaHtmlCell(st);
			NicaHtmlCell* cell2 = new NicaHtmlCell(std::to_string(ch.GetEntries()-temp));			
			NicaHtmlCell* cell3 = new NicaHtmlCell("good");

			row->AddContent(*cell1);
			row->AddContent(*cell2);
			row->AddContent(*cell3);
			row->SetClass(NicaHtmlTableRowStyles::LightGreen());
			table->AddContent(*row);
			file_no++;	
     		}
		if(file_no==n_files){
			file_no=0;
			txt_no++;
			file_name = Form("%s%d.txt",str_temp.c_str(),txt_no);
		
			goodfiles.close();
			goodfiles.open(file_name);
			goodfiles << "====NIcA_MINIDST_PROD===" << endl;
        		goodfiles << "NVAR 1 NJOBS 10000" << endl;		
			cout << "Creating " <<  file_name << endl;
		}

                temp2=ch.GetEntries()-temp;
                ++j;
	}

	fil->AddContent(*table);	
	fil->Save();

	errf << endl;
        errf.close();

}
