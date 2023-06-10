#include "offsets.hpp"
#include <string>
#include <map>

Offsets get_offsets(std::string firmware_name) {
	return offsets_mappings.at(firmware_name);
}