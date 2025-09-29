#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

const int nFiles = 1;
const int nsectors = 7;

const char* fin_moller[nFiles] = { "slim_output/slim_sieve_C12_opticsDS_p2_500.root"};
const char* fin_C12[nFiles] = {"slim_output/slim_sieve_C12_opticsDS_p2_500.root"};

const char* passNames[nFiles] = {"pass5"};
const double pi = acos(-1);

const int nholes = 21;
double phi_hole_lo[nholes] = {0,       2.3*pi/21, 4*pi/21, 6*pi/21,  8.5*pi/21, 9.7*pi/21, 12*pi/21, 14*pi/21, 16*pi/21, 18*pi/21,  20.32*pi/21, 22*pi/21, 24*pi/21, 26.5*pi/21, 27.65*pi/21, 30*pi/21,  32*pi/21, 33.75*pi/21, 36*pi/21, 38*pi/21, 40*pi/21};

const char* sector[nsectors] = {"sector1", "sector2", "sector3", "sector4", "sector5" ,"sector6", "sector7"};
double phi_lo[nsectors] = {0, 2*pi/7, 4*pi/7, 6*pi/7, 8*pi/7, 10*pi/7, 12*pi/7};
double phi_hi[nsectors] = {2*pi/7, 4*pi/7, 6*pi/7, 8*pi/7, 10*pi/7, 12*pi/7, 2*pi};

TH2D* h2d_r_phi_C12[nFiles] = {NULL};
TH2D* h2d_rprime_phi_C12[nFiles] = {NULL};
TH2D* h2d_phiprime_phi_C12[nFiles] = {NULL};

TH1D* main_radial_C12[nFiles] = {NULL};
TH1D* main_radial_moller[nFiles] = {NULL};

TH2D* h2d_r_phi_moller[nFiles] = {NULL};
TH2D* h2d_rprime_phi_moller[nFiles] = {NULL};
TH2D* h2d_phiprime_phi_moller[nFiles] = {NULL};

TH2D* Etheta_C12[nFiles] = {NULL};  // E vs theta_tg
TH2D* Etheta_moller[nFiles] = {NULL};  // E vs theta_tg

TH1D* radial_C12[nFiles][nsectors] = {NULL};
TH1D* rprime_C12[nFiles][nsectors] = {NULL};

TH1D* radial_moller[nFiles][nsectors] = {NULL};
TH1D* rprime_moller[nFiles][nsectors] = {NULL};

TCanvas* c1[nFiles] = {NULL};
TCanvas* c2[nFiles] = {NULL};
TCanvas* c3[nFiles] = {NULL};

void moller_C12(){

  for(int i=0; i<nFiles; i++){

    TFile* f1 = new TFile(Form("%s",fin_C12[i]), "READ");
    TFile* f2 = new TFile(Form("%s",fin_moller[i]), "READ");

    TTree* T1 = (TTree*)f1->Get("newT");
    TTree* T2 = (TTree*)f2->Get("newT");

    c1[i] = new TCanvas(Form("C1_%s",passNames[i]));
    c2[i] = new TCanvas(Form("C2_%s",passNames[i]));
    c3[i] = new TCanvas(Form("C3_%s",passNames[i]));

    int nevents_c12 = T1->GetEntries();

    main_radial_C12[i] = new TH1D("main_r_C12","r_distribution;Counts",500, 500, 1200); 
    main_radial_moller[i] = new TH1D("main_r_moller","r_distribution;Counts",500, 500, 1200); 

    h2d_r_phi_C12[i] = new TH2D("r_phi_C12","r_phi_distribution;phi[rad];Counts",500, 500, 1100, 500, 0, 2*pi); 
    h2d_rprime_phi_C12[i] = new TH2D("rprime_phi_C12","rprime_phi_distribution;phi[rad];Counts", 500, 0 , 0.1, 200, 0, 2*pi);
    h2d_phiprime_phi_C12[i] = new TH2D("phiprime_phi_C12","phiprime_phi_distribution;phi[rad];Counts", 500, -0.02 , 0.02, 200, 0, 2*pi);

    h2d_r_phi_moller[i] = new TH2D("r_phi_moller","r_phi_distribution;phi[rad];Counts",500, 500, 1100, 500, 0, 2*pi); 
    h2d_rprime_phi_moller[i] = new TH2D("rprime_phi_moller","rprime_phi_distribution;phi[rad];Counts", 500, 0 , 0.1, 200, 0, 2*pi);
    h2d_phiprime_phi_moller[i] = new TH2D("phiprime_phi_moller","phiprime_phi_distribution;phi[rad];Counts", 500, -0.02 , 0.02, 200, 0, 2*pi);

    Etheta_C12[i] = new TH2D(Form("Etheta_%s_C12",passNames[i]),"E' vs #theta;#theta_{tg}(deg);E'(GeV)",100,0,1.8,100,0,12);
    Etheta_moller[i] = new TH2D(Form("Etheta_%s_moller",passNames[i]),"E' vs #theta;#theta_{tg}(deg);E'(GeV)",100,0,1.8,100,0,12);

    for(int j=0; j<nsectors; j++){

      radial_C12[i][j] = new TH1D(Form("r_C12_%s_%s",passNames[i], sector[j]),"gem_r_distribution;r[mm];Counts",500,500,1300);
      rprime_C12[i][j] = new TH1D(Form("gem_rprime_C12_%s_%s",passNames[i], sector[j]),"gem_r'_distribution;r';Counts",1000, 0, 0.1);

      radial_moller[i][j] = new TH1D(Form("r_moller_%s_%s",passNames[i], sector[j]),"gem_r_distribution;r[mm];Counts",500,500,1300);
      rprime_moller[i][j] = new TH1D(Form("gem_rprime_moller_%s_%s",passNames[i], sector[j]),"gem_r'_distribution;r';Counts",1000, 0, 0.1);

    }

    double sieve_r_c12, gem_r_c12, gem_ph_c12, rate_c12, tg_th_c12, tg_p_c12, gem_px_c12, gem_py_c12, gem_pz_c12, gem_x_c12, gem_y_c12, main_r_c12;

    T1->SetBranchAddress("sieve_r",&sieve_r_c12);
    T1->SetBranchAddress("gem1_r",&gem_r_c12);
    T1->SetBranchAddress("gem1_ph",&gem_ph_c12);
    T1->SetBranchAddress("gem1_px",&gem_px_c12);
    T1->SetBranchAddress("gem1_py",&gem_py_c12);
    T1->SetBranchAddress("gem1_pz",&gem_pz_c12);
    T1->SetBranchAddress("gem1_x",&gem_x_c12);
    T1->SetBranchAddress("gem1_y",&gem_y_c12);
    T1->SetBranchAddress("rate",&rate_c12);
    T1->SetBranchAddress("tg_th",&tg_th_c12);
    T1->SetBranchAddress("tg_p",&tg_p_c12);
    T1->SetBranchAddress("main_r",&main_r_c12);

    for(int j=0; j<nevents_c12; j++){

      T1->GetEntry(j);
      int index_sector_c12 = -999;

      rate_c12 = rate_c12/(200.); 
      if(sieve_r_c12 < 26.5) continue;
      if(gem_ph_c12<0) gem_ph_c12 += 2*pi;

      double r_prime_c12 = (gem_x_c12*gem_px_c12 + gem_y_c12*gem_py_c12)/(gem_r_c12*gem_pz_c12);
      double phi_prime_c12 = (-gem_y_c12*gem_px_c12+gem_x_c12*gem_py_c12)/(gem_r_c12*gem_pz_c12);

      h2d_r_phi_C12[i]->Fill(gem_r_c12, gem_ph_c12, rate_c12);
      h2d_rprime_phi_C12[i]->Fill(r_prime_c12, gem_ph_c12, rate_c12);
      h2d_phiprime_phi_C12[i]->Fill(phi_prime_c12, gem_ph_c12, rate_c12);
      Etheta_C12[i]->Fill((tg_th_c12)*(180/pi), tg_p_c12/1000, rate_c12);

      main_radial_C12[i]->Fill(main_r_c12, rate_c12);
    
      for(int l=0; l<nsectors; l++){  if(gem_ph_c12 > phi_lo[l] && gem_ph_c12 < phi_hi[l]) index_sector_c12 = l; }

      radial_C12[i][index_sector_c12]->Fill(gem_r_c12, rate_c12); 
      rprime_C12[i][index_sector_c12]->Fill(r_prime_c12, rate_c12);

    }

    int nevents_moller = T2->GetEntries();
 
    double sieve_r_moller, gem_r_moller, gem_ph_moller, rate_moller, tg_th_moller, tg_p_moller, gem_px_moller, gem_py_moller, gem_pz_moller, gem_x_moller, gem_y_moller, main_r_moller;

    T2->SetBranchAddress("sieve_r",&sieve_r_moller);
    T2->SetBranchAddress("gem1_r",&gem_r_moller);
    T2->SetBranchAddress("gem1_ph",&gem_ph_moller);
    T2->SetBranchAddress("gem1_px",&gem_px_moller);
    T2->SetBranchAddress("gem1_py",&gem_py_moller);
    T2->SetBranchAddress("gem1_pz",&gem_pz_moller);
    T2->SetBranchAddress("gem1_x",&gem_x_moller);
    T2->SetBranchAddress("gem1_y",&gem_y_moller);
    T2->SetBranchAddress("rate",&rate_moller);
    T2->SetBranchAddress("tg_th",&tg_th_moller);
    T2->SetBranchAddress("tg_p",&tg_p_moller);
    T2->SetBranchAddress("main_r",&main_r_moller);

    for(int k=0; k<nevents_moller; k++){

      T2->GetEntry(k);
      int index_sector_moller = -999;
      rate_moller = rate_moller/(400.);
      if(sieve_r_moller < 26.5) continue;
      if(gem_ph_moller<0) gem_ph_moller += 2*pi;

      double r_prime_moller = (gem_x_moller*gem_px_moller + gem_y_moller*gem_py_moller)/(gem_r_moller*gem_pz_moller);
      double phi_prime_moller = (-gem_y_moller*gem_px_moller+gem_x_moller*gem_py_moller)/(gem_r_moller*gem_pz_moller);

      h2d_r_phi_moller[i]->Fill(gem_r_moller, gem_ph_moller, rate_moller);
      h2d_rprime_phi_moller[i]->Fill(r_prime_moller, gem_ph_moller, rate_moller);
      h2d_phiprime_phi_moller[i]->Fill(phi_prime_moller, gem_ph_moller, rate_moller);

      main_radial_moller[i]->Fill(main_r_moller, rate_moller);

      Etheta_moller[i]->Fill((tg_th_moller)*(180/pi), tg_p_moller/1000, rate_moller);
    
      for(int l=0; l<nsectors; l++){  if(gem_ph_moller > phi_lo[l] && gem_ph_moller < phi_hi[l]) index_sector_moller = l; }

      radial_moller[i][index_sector_moller]->Fill(gem_r_moller, rate_moller); 
      rprime_moller[i][index_sector_moller]->Fill(r_prime_moller, rate_moller);


    }

    c1[i]->cd();
    h2d_r_phi_C12[i]->SetLineColor(kBlue);
    h2d_r_phi_C12[i]->Draw("p");
    h2d_r_phi_moller[i]->SetLineColor(kGreen);
    h2d_r_phi_moller[i]->Draw("samep");

    for(int ihole = 0; ihole < nholes; ihole++){

      TLine* line1 = new TLine(500 , phi_hole_lo[ihole], 1100, phi_hole_lo[ihole]);
      line1->SetLineColor(kRed);
      line1->SetLineWidth(2);
      line1->Draw("same");

    }

    c2[i]->cd();
    h2d_rprime_phi_C12[i]->SetLineColor(kBlue);
    h2d_rprime_phi_C12[i]->Draw("p");
    h2d_rprime_phi_moller[i]->SetLineColor(kGreen);
    h2d_rprime_phi_moller[i]->Draw("samep");

    c3[i]->cd();
    h2d_phiprime_phi_C12[i]->SetLineColor(kBlue);
    h2d_phiprime_phi_C12[i]->Draw("p");
    h2d_phiprime_phi_moller[i]->SetLineColor(kGreen);
    h2d_phiprime_phi_moller[i]->Draw("samep");

    for(int ihole = 0; ihole < nholes; ihole++){

      TLine* line1 = new TLine(-0.02, phi_hole_lo[ihole] , 0.02, phi_hole_lo[ihole]);
      line1->SetLineColor(kRed);
      line1->SetLineWidth(2);
      line1->Draw("same");

    }

  }

  TFile* fout = new TFile("C12_opticsDS_p2_plots.root", "RECREATE");
  fout->cd();

  for(int i=0; i<nFiles; i++){

    h2d_r_phi_C12[i]->Write();
    h2d_r_phi_moller[i]->Write();

    main_radial_C12[i]->Write();
    main_radial_moller[i]->Write();

    h2d_rprime_phi_C12[i]->Write();
    h2d_rprime_phi_moller[i]->Write();

    h2d_phiprime_phi_C12[i]->Write();
    h2d_phiprime_phi_moller[i]->Write();

    Etheta_C12[i]->Write();
    Etheta_moller[i]->Write();

    for(int j=0; j<nsectors; j++){

      radial_C12[i][j]->Write();
      rprime_C12[i][j]->Write();

      radial_moller[i][j]->Write();
      rprime_moller[i][j]->Write();

    }

  }

}
