#include "LifeEntry.h"

bool LifeEntry::wasAliveDuring(int a_year) const
{
	return a_year >= m_birthYear && a_year <= m_deathYear;
}

bool LifeEntry::operator<(const LifeEntry& a_rhs) const
{
	if (m_birthYear != a_rhs.m_birthYear)
		return m_birthYear < a_rhs.m_birthYear;
	return m_deathYear < a_rhs.m_deathYear;
}

bool LifeEntry::validateYear(int a_year)
{
	return a_year >= MIN_YEAR && a_year <= MAX_YEAR;
}
