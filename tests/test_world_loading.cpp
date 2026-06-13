#include <iostream>
#include <string>
#include <vector>

#include "world.hpp"

#ifndef VR_PROJECT_SOURCE_DIR
#define VR_PROJECT_SOURCE_DIR "."
#endif

namespace
{
int failures = 0;

void expect(bool condition, const std::string &message)
{
	if (!condition)
	{
		std::cerr << "FAIL: " << message << '\n';
		failures++;
	}
}

std::string source_path(const std::string &relative_path)
{
	return std::string(VR_PROJECT_SOURCE_DIR) + "/" + relative_path;
}

void check_world(const std::string &world_file, int expected_masters, int expected_instances)
{
	World world(source_path(world_file));

	expect(world.is_ready(), world_file + " should parse successfully");
	expect(world.num_masters() == expected_masters, world_file + " should have expected master count");
	expect(world.num_instances() == expected_instances, world_file + " should have expected instance count");

	if (!world.is_ready())
	{
		return;
	}

	int master_index = 0;
	for (const auto &master : world.masters())
	{
		expect(!master.vert.empty(), world_file + " master " + std::to_string(master_index) + " should have vertices");
		expect(master.poly0.size() == master.poly1.size(),
		       world_file + " master " + std::to_string(master_index) + " polygon arrays should match");
		expect(master.poly1.size() == master.poly2.size(),
		       world_file + " master " + std::to_string(master_index) + " polygon arrays should match");
		master_index++;
	}

	int instance_index = 0;
	for (const auto &instance : world.instances())
	{
		expect(instance.masterptr != nullptr,
		       world_file + " instance " + std::to_string(instance_index) + " should have a master");

		if (instance.masterptr != nullptr)
		{
			expect(instance.world_vert.size() == instance.masterptr->vert.size(),
			       world_file + " instance " + std::to_string(instance_index) + " should have transformed vertices");
		}

		instance_index++;
	}
}
} // namespace

int main()
{
	check_world("res/cube.txt", 1, 2);
	check_world("res/office.txt", 3, 26);
	check_world("res/teapot.txt", 1, 2);

	if (failures > 0)
	{
		std::cerr << failures << " test failure(s)\n";
		return 1;
	}

	std::cout << "World loading tests passed\n";
	return 0;
}
