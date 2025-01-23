#include <iostream>
#include <vector>
#include <string>

void generate_combinations(const std::vector<std::string>& arr,
                           std::vector<std::string>& current,
                           size_t start = 0){
	// Store current combination if not empty
	if(!current.empty()){
		std::string command = "cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ";
		for(const std::string& feature : current) command = command + feature + ' ';
		command += " .. && ninja -j8";
		std::cout << std::endl << command << std::endl;
		std::system(command.c_str());
	}

	// Generate combinations of different lengths
	for(size_t i = start; i < arr.size(); ++i){
		current.push_back(arr[i]);
		generate_combinations(arr, current, i + 1);
		current.pop_back();
	}
}

int main(){
	std::vector<std::string> features = {
		"-DENABLE_BATTERY=ON",
		"-DENABLE_BLUETOOTH=ON",
		"-DENABLE_CPU=ON",
//		"-DENABLE_GPU=ON",
//		"-DENABLE_HARD_DISKS=ON",
//		"-DENABLE_MEMORY=ON",
//		"-DENABLE_OPERATING_SYSTEM=ON",
//		"-DENABLE_PROCESSES=ON",
//		"-DENABLE_SERVICES=ON",
//		"-DENABLE_STARTUP=ON",
//		"-DENABLE_USB=ON",
//		"-DENABLE_WIFI=ON"
	};

	// No features enabled
	std::cout << std::endl << "cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug .. && ninja -j8" << std::endl;
	std::system("cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug .. && ninja -j8");

	// Vector to store current combination
	std::vector<std::string> current;

	// Generate all combinations
	generate_combinations(features, current);

	return 0;
}
