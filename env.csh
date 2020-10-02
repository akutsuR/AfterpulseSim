#!/bin/csh -f

setenv ROOTSYS /home/skofl/sklib_gcc4.8.5/root_v5.28.00h
setenv WCSIMDIR /disk02/usr6/rakutsu/iwcd/wcsim/WCSimIWCD_TDR_CorrectOffset

if ($?LD_LIBRARY_PATH) then
    setenv LD_LIBRARY_PATH ${ROOTSYS}/lib:$LD_LIBRARY_PATH
else
    setenv LD_LIBRARY_PATH ${ROOTSYS}/lib
endif

setenv LD_LIBRARY_PATH ${WCSIMDIR}:$LD_LIBRARY_PATH
setenv PATH ${ROOTSYS}/bin:${PATH}

rehash
