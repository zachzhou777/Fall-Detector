# Fall Detector
Started working on this during HackMobile, the Qualcomm intern hackathon.

I'm using the MobiFall/MobiAct datasets (bmi.teicrete.gr/the-mobifall-and-mobiact-datasets). The algorithm is currently 98% accurate in detecting falls and suffers from zero false positives on the dataset. My suspicion is that the algorithm fails when the fall happens within the last five seconds of an acclerometer trace, as my algorithm only raises an alert five seconds after a fall has occurred. Currently looking into this.
