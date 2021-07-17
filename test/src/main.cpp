#include <iostream>
#include <filesystem>
#include <fstream>

int main() {
	try {
		std::filesystem::path path("data/text/test.txt");
		if (!std::filesystem::exists(path)) {
			throw std::exception("Error while Opening test.txt!");
		}

		std::string discard;
		int value;
		std::ifstream ifstrm(path);

		if (ifstrm.good()) {
			ifstrm >> value;
		} else {
			throw std::exception("Error while opening output file!");
		}

		std::cout << "value: " << value << std::endl;
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	std::cout << "Bye!" << std::endl;
	std::cin.get();
	return 0;
}