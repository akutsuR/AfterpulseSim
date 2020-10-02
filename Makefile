CXX=g++
LD=g++

CXXFLAGS += -Wall -std=c++11 -g $(shell root-config --cflags) -I$(WCSIMDIR)/include
LDFLAGS += $(shell root-config --ldflags) $(shell root-config --libs) -lTreePlayer -L$(WCSIMDIR) -l WCSimRoot

OBJ1=addNoiseHitsWCSim.o\
	 PMTAfterPulse.o\
	 PMTDark.o\
	 PMTSinglePE.o

.PHONY: clean Execs

%o::%cc
	$(CXX) $(CXXFLAGS) -c $? -o $@

addNoiseHitsWCSim.exe: $(OBJ1)
	$(RM) $@
	$(LD) $^ $(LDFLAGS) -o $@
	@echo "$@ done"
	$(RM) $^

clean:
	$(RM) *.o *.exe
