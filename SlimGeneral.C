#include "/work/halla/moller12gev/vdoomra/optics/remoll/include/remolltypes.hh"

void SlimGeneral(TString infile){

   auto fileName = Form("%s",infile.Data());
   auto treeName = "T";
   ROOT::RDataFrame d(treeName, fileName);

   auto primary_hit = "hit.pid==11 && hit.mtrid==0 && (hit.trid==1)";
   auto selected_df = d.Define("prm_e",primary_hit).Define("main","hit.det==28")
     .Define("good_ev","Sum(prm_e && main)").Filter("good_ev>0");

   auto df_small = selected_df.Define("main_trid","hit.trid[main && prm_e][0]")
          .Define("main_r","hit.r[main && prm_e && hit.trid==main_trid][0]")
          .Define("front_sieve_trid","hit.trid[main_trid == hit.trid && prm_e && hit.det == 9000][0]")
          .Define("front_sieve_px","hit.px[main_trid == hit.trid && prm_e && hit.det == 9000][0]")
          .Define("front_sieve_py","hit.py[main_trid == hit.trid && prm_e && hit.det == 9000][0]")
          .Define("front_sieve_pz","hit.pz[main_trid == hit.trid && prm_e && hit.det == 9000][0]")
          .Define("front_sieve_k","hit.k[main_trid == hit.trid && prm_e && hit.det == 9000][0]")
          .Define("behind_sieve_trid","hit.trid[main_trid == hit.trid && prm_e && hit.det == 9001][0]")
          .Define("behind_sieve_px","hit.px[main_trid == hit.trid && prm_e && hit.det == 9001][0]")
          .Define("behind_sieve_py","hit.py[main_trid == hit.trid && prm_e && hit.det == 9001][0]")
          .Define("behind_sieve_pz","hit.pz[main_trid == hit.trid && prm_e && hit.det == 9001][0]")
          .Define("behind_sieve_k","hit.k[main_trid == hit.trid && prm_e && hit.det == 9001][0]")
          .Define("tg_us_px","hit.px[main_trid == hit.trid && prm_e && hit.det == 8000][0]")
          .Define("tg_us_py","hit.py[main_trid == hit.trid && prm_e && hit.det == 8000][0]")
          .Define("tg_us_pz","hit.pz[main_trid == hit.trid && prm_e && hit.det == 8000][0]")
          .Define("tg_us_k","hit.k[main_trid == hit.trid && prm_e && hit.det == 8000][0]")
          .Define("tg_ds_px","hit.px[main_trid == hit.trid && prm_e && hit.det == 8001][0]")
          .Define("tg_ds_py","hit.py[main_trid == hit.trid && prm_e && hit.det == 8001][0]")
          .Define("tg_ds_pz","hit.pz[main_trid == hit.trid && prm_e && hit.det == 8001][0]")
          .Define("tg_ds_k","hit.k[main_trid == hit.trid && prm_e && hit.det == 8001][0]")
          .Define("main_ph","hit.ph[main && prm_e && hit.trid==main_trid][0]")
          .Define("main_x","hit.x[main && prm_e && hit.trid==main_trid][0]")
          .Define("main_y","hit.y[main && prm_e && hit.trid==main_trid][0]")
          .Define("main_px","hit.px[ prm_e && main && hit.trid==main_trid][0]")
          .Define("main_py","hit.py[ prm_e && main && hit.trid==main_trid][0]")
          .Define("main_pz","hit.pz[ prm_e && main && hit.trid==main_trid][0]")
          .Define("sieve_trid","hit.trid[hit.det==270 && prm_e && hit.trid==main_trid][0]")
          .Define("sieve_r","hit.r[hit.det==270 && prm_e && hit.trid==main_trid][0]")
          .Define("sieve_ph","hit.ph[hit.det==270 && prm_e && hit.trid==main_trid][0]")
          .Define("sieve_x","hit.x[hit.det==270 && prm_e && hit.trid==main_trid][0]")
          .Define("sieve_y","hit.y[hit.det==270 && prm_e && hit.trid==main_trid][0]")
          .Define("sieve_z","hit.z[hit.det==270 && prm_e && hit.trid==main_trid][0]")
          .Define("sieve_px","hit.px[hit.det==270 && prm_e && hit.trid==main_trid][0]")
          .Define("sieve_py","hit.py[hit.det==270 && prm_e && hit.trid==main_trid][0]")
          .Define("sieve_pz","hit.pz[hit.det==270 && prm_e && hit.trid==main_trid][0]")
	  .Define("gem1_trid","hit.trid[ prm_e && hit.det==32 && hit.trid==main_trid][0]")
          .Define("gem1_r","hit.r[ prm_e && hit.det==32 && hit.trid==main_trid][0]")
          .Define("gem1_ph","hit.ph[ prm_e && hit.det==32 && hit.trid==main_trid][0]")
	  .Define("gem1_x","hit.x[ prm_e && hit.det==32 && hit.trid==main_trid][0]")
          .Define("gem1_y","hit.y[prm_e && hit.det==32 && hit.trid==main_trid][0]")
	  .Define("gem1_px","hit.px[ prm_e && hit.det==32 && hit.trid==main_trid][0]")
          .Define("gem1_py","hit.py[ prm_e && hit.det==32 && hit.trid==main_trid][0]")
	  .Define("gem1_pz","hit.pz[ prm_e && hit.det==32 && hit.trid==main_trid][0]")
          .Define("gem1_k","hit.k[ prm_e && hit.det==32 && hit.trid==main_trid][0]")
          .Define("gem1_vx","hit.vx[ prm_e && hit.det==32 && hit.trid==main_trid][0]")
          .Define("gem1_vy","hit.vy[ prm_e && hit.det==32 && hit.trid==main_trid][0]")
	  .Define("gem1_vz","hit.vz[ prm_e && hit.det==32 && hit.trid==main_trid][0]")
	  .Define("gem2_trid","hit.trid[ prm_e && hit.det==33 && hit.trid==main_trid][0]")
          .Define("gem2_r","hit.r[ prm_e && hit.det==33 && hit.trid==main_trid][0]")
          .Define("gem2_ph","hit.ph[ prm_e && hit.det==33 && hit.trid==main_trid][0]")
	  .Define("gem2_x","hit.x[ prm_e && hit.det==33 && hit.trid==main_trid][0]")
          .Define("gem2_y","hit.y[ prm_e && hit.det==33 && hit.trid==main_trid][0]")
	  .Define("gem2_px","hit.px[ prm_e && hit.det==33 && hit.trid==main_trid][0]")
          .Define("gem2_py","hit.py[ prm_e && hit.det==33 && hit.trid==main_trid][0]")
	  .Define("gem2_pz","hit.pz[ prm_e && hit.det==33 && hit.trid==main_trid][0]")
          .Define("gem2_k","hit.k[ prm_e && hit.det==33 && hit.trid==main_trid][0]")
	  .Define("gem3_trid","hit.trid[ prm_e && hit.det==34 && hit.trid==main_trid][0]")
          .Define("gem3_r","hit.r[ prm_e && hit.det==34 && hit.trid==main_trid][0]")
          .Define("gem3_ph","hit.ph[ prm_e && hit.det==34 && hit.trid==main_trid][0]")
	  .Define("gem3_x","hit.x[ prm_e && hit.det==34 && hit.trid==main_trid][0]")
          .Define("gem3_y","hit.y[ prm_e && hit.det==34 && hit.trid==main_trid][0]")
	  .Define("gem3_px","hit.px[ prm_e && hit.det==34 && hit.trid==main_trid][0]")
          .Define("gem3_py","hit.py[ prm_e && hit.det==34 && hit.trid==main_trid][0]")
	  .Define("gem3_pz","hit.pz[ prm_e && hit.det==34 && hit.trid==main_trid][0]")
          .Define("gem3_k","hit.k[ prm_e && hit.det==34 && hit.trid==main_trid][0]")
	  .Define("gem4_trid","hit.trid[ prm_e && hit.det==35 && hit.trid==main_trid][0]")
          .Define("gem4_r","hit.r[ prm_e && hit.det==35 && hit.trid==main_trid][0]")
          .Define("gem4_ph","hit.ph[ prm_e && hit.det==35 && hit.trid==main_trid][0]")
	  .Define("gem4_x","hit.x[ prm_e && hit.det==35 && hit.trid==main_trid][0]")
          .Define("gem4_y","hit.y[ prm_e && hit.det==35 && hit.trid==main_trid][0]")
	  .Define("gem4_px","hit.px[ prm_e && hit.det==35 && hit.trid==main_trid][0]")
          .Define("gem4_py","hit.py[ prm_e && hit.det==35 && hit.trid==main_trid][0]")
	  .Define("gem4_pz","hit.pz[ prm_e && hit.det==35 && hit.trid==main_trid][0]")
          .Define("gem4_k","hit.k[ prm_e && hit.det==35 && hit.trid==main_trid][0]")
          .Define("tg_th","part.th[part.trid==main_trid][0]")
          .Define("tg_ph","part.ph[part.trid==main_trid][0]")
          .Define("tg_p","part.p[part.trid==main_trid][0]")
	  .Define("tg_vx","part.vx[part.trid==main_trid][0]")
          .Define("tg_vy","part.vy[part.trid==main_trid][0]")
          .Define("tg_vz","part.vz[part.trid==main_trid][0]")
	  .Define("tg_trid","part.trid[part.trid==main_trid][0]")
          .Define("tg_pid","part.pid[part.trid==main_trid][0]")
          .Define("ev_p","ev.beamp")
          .Define("bm_th","bm.th");

   df_small.Snapshot("newT","o_remoll_slim.root",{ "front_sieve_trid", "front_sieve_px", "front_sieve_py", "front_sieve_pz", "front_sieve_k",
						  "behind_sieve_trid", "behind_sieve_px", "behind_sieve_py", "behind_sieve_pz", "behind_sieve_k",
						  "sieve_r", "main_r", "main_ph", "main_trid", "main_x", "main_y", "main_px", "main_py",
						  "main_pz", "rate" , "gem1_r", "gem1_ph", "sieve_ph", "sieve_px", "sieve_py", "sieve_pz", "gem1_x",
						  "gem1_y", "gem4_x", "gem4_y","gem1_px", "gem1_py", "gem1_pz", "sieve_x", "sieve_y", "tg_th", "tg_ph",
						  "tg_p", "tg_vz", "ev_p", "bm_th"});

	 

}
 
