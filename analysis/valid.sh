#!/bin/bash

for i in {12147,12176,12233,112276,12285,12313,12320,12323,12393,12429,12455,12463,12490,12525,12573,12634}
do
	./draw_DWC_ntuple $i 5000
	./closure $i 5000 0 ADCcorrectionEntire ../correction_entire.csv 
	./closure $i 5000 0 ADCcorrectionwoAVG ../correction_entire.csv 
	./closure $i 5000 0 PatchBased ../th2d_means.csv
	./closure_no $i 5000 0 PatchBased ../th2d_means.csv
done
