#ifndef LIFEENTRY_H
#define LIFEENTRY_H

#include <memory>
#include <list>

class LifeEntryTree;

class LifeEntry
{
public:
	typedef std::shared_ptr<LifeEntry> SPtr;
	typedef std::weak_ptr<LifeEntry> WPtr;
	typedef std::list<SPtr> List;
	typedef LifeEntryTree Tree;

	static const int MIN_YEAR = 1900;
	static const int MAX_YEAR = 2000;
	static const int MAX_YEAR_WIDTH = 4;
	static const int MAX_INPUT_LINE_WIDTH = MAX_YEAR_WIDTH * 2 + 3; //Space for format "YEAR YEAR\n\x00"


	LifeEntry(int a_birthYear, int a_deathYear) { m_birthYear = a_birthYear; m_deathYear = a_deathYear; }

	int getBirthYear() const { return m_birthYear; }
	bool wasAliveDuring(int) const;
	bool operator<(const LifeEntry& a_rhs) const;

	static bool validateYear(int);

private:
	int m_birthYear;
	int m_deathYear;
};
#endif
