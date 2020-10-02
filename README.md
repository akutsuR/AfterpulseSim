# AfterpulseSim

A toy MC that adds either or both of aftrepulse and dark noise to each PMT.
This can run on sukap. The following describes steps to run example script on sukap.

- cd <clone location>/AfterpulseSim
- source env.csh
- make clean
- make addNoiseHitsWCSim.exe
- ./run_addnoise.csh

After executing these commands, there will be a ROOT file "addNoiseHitsWCSim_test_out.root". 
The file can be directly fed into fiTQun so that impacts of the noise hits added can be studied easily.
