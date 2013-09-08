#ifndef OPTIMALIZATIONMANAGER_H
#define OPTIMALIZATIONMANAGER_H

#include <ga/ga.h>
#include "model.h"
#include <boost/shared_ptr.hpp>
#include <ga/GAGenome.h>
#include <ga/GAAllele.h>
#include <ga/GABaseGA.h>
#include <ga/GASStateGA.h>
#include <ga/GAScaling.h>
#include <ga/ga.h>
#include <ga/GA2DArrayGenome.h>
#include "group.h"

void CheckNeighbors(const std::set<unsigned int> &neighbors, CmGroupp &grupa,
                    const std::vector<CmFacep> &faces, const GA1DArrayAlleleGenome<int> &genome,
                    std::set<unsigned int> &set, const int &searchMatIndex);

class CmOptimalizationManager
{
public:
    CmOptimalizationManager();
    void Init(const CmPartp &part, int &error);
private:

    CmPartp m_part;

};
typedef boost::shared_ptr<CmOptimalizationManager> CmOptimalizationManagerp;

#endif // OPTIMALIZATIONMANAGER_H
