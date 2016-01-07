A preliminary set of code has been put  to get background estimates in 6 analysis bins defined as follows
Binning
```
1: 7-8 jets, 2 b-jets, 200 < MHT < 500

2: 7-8 jets, 3+ b-jets, 200 < MHT < 500

3: 7-8 jets, 2+ b-jets, MHT > 500

4: 9+ jets, 2 b-jets, 200 < MHT < 500

5: 9+ jets, 3+ b-jets, 200 < MHT < 500

6: 9+ jets, 2+ b-jets, MHT > 500
```
Commands to run the code
Step1:
```
The following will produce necessary histograms necessary for ZgammaRatio,Double Ratio and prediction 
root -l -q ControlRegion_Input.C+

```
Step2:

```
The following code will produce SigmaIetaIeta distrbutions in different regions of phase space need for purity study.Running this code will produce a root file named PhotonCR_Input.root
root -l -q PhotonCR_Input.C+
```
Step3:
Prerequisite :previous step

```
A simple code has been put to get the purity fits for barrel photons. histograms used  here has a set of loose selection.
root -l -q FitDataEB.C 
```
Step4:
The phenomenological ZgammaRatio is being determined in this step.
```
root -l -q ZGammaRatio.C 

```
Step5:Double Ratio
is comming soon!!


Step6: Prediction
This is the final step of the estimation process , here we will combine all the individual pieces of information. 
```
root -l -q Prediction.C
```



