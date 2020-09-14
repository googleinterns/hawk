#include <iostream>
#include "config.hpp"

int main(int argc, char *argv[])
{
	Config config;
	config.detect_usecase(argc, argv);
	return 0;
}