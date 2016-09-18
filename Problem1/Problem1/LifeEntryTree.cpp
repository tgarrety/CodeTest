#include "LifeEntryTree.h"
#include "LifeEntry.h"
#include "LifeEntryHelpers.h"
#include <algorithm>

LifeEntryTree::LifeEntryTree(int a_minYear, int a_maxYear)
	: m_minYear(a_minYear),
	m_maxYear(a_maxYear),
	m_medianYear((a_maxYear + a_minYear) / 2)
{

}

void LifeEntryTree::addLifeEntry(std::shared_ptr<LifeEntry> a_spEntry)
{
	if (a_spEntry->wasAliveDuring(m_medianYear))
	{
		m_aliveAtMedianYear.push_back(a_spEntry);
	}
	else
	{
		if (a_spEntry->getBirthYear() < m_medianYear)
		{
			if (!m_aliveBeforeMedianYear)
				m_aliveBeforeMedianYear = std::make_shared<LifeEntryTree>(m_minYear, m_medianYear);
			m_aliveBeforeMedianYear->addLifeEntry(a_spEntry);
		}
		else
		{
			if (!m_aliveAfterMedianYear)
			{
				//Special case to allow for right branches where the median year is the max year
				int lowerBound = m_medianYear;
				if (lowerBound == m_minYear)
					lowerBound = m_maxYear;

				m_aliveAfterMedianYear = std::make_shared<LifeEntryTree>(lowerBound, m_maxYear);
			}
			m_aliveAfterMedianYear->addLifeEntry(a_spEntry);
		}
	}
}

int LifeEntryTree::getNumberAliveAtYear(int a_year) const
{
	if (a_year == m_medianYear)
	{
		return m_aliveAtMedianYear.size();
	}

	if (a_year < m_minYear || a_year > m_maxYear)
		return 0;
	int total = 0;
	for (auto iter = m_aliveAtMedianYear.begin(); iter != m_aliveAtMedianYear.cend(); ++iter)
		if ((*iter)->wasAliveDuring(a_year))
			total++;
	if (m_aliveBeforeMedianYear)
		total += m_aliveBeforeMedianYear->getNumberAliveAtYear(a_year);
	if (m_aliveAfterMedianYear)
		total += m_aliveAfterMedianYear->getNumberAliveAtYear(a_year);
	return total;
}

int LifeEntryTree::getNumberAliveAtYearRange(int a_startYear, int a_endYear) const
{
	if ((a_startYear < m_minYear && a_endYear < m_maxYear) || (a_endYear > m_minYear && a_endYear > m_maxYear))
		return 0;
	int aliveTotal = 0;
	for (auto iter = m_aliveAtMedianYear.begin(); iter != m_aliveAtMedianYear.cend(); ++iter)
		if ((*iter)->wasAliveDuring(a_startYear) || (*iter)->wasAliveDuring(a_endYear))
			aliveTotal++;
	if (m_aliveBeforeMedianYear)
		aliveTotal += m_aliveBeforeMedianYear->getNumberAliveAtYearRange(a_startYear, a_endYear);
	if (m_aliveAfterMedianYear)
		aliveTotal += m_aliveAfterMedianYear->getNumberAliveAtYearRange(a_startYear, a_endYear);
	return aliveTotal;
}

LifeEntryTree::SPtr LifeEntryTree::ConstructTree(std::list<std::shared_ptr<LifeEntry> > a_container)
{
	auto treeRoot = std::make_shared<LifeEntryTree>(LifeEntry::MIN_YEAR, LifeEntry::MAX_YEAR);
	for (auto iter = a_container.begin(); iter != a_container.cend(); ++iter)
		treeRoot->addLifeEntry(*iter);
	return treeRoot;
}
