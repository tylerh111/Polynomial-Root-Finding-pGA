#!/bin/bash
mpirun -host localhost,localhost,localhost,localhost ./Polynomial_Root_Finding_pGA input.txt \
	--population-size 5000 --accepted-error 0.0025 --mutation-rate 0.633 --mutation-radius 250 --starting-radius 5000
