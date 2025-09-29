This repository is a compilation of all the codes and scripts that were developed for the optics reconstruction for the upcoming MOLLER Experiment at Jefferson Lab.

Step 1: Run the Geant4 simulation with the MOLLER specific remoll framework, Use the script sBatchSubmit.py 

Step 2: With the output from the first step, we create a slim tree (with the SlimGeneral.C macro) keeping only the information from specific virtual detector planes. The most important to us are the GEM related information and the truth information. For this purpose, you will use the sBatchSubmit_SlimGeneral.py script. You do not really need to submit additional jobs for slimming the tree but it makes the process faster.
