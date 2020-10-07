#include <iostream>
#include <string>
#include <stdio.h>

void Tarea2() {
  // Abrimos los datos de Root
  TFile *file = TFile::Open("../../Tracks_Clusters.root");

  // Definimos el árbol llamado "tree" para obtener los datos del archivo *.root
  // El árbol en este archivo de root se llama "JetRecoTree"
  TTree *tree = (TTree*) file->Get("JetRecoTree");
  tree->Print(); // Imprimimos los datos

  /*Extraemos las variables que que queremos de los datos
    Exploraremos dos tipos de jets: "Reco" (reconstruidos del calorimetro) y
    "Truth" (nivel de partcula) con los "Event Weight" activados*/

  float evtw = -1;
  vector<float> *reco_R4_pt = {};
  vector<float> *truth_R4_pt = {};
  vector<float> *reco_R4_jvf = {};
  vector<float> *track_R4_pt = {};


  tree->SetBranchAddress("EventWeight", &evtw);
  tree->SetBranchAddress("RecoJets_R4_pt", &reco_R4_pt);
  tree->SetBranchAddress("TruthJets_R4_pt", &truth_R4_pt);
  tree->SetBranchAddress("RecoJets_R4_jvf", &reco_R4_jvf);
  tree->SetBranchAddress("TrackJets_R4_pt", &track_R4_pt);

  // Creamos el Canvas
  TCanvas *canvas = new TCanvas("Canvas","",800,600);

  // Creamos los histogramas para "Reco" y "leadreco"
  TH1F *hist_leadreco_pt = new TH1F("Lead Reco-jet",
  "Leading jet pT;pT(GeV);Events",50,10,200);

  TH1F *hist_reco_pt = new TH1F("Reco-jet","Jet pT; pT(GeV);Events",50,10,200);

  // Llenamos los histogramas de los RecoJets
  int nentries, nbytes;
  nentries = (Int_t)tree->GetEntries();

  for (int i = 0; i < nentries; i++) {
    nbytes = tree->GetEntry(i);

    if(reco_R4_pt->size()>0){
      hist_leadreco_pt->Fill(reco_R4_pt->at(0)/1000.,evtw);

      for(int j=0; j<reco_R4_pt->size(); j++) {
        hist_reco_pt->Fill(reco_R4_pt->at(j)/1000.,evtw);
      }
    }
  }

  std::cout << "Done!" << std::endl;

  // Dibujamos y guardamos el histograma, este se va a llamar "RecoJet p_T.svg"
  hist_reco_pt->SetMarkerStyle(20);
  hist_reco_pt->SetMarkerColor(kRed);
  hist_reco_pt->Draw("");
  hist_leadreco_pt->SetMarkerStyle(21);
  hist_leadreco_pt->Draw("same");
  canvas->SetLogy();
  canvas->Print("RecoJet p_T.svg");
  canvas->Clear();

  // Ahora realizaremos el mismo procedimiento, pero con los Jets Truth
  // Creamos los histogramas para "truth" y "leadtruth"
  TH1F *hist_leadtruth_pt = new TH1F("Lead Truth-jet",
  "Leading jet pT;pT(GeV);Events",50,10,200);

  TH1F *hist_truth_pt = new TH1F("Truth-jet","Jet pT;pT(GeV);Events",50,10,200);

  // Llenamos los histogramas de los TruthJets
  for (int i = 0; i < nentries; i++) {
    nbytes = tree->GetEntry(i);

    if(truth_R4_pt->size()>0){
      hist_leadtruth_pt->Fill(truth_R4_pt->at(0)/1000.,evtw);

      for(int j=0; j<truth_R4_pt->size(); j++) {
        hist_truth_pt->Fill(truth_R4_pt->at(j)/1000.,evtw);
      }
    }
  }

  std::cout << "Done!" << std::endl;

  // Dibujamos y guardamos el histograma, este se va a llamar "TruthJet p_T.svg"
  hist_truth_pt->SetMarkerStyle(20);
  hist_truth_pt->SetMarkerColor(kRed);
  hist_truth_pt->Draw("");
  hist_leadtruth_pt->SetMarkerStyle(21);
  hist_leadtruth_pt->Draw("same");
  canvas->Print("TruthJet p_T.svg");
  canvas->Clear();

  // Ahora vamos a quitarle los eventos de peso "Event Weight" a los histogramas
  // Empezaremos con los RecoJets
  nentries = (Int_t)tree->GetEntries();

  for (int i = 0; i < nentries; i++) {
    nbytes = tree->GetEntry(i);

    if(reco_R4_pt->size()>0){
      hist_leadreco_pt->Fill(reco_R4_pt->at(0)/1000.);

      for(int j=0; j<reco_R4_pt->size(); j++) {
        hist_reco_pt->Fill(reco_R4_pt->at(j)/1000.);
      }
    }
  }

  std::cout << "Done!" << std::endl;

  /* Dibujamos y guardamos el histograma sin los "Events Weights", este se va a
  llamar "RecoJet p_T SinEventWeights.svg"*/
  hist_reco_pt->SetMarkerStyle(20);
  hist_reco_pt->SetMarkerColor(kRed);
  hist_reco_pt->Draw("");
  hist_leadreco_pt->SetMarkerStyle(21);
  hist_leadreco_pt->Draw("same");
  canvas->Print("RecoJet p_T SinEventWeights.svg");
  canvas->Clear();

  // Seguimos con los TruthJets
  for (int i = 0; i < nentries; i++) {
    nbytes = tree->GetEntry(i);

    if(truth_R4_pt->size()>0){
      hist_leadtruth_pt->Fill(truth_R4_pt->at(0)/1000.);

      for(int j=0; j<truth_R4_pt->size(); j++) {
        hist_truth_pt->Fill(truth_R4_pt->at(j)/1000.);
      }
    }
  }

  /* Dibujamos y guardamos el histograma sin los "Events Weights", este se va a
  llamar "TruthJet p_T SinEventWeights.svg"*/
  hist_reco_pt->SetMarkerStyle(20);
  hist_reco_pt->SetMarkerColor(kRed);
  hist_reco_pt->Draw("");
  hist_leadreco_pt->SetMarkerStyle(21);
  hist_leadreco_pt->Draw("same");
  canvas->Print("TruthJet p_T SinEventWeights.svg");
  canvas->Clear();

  // Ahora estudiaremos la dependencia de los jets a pileup para recojets y
  // Trhuthjets printf

  UInt_t npv = -1;
  tree->SetBranchAddress("NPV", &npv);

  float mu=-1;
  tree->SetBranchAddress("mu_average", &mu);

  // Plot jets pt vs. pile up

  // RecoJets_R4_pt vs. NPV

  TH2 *hist_jetpt_npv = new TH2F("Reco-Jet pT vs. NPV",
  "Reco-Jet pT vs. NPV;NPV;jet pT",50,1,50,20,0,200);

  TProfile *prof_jetpt_npv = new TProfile("Profile Reco-jet pT vs. NPV",
  ";NPV;jet pT",50,1,50, 0, 200);

  // Llenamos los histogramas

  nentries = (Int_t)tree->GetEntries();

  for (int i = 0; i < nentries; i++)
  {
    nbytes = tree->GetEntry(i);

    if(reco_R4_pt->size()!=0 && reco_R4_pt->at(0)>20000.){
      for(int j=0; j<reco_R4_pt->size(); j++){
        hist_jetpt_npv->Fill(reco_R4_pt->at(j)/1000.,npv,evtw);
        prof_jetpt_npv->Fill(reco_R4_pt->at(j)/1000.,npv,evtw);
      }
    }
  }

  std::cout << "Done!" << std::endl;

  /* Dibujamos y guardamos el histograma hist_jetpt_npv, este se va a
  llamar "hist_jetpt_npv.svg"*/
  hist_jetpt_npv->Draw("colz");
  canvas->SetLogy(false);
  canvas->Print("hist_jetpt_npv.svg");
  canvas->Clear();

  // Dibujamos el profile
  prof_jetpt_npv->Draw("");
  canvas->Print("prof_jetpt_npv.svg");
  canvas->Clear();

  // Ahora RecoJets_R4_pt vs. mu
  TH2 *hist_jetpt_mu = new TH2F("Reco-Jet pT vs. mu",
  "Reco-Jet pT vs. mu_average;mu_average;jet pT",50,1,50,20,0,200);

  TProfile *prof_jetpt_mu = new TProfile("Profile Reco-jet pT vs. mu",
  ";mu_average; jet pT",50,1,50, 0, 200);

  // Llenamos los histogramas

  nentries = (Int_t)tree->GetEntries();

  for (int i = 0; i < nentries; i++)
  {
    nbytes = tree->GetEntry(i);

    if(reco_R4_pt->size()!=0 && reco_R4_pt->at(0)>20000.){
      for(int j=0; j<reco_R4_pt->size(); j++){
        hist_jetpt_mu->Fill(reco_R4_pt->at(j)/1000.,mu,evtw);
        prof_jetpt_mu->Fill(reco_R4_pt->at(j)/1000.,mu,evtw);
      }
    }
  }

  std::cout << "Done!" << std::endl;

  /* Dibujamos y guardamos el histograma hist_jetpt_mu, este se va a
  llamar "hist_jetpt_mu.svg"*/
  hist_jetpt_mu->Draw("colz");
  canvas->Print("hist_jetpt_mu.svg");
  canvas->Clear();

  // Dibujamos el profile
  prof_jetpt_mu->Draw("");
  canvas->Print("prof_jetpt_mu.svg");
  canvas->Clear();

  // TrhuthJets_R4_pt vs. NPV
  TH2 *hist_Tjetpt_npv = new TH2F("Truth-Jet pT vs. NPV",
  "Truth-Jet pT vs. NPV;NPV;jet pT",50,10,100,20,0,60);

  TProfile *prof_Tjetpt_npv = new TProfile("Profile Truth-jet pT vs. NPV",
  ";NPV;jet pT",50,1,50, 18,200);

  // Llenamos los histogramas

  nentries = (Int_t)tree->GetEntries();

  for (int i = 0; i < nentries; i++)
  {
    nbytes = tree->GetEntry(i);

    if(truth_R4_pt->size()!=0 && truth_R4_pt->at(0)>20000.){
      for(int j=0; j<truth_R4_pt->size(); j++){
        hist_Tjetpt_npv->Fill(truth_R4_pt->at(j)/1000.,npv,evtw);
        prof_Tjetpt_npv->Fill(truth_R4_pt->at(j)/1000.,npv,evtw);
      }
    }
  }

  std::cout << "Done!" << std::endl;

  /* Dibujamos y guardamos el histograma hist_Tjetpt_npv, este se va a
  llamar "hist_Tjetpt_npv.svg"*/
  hist_Tjetpt_npv->Draw("colz");
  canvas->Print("hist_Tjetpt_npv.svg");
  canvas->Clear();

  // Dibujamos el profile
  prof_Tjetpt_npv->Draw("");
  canvas->Print("prof_Tjetpt_npv.svg");
  canvas->Clear();

  /* Se puede observar que el perfíl de recojet vs. npv se dispersa más,
  conforme npv aumnenta, mientras que truthjet vs. npv permanece lineal y
  tiene poca dispesión*/

  // Ahora RecoJets_R4_pt vs. mu
  TH2 *hist_Tjetpt_mu = new TH2F("Truth-Jet pT vs. mu",
  "Truth-Jet pT vs. mu_average;mu_average;jet pT",50,1,50,20,-5,100);

  TProfile *prof_Tjetpt_mu = new TProfile("Profile Truth-jet pT vs. mu",
  ";mu_average; jet pT",50,1,50, 0, 200);

  // Llenamos los histogramas

  nentries = (Int_t)tree->GetEntries();

  for (int i = 0; i < nentries; i++) {
    nbytes = tree->GetEntry(i);

    if(truth_R4_pt->size()!=0 && truth_R4_pt->at(0)>20000.){
      for(int j=0; j<truth_R4_pt->size(); j++){
        hist_Tjetpt_mu->Fill(truth_R4_pt->at(j)/1000.,mu,evtw);
        prof_Tjetpt_mu->Fill(truth_R4_pt->at(j)/1000.,mu,evtw);
      }
    }
  }

  std::cout << "Done!" << std::endl;

  /* Dibujamos y guardamos el histograma hist_Tjetpt_mu, este se va a
  llamar "hist_Tjetpt_mu.svg"*/
  hist_Tjetpt_mu->Draw("colz");
  canvas->Print("hist_Tjetpt_mu.svg");
  canvas->Clear();

  // Dibujamos el profile
  prof_Tjetpt_mu->Draw("");
  canvas->Print("prof_Tjetpt_mu.svg");
  canvas->Clear();

  /* Ahora usaremos un corte |JVF|>0.5 para comparar el leading jet pT para
  truth jets, reco jets sin el corte y recojets con el corte, este mismo
  procedimiento lo realizaremos con "Track-jets" y verificar que este es
  estable contra el pileup*/

  // Con el corte
  // Generamos los histogramas

  TH1F *hist_leadreco_pt_cut = new TH1F("Lead Reco-jet (cut)",
  "Leading jet pT; pT (GeV); Events", 50, 0, 200);

  TH1F *hist_leadtruth_pt_cut = new TH1F("Lead Truth-jet (cut)",
  "Leading jet pT; pT (GeV); Events", 50, 0, 200);

  TH1F *hist_leadtrack_pt_cut = new TH1F("Lead Track-jet (cut)",
  "Leading jet pT; pT (GeV); Events", 50, 0, 200);

  // Sin el corte

  TH1F *hist_leadreco1_pt = new TH1F("Lead Reco-jet compare",
  "Leading jet pT; pT (GeV); Events", 50, 0, 200);

  TH1F *hist_leadtruth1_pt = new TH1F("Lead Truth-jet compare",
  "Leading jet pT; pT (GeV); Events", 50, 0, 200);

  TH1F *hist_leadtrack_pt = new TH1F("Lead Track-jet compare",
  "Leading jet pT; pT (GeV); Events", 50, 0, 200);

  // Llenamos los histogramas

  for (int i=0; i < nentries; i++) {
    nbytes = tree->GetEntry(i);

    if(reco_R4_pt->size()!=0 &&
    reco_R4_pt->at(0)>20000.){
      hist_leadreco1_pt->Fill(reco_R4_pt->at(0)/1000., evtw);
      hist_leadtruth1_pt->Fill(truth_R4_pt->at(0)/1000., evtw);

      if(std::abs(reco_R4_jvf->at(0)) > 0.5){
        hist_leadreco_pt_cut->Fill(reco_R4_pt->at(0)/1000., evtw);
        hist_leadtruth_pt_cut->Fill(truth_R4_pt->at(0)/1000., evtw);
      }
    }

    if(track_R4_pt->size()!=0){
      hist_leadtrack_pt->Fill(track_R4_pt->at(0)/1000., evtw);

      if(reco_R4_pt->size()!=0 &&
      std::abs(reco_R4_jvf->at(0)) > 0.5){
        hist_leadtrack_pt_cut->Fill(track_R4_pt->at(0)/1000., evtw);
      }
    }
  }

  std::cout << "Done cuts!" << std::endl;
  canvas->SetLogy();

  // Dibujamos los histogramas con el corte
  hist_leadreco_pt_cut->SetMarkerStyle(20);
  hist_leadreco_pt_cut->SetMarkerColor(kRed);
  hist_leadreco_pt_cut->Draw("");

  hist_leadtruth_pt_cut->SetMarkerStyle(20);
  hist_leadtruth_pt_cut->SetMarkerColor(kBlue);
  hist_leadtruth_pt_cut->Draw("Same");

  // Dibujamos los histogramas sin el corte
  hist_leadreco1_pt->SetMarkerStyle(20);
  hist_leadreco1_pt->SetMarkerColor(kOrange);
  hist_leadreco1_pt->Draw("Same");

  hist_leadtruth1_pt->SetMarkerStyle(20);
  hist_leadtruth1_pt->SetMarkerColor(kCyan);
  hist_leadtruth1_pt->Draw("Same");

  canvas->Print("lead_TruthReco_cut.svg");
  canvas->Clear();

  // Comparamos track y reco

  // Dibujamos los histogramas con el corte
  hist_leadreco_pt_cut->SetMarkerStyle(20);
  hist_leadreco_pt_cut->SetMarkerColor(kRed);
  hist_leadreco_pt_cut->Draw("");

  hist_leadtrack_pt_cut->SetMarkerStyle(20);
  hist_leadtrack_pt_cut->SetMarkerColor(kBlue);
  hist_leadtrack_pt_cut->Draw("Same");

  // Dibujamos los histogramas sin el corte
  hist_leadreco1_pt->SetMarkerStyle(20);
  hist_leadreco1_pt->SetMarkerColor(kOrange);
  hist_leadreco1_pt->Draw("Same");

  hist_leadtrack_pt->SetMarkerStyle(20);
  hist_leadtrack_pt->SetMarkerColor(kCyan);
  hist_leadtrack_pt->Draw("Same");

  canvas->Print("lead_TrackReco_cut.svg");
  canvas->Clear();

  /*Ahora estudiaremos la respuesta del Jet: la relación entre el pT del
    jet y los tracksJet. Primero deberíamos "hacer coincidir" los
    jets y los tracks jets. Usaremos la distancia DeltaR entre los dos,
    pero primero debemos definirlos como cuatro vectores:*/

    vector<float> *reco_R4_eta = {};
    vector<float> *reco_R4_phi;
    vector<float> *reco_R4_m;

    vector<float> *truth_R4_eta;
    vector<float> *truth_R4_phi;
    vector<float> *truth_R4_m;

    vector<float> *track_R4_eta;
    vector<float> *track_R4_phi;
    vector<float> *track_R4_m;

    tree->SetBranchAddress("RecoJets_R4_eta", &reco_R4_eta);
    tree->SetBranchAddress("RecoJets_R4_phi", &reco_R4_phi);
    tree->SetBranchAddress("RecoJets_R4_m", &reco_R4_m);
    tree->SetBranchAddress("TruthJets_R4_eta", &truth_R4_eta);
    tree->SetBranchAddress("TruthJets_R4_phi", &truth_R4_phi);
    tree->SetBranchAddress("TruthJets_R4_m", &truth_R4_m);
    tree->SetBranchAddress("TrackJets_R4_eta", &track_R4_eta);
    tree->SetBranchAddress("TrackJets_R4_phi", &track_R4_phi);
    tree->SetBranchAddress("TrackJets_R4_m", &track_R4_m);

    /* Creamos los histogramas en donde se van a comparar los tres casos: RecoJet
    con y sin corte y track-jets, ademas de truth - track*/

    TH1F *hist_DR_reco_track_cut = new TH1F("Delta R reco-track cut",
    "Delta R; #Delta R; Events",20,0,2);

    TH1F *hist_DR_reco_track = new TH1F("Delta R reco-track",
    "Delta R; #Delta R; Events",20,0,2);

    TH1F *hist_DR_track_truth = new TH1F("Delta R track-truth",
    "Delta R; #Delta R; Events",20,0,2);

    // Llenamos los histogramas

    nentries = (Int_t)tree->GetEntries();

    for (int i = 0; i < nentries; i++)
    {
      nbytes = tree->GetEntry(i);

      // Reco track con corte
      if(track_R4_pt->size()!=0 && track_R4_pt->at(0)>20000.){
        TLorentzVector trackJet;
        trackJet.SetPtEtaPhiM(track_R4_pt->at(0),track_R4_eta->at(0),
        track_R4_phi->at(0),track_R4_m->at(0));

        if(reco_R4_pt->size()!=0 && fabs(reco_R4_jvf->at(0))>0.5){
          TLorentzVector recoJet;
          recoJet.SetPtEtaPhiM(reco_R4_pt->at(0),reco_R4_eta->at(0),
          reco_R4_phi->at(0),reco_R4_m->at(0));

          //Plot the Delta R
          hist_DR_reco_track_cut->Fill(trackJet.DeltaR(recoJet),evtw);
        }
      }

      // Reco track sin corte
      if(track_R4_pt->size()!=0 && track_R4_pt->at(0)>20000.){
        TLorentzVector trackJet1;
        trackJet1.SetPtEtaPhiM(track_R4_pt->at(0),track_R4_eta->at(0),
        track_R4_phi->at(0),track_R4_m->at(0));

        if(reco_R4_pt->size()!=0){
          TLorentzVector recoJet1;
          recoJet1.SetPtEtaPhiM(reco_R4_pt->at(0),reco_R4_eta->at(0),
          reco_R4_phi->at(0),reco_R4_m->at(0));

          //Plot the Delta R
          hist_DR_reco_track->Fill(trackJet1.DeltaR(recoJet1),evtw);
        }
      }

      if(truth_R4_pt->size()!=0 && truth_R4_pt->at(0)>20000.){
        TLorentzVector truthJet;
        truthJet.SetPtEtaPhiM(truth_R4_pt->at(0),truth_R4_eta->at(0),
        truth_R4_phi->at(0),truth_R4_m->at(0));

        if(track_R4_pt->size()!=0 && track_R4_pt->at(0)>20000.){
          TLorentzVector trackJet2;
          trackJet2.SetPtEtaPhiM(track_R4_pt->at(0),track_R4_eta->at(0),
          track_R4_phi->at(0),track_R4_m->at(0));

          //Plot the Delta R
          hist_DR_track_truth->Fill(truthJet.DeltaR(trackJet2),evtw);
        }
      }
    }

    std::cout << "Done!" << std::endl;

}
