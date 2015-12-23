A very basic code has been put now just to get rough estimates . More details will be updated after the binning is decided 
There are only 6 bins in this version ,HT[>800],MHT[200,500,>750],NJet>=7,Btags[0-1,>2]

commands to run the code
The following will produce necessary histograms necessary for prediction 
root -l PhotonCR_input.C 

The following will print the prediction in 6 bins
root -l Prediction.C
