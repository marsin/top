#ifndef PART2D_H
#define PART2D_H
#include <boost/shared_ptr.hpp>
#include <vector>
#include <set>
#include "vertex.h"
#include "face.h"

class CmPart
{
public:
    CmPart(const std::string &name);

    void Vertices(const std::vector<vertexp>& vertices);
    std::vector<vertexp> Vertices();
    void AddVertex(const vertexp &vert);

    void Faces(const std::vector<CmFacep> &faces);
    std::vector<CmFacep> Faces();
    void AddFace(const CmFacep &CmFace);

    std::set<unsigned int> GetAllLoadsVerticesIndex();
    std::set<unsigned int> GetAllBCsVerticesIndex();
    std::set<unsigned int> GetAllLoadsIndex();
    std::set<unsigned int> GetAllBCsIndex();
    bool AddLoadForVertex(const unsigned int &vertexIndex, int &error);
    bool AddBCForVertex(const unsigned int &vertexIndex, int &error);

    bool Contains(const std::vector<unsigned int> &uintVector, const unsigned int index);
    bool Contains(const std::set<unsigned int> &uintSet, const unsigned int index);

    void SetupNeighbors();

    void SetNumOfHoles(const unsigned int &num);
    unsigned int GetNumOfHoles();
private:
    std::string m_name;
    std::vector<vertexp> m_vertices;
    std::vector<CmFacep> m_faces;
    std::set<unsigned int> m_loads;
    std::set<unsigned int> m_bcs;
    std::set<unsigned int> m_verticesLoads;
    std::set<unsigned int> m_verticesBCs;
    unsigned int m_holes;
};
typedef boost::shared_ptr<CmPart> CmPartp;

#endif // PART2D_H
