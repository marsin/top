#ifndef FACE2D_H
#define FACE2D_H
#include <boost/shared_ptr.hpp>
#include <vector>
#include <set>
#include <list>
class CmFace
{
public:
    CmFace(const unsigned int &num,
         const std::vector<unsigned int> &verticesNums,
         int& error);

    bool VerticesNums(const std::vector<unsigned int> &verticesNums);
    std::vector<unsigned int> VerticesNums();
    unsigned int GetIndex();

    void AddNeighbor(const unsigned int &neighborIndex);
    void RemoveNeighbor(const unsigned int &neighborIndex);
    std::set<unsigned int> GetNeighbors();

    void SetMaterialIndex(const unsigned int &materialIndex);
    unsigned int GetMaterialIndex();

    void BL(const unsigned int &index);
    unsigned int BL();
    void BR(const unsigned int &index);
    unsigned int BR();
    void TL(const unsigned int &index);
    unsigned int TL();
    void TR(const unsigned int &index);
    unsigned int TR();
private:

    unsigned int m_BL;
    unsigned int m_BR;
    unsigned int m_TL;
    unsigned int m_TR;
    unsigned int m_materialIndex;
    std::set<unsigned int> m_neighbors;
    std::vector<unsigned int> m_verticesNums;
    unsigned int m_num;
};
typedef boost::shared_ptr<CmFace> CmFacep;

#endif // FACE2D_H
