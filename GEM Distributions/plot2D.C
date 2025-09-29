
//-------------------Number of Files merged for Optics1 Target--------------------//

//-------------------C12 pass 1 = 249----------------------//
//-------------------C12 pass 2 = 250----------------------//
//-------------------C12 pass 3 = 247----------------------//
//-------------------C12 pass 4 = 250----------------------//
//-------------------C12 pass 5 = 250----------------------//

//-------------------moller pass 1 = 248----------------------//
//-------------------moller pass 2 = 250----------------------//
//-------------------moller pass 3 = 250----------------------//
//-------------------moller pass 4 = 250----------------------//
//-------------------moller pass 5 = 250----------------------//



const int nFiles = 2;
const char* files[nFiles] = {"../slim_output/slim_C12_opticsDS_p1.root",
                             "../slim_output/slim_Moller_opticsDS_p1.root" };

const char* fileNms[nFiles] = {"C12", "moller"};

const int color[nFiles] = {kBlue, kGreen};
TH2D* hist[nFiles];

void plot2D()
{

   for(int j=0; j<nFiles; j++){

     hist[j] = new TH2D(Form("hist_%s",fileNms[j]),"GEM::X vs Y;x[mm];y[mm]",1000,-1500,1500,1000,-1500,1500);
  
     TFile* fin = new TFile(Form("%s",files[j]), "READ");
     TTree* t = (TTree*)fin->Get("newT");
 
     double gem1_x, gem1_y, rate, gem1_r, sieve_x, sieve_y, gem1_px, gem1_py, gem1_pz, sieve_r;
     t->SetBranchAddress("gem1_x",&gem1_x);
     t->SetBranchAddress("gem1_y",&gem1_y);
     t->SetBranchAddress("rate",&rate);
     t->SetBranchAddress("sieve_r",&sieve_r);


     hist[j]->SetMarkerColor(color[j]);
     hist[j]->SetMarkerStyle(6);

     int nevts = t->GetEntries();

        for(int i=0; i<nevts; i++){

                t->GetEntry(i);
   
                if(sieve_r<26.5) continue;

                rate = rate/200;

                hist[j]->Fill(gem1_x, gem1_y, rate);   
   
        }

        }

        float x1,x2,x3,x4,y1,y2,y3,y4;
        float r1,r2;
        double pi = acos(-1);
        double septant = 360/7.;
        double halfaccep =13;
	TLine* l1=new TLine;
	TLine* l2=new TLine;
	TLine* l3=new TLine;
	TLine* l4=new TLine;

        TCanvas *plot1 = new TCanvas("plot1", "Canvas", 2430,343,2101,1749); 
        plot1->cd();
	gStyle->SetOptStat(0);
        gPad->SetRightMargin(0.17);
        gPad->SetLeftMargin(0.11);
        gPad->SetTopMargin(0.08);
        gPad->SetBottomMargin(0.11);
 
        hist[0]->Draw("p");
        hist[1]->Draw("same p");       

        r1 = 640/cos(13*pi/180.);
        r2 = (640+480)/cos(13*pi/180.);
	for(int ir=-1;ir<=1;ir++){
	for(int pq=0;pq<7;pq++){
        l1->SetLineWidth(2);
        l2->SetLineWidth(2);
        l3->SetLineWidth(2);
        l4->SetLineWidth(2);
       
	if(ir==-1){
        l1->SetLineColor(kRed);
        l2->SetLineColor(kRed);
        l3->SetLineColor(kRed);
        l4->SetLineColor(kRed);
       
	}
	if(ir==0){
        l1->SetLineColor(kGreen);
        l2->SetLineColor(kGreen);
        l3->SetLineColor(kGreen);
        l4->SetLineColor(kGreen);
       
	}
	if(ir==1){
        l1->SetLineColor(kBlack);
        l2->SetLineColor(kBlack);
        l3->SetLineColor(kBlack);
        l4->SetLineColor(kBlack);
       
	}
        x1=r1*cos(((pq+0.5+0.31*ir)*septant-halfaccep)*pi/180.);
        y1=r1*sin(((pq+0.5+0.31*ir)*septant-halfaccep)*pi/180.);

        x2=r2*cos(((pq+0.5+0.31*ir)*septant-halfaccep)*pi/180.);
        y2=r2*sin(((pq+0.5+0.31*ir)*septant-halfaccep)*pi/180.);
        
        x3=r1*cos(((pq+0.5+0.31*ir)*septant+halfaccep)*pi/180.);
        y3=r1*sin(((pq+0.5+0.31*ir)*septant+halfaccep)*pi/180.);

        x4=r2*cos(((pq+0.5+0.31*ir)*septant+halfaccep)*pi/180.);
        y4=r2*sin(((pq+0.5+0.31*ir)*septant+halfaccep)*pi/180.);
        
        l1->DrawLine(x1,y1,x2,y2);
        l2->DrawLine(x3,y3,x4,y4);
        l3->DrawLine(x1,y1,x3,y3);
        l4->DrawLine(x2,y2,x4,y4);
	
	}
	}

}

