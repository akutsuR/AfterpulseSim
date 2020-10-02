#!/bin/csh -f

source env.csh
set exe_noise=./addNoiseHitsWCSim.exe
set par_file=NuPRISM_10x8_mPMT_40perCent.parameters.dat 

set in_file_dir=/home/rakutsu/disk/iwcd/neutron/pargun/MuminusUniform1p0GeV_NoDark/out/wcsim_root
set in_noise_file=$in_file_dir/MuminusUniform1p0GeV_NoDark.000.root
set out_noise_file=MuminusUniform1p0GeV_NoDark_addnoise.000.root
set seed=65645879
set add_ap=1
set add_dn=1

$exe_noise -i $in_noise_file\
           -o $out_noise_file\
           -s $seed\
           -a $add_ap\
           -d $add_dn\
           --UseHZC 0\
           --DarkTWinLow -500\
           --DarkTWinUp 30000\
           --DarkRate 1
