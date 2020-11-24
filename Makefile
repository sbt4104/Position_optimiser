run_placer: executable

executable: Port_placement.cpp
	g++ -o placer Port_placement.cpp Require_functions/plotter.cpp Require_functions/port_structure.cpp Require_functions/evaluate_placer.cpp Require_functions/moves_placer.cpp
	./placer
	python plotter.py -blockfile tests/block_3.csv -portfile output/dataset_3_output.csv -connectivityfile tests/connectivity_3.csv

clean:
	rm -rf ./placer
