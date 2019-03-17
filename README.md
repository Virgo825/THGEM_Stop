ATTENTION: To compile and link this program, a version of Geant4.10 has to be installed.

Change number of layers will reset material.

Simulation 1.8A Neutron react with 0.1um B4C.

Save alpha and Li7 particle information such as outgoing energy, momentum(angle distribution) and energy deposition in Gas(90% Ar and 10% CO2).

How to run?

cd build
cmake ..
make

To run the program in batch mode, please type >./thgem -m ./run.mac.
In run.mac one can set the type and energy of the particle that are fired with the particle gun.
The program can also be run with visualization, for that type >./thgem, and then press the "run" button.
