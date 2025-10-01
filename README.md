This repository is a compilation of all the codes and scripts that were developed for the optics reconstruction for the upcoming MOLLER Experiment at Jefferson Lab.

Misc. Information: C12 Form Factors in beam Generator.pdf file was a very early on study that we preformed to prove that the beam generator in remoll incorporated the correct C12 form factors. The result from the beam generator was compared with that obtained with the dedicated C12 generator in remoll.

Step 1: Run the Geant4 simulation with the MOLLER specific remoll framework, Use the script sBatchSubmit.py 

Step 2: With the output from the first step, we create a slim tree (with the SlimGeneral.C macro) keeping only the information from specific virtual detector planes. The most important to us are the GEM related information and the truth information. For this purpose, you will use the sBatchSubmit_SlimGeneral.py script. You do not really need to submit additional jobs for slimming the tree but it makes the process faster.

Step 3: Plot some initial distributions at the GEM Planes. For that you can use the macros provided in the GEM distributions folder. For your reference, some sample GEM distributions are included in the repository.

Step 4: Once you have confidence onto the output you have generated, the next step is to generate the csv files for each sieve hole ( 21 sieve hole x 3 C Foil targets x 4 beam pass energies). For this purpose, you will use the generating_csv_files.C script. The basic idea is to define an elliptical area around the sieve hole images (to remove radiaated particles) and keep particles that fall within the ellipse as our accepted particles to perform the reconstruction. This is achieved via a covariance matrix analysis in r - phi, r' - phi and phi'- phi space.
