// th2d_to_csv.C
// For each TH2D in the ROOT file, write one CSV row:
//   col 0  : histogram name
//   col 1+ : mean of ProjectionY for each X bin

void th2d_to_csv(const char* rootFile  = "drs_stop_Run_12341.root",
                 const char* csvFile   = "th2d_means.csv")
{
    TFile *f = TFile::Open(rootFile, "READ");
    if (!f || f->IsZombie()) {
        fprintf(stderr, "Cannot open %s\n", rootFile);
        return;
    }

    FILE *csv = fopen(csvFile, "w");
    if (!csv) {
        fprintf(stderr, "Cannot open output file %s\n", csvFile);
        f->Close();
        return;
    }

    // Collect all TH2D keys
    TIter next(f->GetListOfKeys());
    TKey *key;
    bool headerWritten = false;

    while ((key = (TKey*)next())) {
        if (strcmp(key->GetClassName(), "TH2D") != 0) continue;

        // Skip DWC histograms
        if (TString(key->GetName()).Contains("DWC")) continue;

        TH2D *h = (TH2D*)key->ReadObj();
        if (!h) continue;

        int nX = h->GetNbinsX();

        // Write CSV header on the first histogram (bins labeled 0 to nX-1)
        if (!headerWritten) {
            fprintf(csv, "name");
            for (int ix = 0; ix < nX; ix++)
                fprintf(csv, ",bin%d_mean", ix);
            fprintf(csv, "\n");
            headerWritten = true;
        }

        // Write histogram name
        fprintf(csv, "%s", h->GetName());

        // For each X bin (ROOT 1-indexed), project along Y and get the mean
        for (int ix = 1; ix <= nX; ix++) {
            TH1D *py = h->ProjectionY("_py_tmp", ix, ix);
            double mean = (py->GetEntries() > 0) ? py->GetMean() : 0.0;
            fprintf(csv, ",%.6g", mean);
            delete py;
        }
        fprintf(csv, "\n");

        delete h;
    }

    fclose(csv);
    f->Close();

    printf("Done. Output written to %s\n", csvFile);
}
