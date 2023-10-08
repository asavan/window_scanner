#include "config.h"
#include <fstream>

ConfigParser::ConfigParser(const std::string& filename) {
	std::ifstream infile(filename, std::fstream::in);
	if (!infile) throw std::runtime_error("No config file " + filename);
	infile >> base_name;
	// ifs >> filename;
	infile >> std::hex >> pr_id;
}

std::string ConfigParser::baseName() const
{
	return base_name;
}

unsigned long long ConfigParser::processId() const
{
	return pr_id;
}
