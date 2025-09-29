#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

const int nFiles = 1;
const int nsectors = 7;

const char* fin[nFiles] = {"../slim_output/slim_moller_with_sieve_p5.root"};

const char* passNames[nFiles] = {"pass5"};
const double pi = acos(-1);

const int nholes = 21;
double phi_hole_lo[nholes] = {0,       2.3*pi/21, 4*pi/21, 6*pi/21,  8.5*pi/21, 9.7*pi/21, 12*pi/21, 14*pi/21, 16*pi/21, 18*pi/21,  20.32*pi/21, 22*pi/21, 24*pi/21, 26.5*pi/21, 27.65*pi/21, 30*pi/21,  32*pi/21, 33.75*pi/21, 36*pi/21, 38*pi/21, 40*pi/21};

const char* sector[nsectors] = {"sector1", "sector2", "sector3", "sector4", "sector5" ,"sector6", "sector7"};
double phi_lo[nsectors] = {0, 2*pi/7, 4*pi/7, 6*pi/7, 8*pi/7, 10*pi/7, 12*pi/7};
double phi_hi[nsectors] = {2*pi/7, 4*pi/7, 6*pi/7, 8*pi/7, 10*pi/7, 12*pi/7, 2*pi};

TH2D* hist_r_phi[nFiles] = {NULL};
TH2D* hist_rprime_phi[nFiles] = {NULL};
TH2D* hist_phiprime_phi[nFiles] = {NULL};
TH2D* Etheta[nFiles] = {NULL};  // E vs theta_tg

TH1D* radial[nFiles][nsectors] = {NULL};
TH1D* phi_dist[nFiles][nsectors] = {NULL};
TH1D* rprime[nFiles][nsectors] = {NULL};

TCanvas* c[nFiles] = {NULL};

void gem_dist_plots(){

  for(int i=0; i<nFiles; i++){

    TFile* f1 = new TFile(Form("%s",fin[i]), "READ");

    TTree* T1 = (TTree*)f1->Get("newT");

    c[i] = new TCanvas(Form("Canvas_%s",passNames[i]));

    int nevents = T1->GetEntries();

    hist_r_phi[i] = new TH2D(Form("gem_r_phi_%s",passNames[i]), Form("gem_dist_%s;r[mm];#phi[rad]",passNames[i]), 600, 500, 1100, 500, 0, 2*pi); 
    hist_rprime_phi[i] = new TH2D(Form("gem_rprime_phi_%s",passNames[i]), Form("gem_dist_%s;r[mm];#phi[rad]",passNames[i]), 500, 0, 0.1, 500, 0, 2*pi); 
    hist_phiprime_phi[i] = new TH2D(Form("gem_phiprime_phi_%s",passNames[i]), Form("gem_dist_%s;r[mm];#phi[rad]",passNames[i]), 500, -0.02, 0.02, 500, 0, 2*pi); 
    Etheta[i] = new TH2D(Form("Etheta_%s",passNames[i]),"E' vs #theta;#theta_{tg}(deg);E'(GeV)",100,0,1.8,100,0,12);
    
    for(int j=0; j<nsectors; j++){

      radial[i][j] = new TH1D(Form("r_%s_%s",passNames[i], sector[j]),"gem_r_distribution;r[mm];Counts",500,500,1300);
      phi_dist[i][j] = new TH1D(Form("phi_%s_%s",passNames[i], sector[j]),"gem_phi_distribution;r[mm];Counts",500,0,2*pi);
      rprime[i][j] = new TH1D(Form("gem_rprime_%s_%s",passNames[i], sector[j]),"gem_r'_distribution;r';Counts",1000, 0, 0.1);

    }

    double sieve_r, gem_r, gem_ph, rate, tg_th, tg_p, gem_px, gem_py, gem_pz, gem_x, gem_y;

    T1->SetBranchAddress("sieve_r",&sieve_r);
    T1->SetBranchAddress("gem1_r",&gem_r);
    T1->SetBranchAddress("gem1_ph",&gem_ph);
    T1->SetBranchAddress("gem1_px",&gem_px);
    T1->SetBranchAddress("gem1_py",&gem_py);
    T1->SetBranchAddress("gem1_pz",&gem_pz);
    T1->SetBranchAddress("gem1_x",&gem_x);
    T1->SetBranchAddress("gem1_y",&gem_y);
    T1->SetBranchAddress("rate",&rate);
    T1->SetBranchAddress("tg_th",&tg_th);
    T1->SetBranchAddress("tg_p",&tg_p);

    for(int j=0; j<nevents; j++){

      T1->GetEntry(j);
      int index_sector = -999;

      rate = rate/400; 
      if(sieve_r < 26.5) continue;
      if(gem_ph<0) gem_ph += 2*pi;

      double r_prime = (gem_x*gem_px + gem_y*gem_py)/(gem_r*gem_pz);
      double phi_prime = (-gem_y*gem_px+gem_x*gem_py)/(gem_r*gem_pz);

      hist_r_phi[i]->Fill(gem_r, gem_ph, rate);
      hist_rprime_phi[i]->Fill(r_prime, gem_ph, rate);
      hist_phiprime_phi[i]->Fill(phi_prime, gem_ph, rate);
      Etheta[i]->Fill((tg_th)*(180/pi), tg_p/1000, rate);
    
      for(int l=0; l<nsectors; l++){  if(gem_ph > phi_lo[l] && gem_ph < phi_hi[l]) index_sector = l; }

      radial[i][index_sector]->Fill(gem_r, rate); 
      phi_dist[i][index_sector]->Fill(gem_ph, rate); 
      rprime[i][index_sector]->Fill(r_prime, rate);

    }

    c[i]->cd();
    hist_rprime_phi[i]->SetLineColor(kBlue);
    hist_rprime_phi[i]->Draw("p");

    for(int ihole = 0; ihole < nholes; ihole++){

      TLine* line1 = new TLine(0, phi_hole_lo[ihole] , 0.1, phi_hole_lo[ihole]);
      line1->SetLineColor(kRed);
      line1->SetLineWidth(2);
      line1->Draw("same");

    }

    c[i]->Draw();

  }

  TFile* fout = new TFile("moller_with_sieve_p5_plots.root", "RECREATE");
  fout->cd();

  for(int i=0; i<nFiles; i++){

    hist_r_phi[i]->Write();
    hist_rprime_phi[i]->Write();
    hist_phiprime_phi[i]->Write();
    Etheta[i]->Write();

    for(int j=0; j<nsectors; j++){

      radial[i][j]->Write();
      phi_dist[i][j]->Write();
      rprime[i][j]->Write();

    }

  }

}
