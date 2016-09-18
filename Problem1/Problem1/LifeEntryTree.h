#ifndef LIFEENTRY_TREE_H
#define LIFEENTRY_TREE_H

#include <memory>
#include <list>

class LifeEntry;

class LifeEntryTree : public std::enable_shared_from_this<LifeEntryTree>
{
public:
	typedef std::shared_ptr<LifeEntryTree> SPtr;
	typedef std::weak_ptr<LifeEntryTree> WPtr;

	LifeEntryTree(int, int);
	virtual ~LifeEntryTree() { }

	void addLifeEntry(std::shared_ptr<LifeEntry>);
	int getNumberAliveAtYear(int) const;
	int getNumberAliveAtYearRange(int, int) const;

	static SPtr ConstructTree(std::list<std::shared_ptr<LifeEntry> >);

private:

	int m_minYear;
	int m_maxYear;
	int m_medianYear;
	std::list<std::shared_ptr<LifeEntry> > m_aliveAtMedianYear;
	SPtr m_aliveBeforeMedianYear;
	SPtr m_aliveAfterMedianYear;
};

#endif