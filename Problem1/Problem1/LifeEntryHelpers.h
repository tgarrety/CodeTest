#ifndef LIFEENTRY_HELPERS_H
#define LIFEENTRY_HELPERS_H

#include "LifeEntry.h"

namespace LifeEntryHelpers
{
	int findMorePopulatedYear(const LifeEntry::Tree&, int, int, int&);
	int findYearWithMostAlive(const LifeEntry::Tree&, int&);
	int findYearWithMostAliveLinear(const LifeEntry::Tree&, int&);
	bool generateYearContainerFile(char*, int);
	bool readYearContainerFromFile(char*, LifeEntry::List&);
};

#endif
