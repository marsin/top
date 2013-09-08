#include "face.h"
#include "errorCodes.h"

CmFace::CmFace(const unsigned int &num,
           const std::vector<unsigned int> &verticesNums,
           int &error)
{
    if(verticesNums.size() == 4)
    {
        m_verticesNums = verticesNums;
        m_num = num;
        error = ERR_NO_ERROR;
    }
    else
    {
        error = ERR_OUT_OF_RANGE;
    }
}

bool CmFace::VerticesNums(const std::vector<unsigned int> &verticesNums)
{
    if(verticesNums.size() > 4)
        return false;
    m_verticesNums = verticesNums;
    return true;
}

std::vector<unsigned int> CmFace::VerticesNums()
{
    return m_verticesNums;
}

unsigned int CmFace::GetIndex()
{
    return m_num;
}

void CmFace::AddNeighbor(const unsigned int &neighborIndex)
{
    m_neighbors.insert(neighborIndex);
}

void CmFace::RemoveNeighbor(const unsigned int &neighborIndex)
{
    m_neighbors.erase(neighborIndex);
}

std::set<unsigned int> CmFace::GetNeighbors()
{
  return m_neighbors;
}

void CmFace::SetMaterialIndex(const unsigned int &materialIndex)
{
  m_materialIndex = materialIndex;
}

unsigned int CmFace::GetMaterialIndex()
{
  return m_materialIndex;
}

void CmFace::BL(const unsigned int &index)
{
  m_BL = index;
}

unsigned int CmFace::BL()
{
  return m_BL;
}

void CmFace::BR(const unsigned int &index)
{
  m_BR = index;
}

unsigned int CmFace::BR()
{
  return m_BR;
}

void CmFace::TL(const unsigned int &index)
{
  m_TL = index;
}

unsigned int CmFace::TL()
{
  return m_TL;
}

void CmFace::TR(const unsigned int &index)
{
  m_TR = index;
}

unsigned int CmFace::TR()
{
  return m_TR;
}
