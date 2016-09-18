#include <chrono>
#include <ctime>
#include <memory>
#include "LifeEntry.h"
#include "LifeEntryHelpers.h"
#include "LifeEntryTree.h"

bool findMostPopulatedYear(char* a_fileName);

int main(int argc, char* argv[])
{
	const char* c_generateNewFileCmd = "gen";
	const char* c_findMostPopulatedYearCmd = "use";
	if (argc < 3)
	{
		printf("Incorrect format specified: (executable name) %s/%s) (file)", 
			c_generateNewFileCmd, c_findMostPopulatedYearCmd);
		return 1;
	}

	char* mode = argv[2];
	bool isGenerateMode = strcmp(c_generateNewFileCmd, mode) == 0;
	if (!isGenerateMode && strcmp(c_findMostPopulatedYearCmd, mode) != 0)
	{
		printf("Incorrect format specified: (executable name) (file) [%s count]|[%s]",
			c_generateNewFileCmd, c_findMostPopulatedYearCmd);
		return 1;
	}

	char* fileName = argv[1];
	if (isGenerateMode)
	{
		int count = (argc < 4) ? 0 : atoi(argv[3]);
		if(count < 1)
		{
			printf("Unable to generate specified count");
			return 1;
		}
		LifeEntryHelpers::generateYearContainerFile(fileName, count);
	}
	else
	{
		findMostPopulatedYear(fileName);
	}

    return 0;
}

bool findMostPopulatedYear(char* a_fileName)
{
	LifeEntry::List foundYears;
	bool readSuccess = LifeEntryHelpers::readYearContainerFromFile(a_fileName, foundYears);
	if (!readSuccess || foundYears.size() == 0)
	{
		printf("Error parsing year container from file.");
		return false;
	}

	auto createdTree = LifeEntryTree::ConstructTree(foundYears);

	typedef std::chrono::high_resolution_clock hr_clock;
	std::chrono::time_point<hr_clock> timer = hr_clock::now();

	int totalAliveInMostPopulatedYearLinear = 0;
	int mostPopulatedYearLinear = LifeEntryHelpers::findYearWithMostAliveLinear(*createdTree, totalAliveInMostPopulatedYearLinear);
	double elapsedTimeLinear = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> > > (hr_clock::now() - timer).count();
	printf("Search 1: Year with the most alive: %d with a count of %d alive (%f sec)\n",
		mostPopulatedYearLinear,
		totalAliveInMostPopulatedYearLinear,
		elapsedTimeLinear);

	timer = hr_clock::now();
	int totalAliveInMostPopulatedYear = 0;
	int mostPopulatedYear = LifeEntryHelpers::findYearWithMostAlive(*createdTree, totalAliveInMostPopulatedYear);
	double elapsedTime = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> > > (hr_clock::now() - timer).count();
	printf("Search 2: Year with the most alive: %d with a count of %d alive (%f sec)\n",
			mostPopulatedYear,
			totalAliveInMostPopulatedYear,
			elapsedTime);
	return true;
}

