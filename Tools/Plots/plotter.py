#!/usr/bin/python

import ROOT
from ROOT import *

# Suppress canvas from popping up. Speeds up plots produciton
gROOT.SetBatch()
gStyle.SetOptStat(0)

infile = "../EventLooper/TriphotonSignalKinematics.root"

f = ROOT.TFile(infile, "READ");

def createHist(f, color, obj):
    hist = f.Get(obj)
    hist.SetLineColor(color)
    hist.Scale(1/hist.Integral())
    return hist

def createRatio(h1, h2):
    h3 = h1.Clone("h3")
    h3.SetLineColor(kBlack)
    h3.SetMarkerStyle(21)
    h3.SetTitle("RATIO")
    h3.Sumw2()
    h3.SetStats(0)
    h3.Divide(h2)

    return h3

## main

c = TCanvas("c", "canvas", 500, 500)
c.SetLogy()
hgenpho1pt = createHist(f, kOrange+8, "genPho1Pt")
hgenpho2pt = createHist(f, kBlack,    "genPho2Pt")
hgenpho3pt = createHist(f, kBlue,     "genPho3Pt")
hpho1ptLoose = createHist(f, kOrange, "Pho1PtLoose")
hpho2ptLoose = createHist(f, kGray+3,    "Pho2PtLoose")
hpho3ptLoose = createHist(f, kBlue+3,     "Pho3PtLoose")

# hgenpho1pt.SetFillColor(kOrange-2)
# hgenpho2pt.SetMarkerStyle(3)
# hgenpho3pt.SetMarkerStyle(3)

# hpho3ptLoose.SetLineStyle(2)
# hpho2ptLoose.SetLineStyle(2)
# hpho1ptLoose.SetLineStyle(2)
hpho3ptLoose.SetFillColor(kCyan)
hpho2ptLoose.SetFillColor(kGray)
hpho1ptLoose.SetFillColor(kOrange-2)
hpho3ptLoose.SetFillStyle(3013)
hpho2ptLoose.SetFillStyle(3013)
hpho1ptLoose.SetFillStyle(3013)


hgenpho3pt.Draw("hist, same")
# hgenpho2pt.Draw("hist, same")
# hgenpho1pt.Draw("hist, same")

hpho3ptLoose.Draw("hist, same")
# hpho2ptLoose.Draw("hist, same")
# hpho1ptLoose.Draw("hist, same")

hgenpho3pt.GetXaxis().SetTitle("pT (GeV)")
# hgenpho3pt.GetYaxis().SetTitle("Events / (20 GeV)")

leg = TLegend(0.60, 0.65, 1.0, 0.85)
leg.SetBorderSize(0)
leg.SetFillColor(0)
leg.SetFillStyle(0)
leg.SetTextFont(42)
leg.SetTextSize(0.035)
leg.AddEntry(hgenpho1pt, "#gamma_{1}", "l")
leg.AddEntry(hgenpho2pt, "#gamma_{2}", "l")
leg.AddEntry(hgenpho3pt, "#gamma_{3}", "l")
leg.AddEntry(hpho1ptLoose, "#gamma_{1} LOOSE ID", "f")
leg.AddEntry(hpho2ptLoose, "#gamma_{2} LOOSE ID", "f")
leg.AddEntry(hpho3ptLoose, "#gamma_{3} LOOSE ID", "f")
leg.Draw()

c.Draw()
c.SaveAs("plots/GenPhotonPt.pdf")


cMinv = TCanvas("cMinv", "cMinv", 500, 500)
hgenTriphotonMinv = createHist(f, kOrange+8, "genTriPhotonMinv")

hgenTriphotonMinv.Draw("hist, same")
hgenTriphotonMinv.GetXaxis().SetTitle("M_{#gamma#gamma#gamma} (GeV)")

leg = TLegend(0.60, 0.75, 1.0, 0.85)
leg.SetBorderSize(0)
leg.SetFillColor(0)
leg.SetFillStyle(0)
leg.SetTextFont(42)
leg.SetTextSize(0.035)
leg.AddEntry(hgenpho1pt, "M_{#gamma#gamma#gamma}", "l")
leg.Draw()

cMinv.Draw()
cMinv.SaveAs("plots/TriphotonMinv.pdf")
