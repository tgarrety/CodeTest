#include "LifeEntryHelpers.h"
#include "LifeEntryTree.h"
#include <algorithm>
#include <memory>
#include <cmath>
#include <ctime>
#include <chrono>
#include <random>
#include <queue>

int LifeEntryHelpers::findMorePopulatedYear(const LifeEntry::Tree& a_container,
											int a_startYear,
											int a_endYearExclusive, 
											int& a_currentHighestYearPopulation)
{
	const int c_smallYearSlice = 50;
	
	if (a_endYearExclusive - a_startYear == 0)
	{
		return 0;
	}

	if (a_endYearExclusive - a_startYear == 1)
	{
		int totalAlive = a_container.getNumberAliveAtYear(a_startYear);
		return (totalAlive > a_currentHighestYearPopulation) ? a_startYear : 0;
	}

	//Once we have descended 'far enough' down a partition, 
	//check to see if there are even enough entries left to surpass the highest year population so far
	if (a_endYearExclusive - a_startYear <= c_smallYearSlice)
	{
		int totalAlive = a_container.getNumberAliveAtYearRange(a_startYear, a_endYearExclusive);
		if (totalAlive < a_currentHighestYearPopulation)
			return 0;
	}

	int medianYear = (a_startYear + a_endYearExclusive) / 2;
	int medianPopulation = a_container.getNumberAliveAtYear(medianYear);
	
	int highestFoundYearPopulation = std::max(medianPopulation, a_currentHighestYearPopulation);
	int lhsMostPopulatedYear = findMorePopulatedYear(a_container, 
													a_startYear, 
													medianYear, 
													highestFoundYearPopulation);
	int lhsMostPopulatedYearPopulation = highestFoundYearPopulation;
	int rhsMostPopulatedYear = findMorePopulatedYear(a_container, 
													medianYear, 
													a_endYearExclusive, 
													highestFoundYearPopulation);

	//Were we unable to beat the previous highest?
	if (highestFoundYearPopulation == a_currentHighestYearPopulation)
		return 0;
	//We must have found the highest year population so far
	a_currentHighestYearPopulation = highestFoundYearPopulation;
	if (highestFoundYearPopulation == medianPopulation)
		return medianYear;
	if (highestFoundYearPopulation == lhsMostPopulatedYearPopulation)
		return lhsMostPopulatedYear;
	return rhsMostPopulatedYear;
}

int LifeEntryHelpers::findYearWithMostAlive(const LifeEntry::Tree& a_container, int& a_highestYearPopulation)
{
	return findMorePopulatedYear(a_container, 
								LifeEntry::MIN_YEAR,
								LifeEntry::MAX_YEAR + 1, 
								a_highestYearPopulation);
}

int LifeEntryHelpers::findYearWithMostAliveLinear(const LifeEntry::Tree& a_container, int& a_highestYearPopulation)
{
	int yearWithMostAliveSoFar = LifeEntry::MIN_YEAR;
	int totalAliveAtYearWithMost = 0;
	for (int year = LifeEntry::MIN_YEAR; year <= LifeEntry::MAX_YEAR; ++year)
	{
		int totalAlive = a_container.getNumberAliveAtYear(year);
		if (totalAlive > totalAliveAtYearWithMost)
		{
			totalAliveAtYearWithMost = totalAlive;
			yearWithMostAliveSoFar = year;
		}
	}
	a_highestYearPopulation = totalAliveAtYearWithMost;
	return yearWithMostAliveSoFar;
}

bool LifeEntryHelpers::generateYearContainerFile(char* a_fileName, int a_totalYears)
{
	FILE* outputFile = fopen(a_fileName, "w");
	if (outputFile == NULL)
		return false;
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);  // mt19937 is a standard mersenne_twister_engine;
	std::uniform_int_distribution<int> randomDistro(LifeEntry::MIN_YEAR, LifeEntry::MAX_YEAR);
	char yearBuffer[LifeEntry::MAX_INPUT_LINE_WIDTH];
	for (int i = 0; i < a_totalYears; ++i)
	{
		int birthYear = randomDistro(generator);
		int deathYear = birthYear == LifeEntry::MAX_YEAR ? birthYear : (randomDistro(generator) % (LifeEntry::MAX_YEAR - birthYear)) + birthYear;

		int outputWidth = sprintf(yearBuffer, "%d %d\n", birthYear, deathYear);
		if (outputWidth <= 0)
			continue;
		fwrite(yearBuffer, sizeof(char), outputWidth, outputFile);
	}
	fclose(outputFile);
	return true;
}

bool LifeEntryHelpers::readYearContainerFromFile(char* a_fileName, LifeEntry::List& a_outContainer)
{
	a_outContainer.clear();
	FILE* inputFile = fopen(a_fileName, "r");
	if (inputFile == NULL)
	{
		printf("Unable to open input file");
		return false;
	}

	char yearBuffer[LifeEntry::MAX_INPUT_LINE_WIDTH];
	while (fgets(yearBuffer, LifeEntry::MAX_INPUT_LINE_WIDTH, inputFile) != NULL)
	{
		int birthYear, deathYear;
		int outputWidth = sscanf(yearBuffer, "%d %d", &birthYear, &deathYear);
		if (outputWidth != 2)
			continue;

		if (!LifeEntry::validateYear(birthYear) ||
			!LifeEntry::validateYear(deathYear) ||
			birthYear > deathYear)
			continue;

		a_outContainer.push_back(std::make_shared<LifeEntry>(birthYear, deathYear));
	}

	fclose(inputFile);
	
	return true;
}
