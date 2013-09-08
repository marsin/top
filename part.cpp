#include "part.h"
#include "errorCodes.h"

CmPart::CmPart(const std::string &name)
{
  m_name = name;
}

void CmPart::Vertices(const std::vector<vertexp> &vertices)
{
  m_vertices = vertices;
}

std::vector<vertexp> CmPart::Vertices()
{
  return m_vertices;
}

void CmPart::AddVertex(const vertexp &vert)
{
  m_vertices.push_back(vert);
}

void CmPart::Faces(const std::vector<CmFacep>& faces)
{
  m_faces = faces;
}

std::vector<CmFacep> CmPart::Faces()
{
  return m_faces;
}

void CmPart::AddFace(const CmFacep &face)
{
  m_faces.push_back(face);
}

std::set<unsigned int> CmPart::GetAllLoadsVerticesIndex()
{
  return m_verticesLoads;
}

std::set<unsigned int> CmPart::GetAllLoadsIndex()
{
  return m_loads;
}

std::set<unsigned int> CmPart::GetAllBCsIndex()
{
  return m_bcs;
}

bool CmPart::AddLoadForVertex(const unsigned int &vertexIndex, int &error)
{
  error = ERR_FACE_NOT_FOUND;
  m_verticesLoads.insert(vertexIndex);
  for (std::vector<CmFacep>::const_iterator it = m_faces.begin();it != m_faces.end(); it++)
  {
    if(Contains((*it)->VerticesNums(),vertexIndex))
    {
      m_loads.insert((*it)->GetIndex());
      error = ERR_NO_ERROR;
    }
  }
  if(error == ERR_NO_ERROR)
  {
    return true;
  }
  return false;
}

bool CmPart::AddBCForVertex(const unsigned int &vertexIndex, int &error)
{
  error = ERR_FACE_NOT_FOUND;
  m_verticesBCs.insert(vertexIndex);
  for (std::vector<CmFacep>::const_iterator it = m_faces.begin();it != m_faces.end(); it++)
  {
    if(Contains((*it)->VerticesNums(),vertexIndex))
    {
      m_bcs.insert((*it)->GetIndex());
      error = ERR_NO_ERROR;
    }
  }
  if(error == ERR_NO_ERROR)
  {
    return true;
  }
  return false;
}

bool CmPart::Contains(const std::vector<unsigned int> &uintVector, const unsigned int index)
{
  for(std::vector<unsigned int>::const_iterator it = uintVector.begin();
      it != uintVector.end(); it++)
  {
    if(*it == index)
    {
      return true;
    }
  }
  return false;
}

bool CmPart::Contains(const std::set<unsigned int> &uintSet, const unsigned int index)
{
  return (uintSet.find(index) != uintSet.end());
}

void CmPart::SetupNeighbors()
{
  for(std::vector<CmFacep>::iterator faceIt=m_faces.begin();faceIt != m_faces.end();faceIt++)
  {
    std::vector<unsigned int> vertices = (*faceIt)->VerticesNums();
    for(std::vector<CmFacep>::iterator faceIt2=m_faces.begin();faceIt2 != m_faces.end();faceIt2++)
    {
      if((*faceIt)->GetIndex() != (*faceIt2)->GetIndex())
      {
        unsigned int hits = 0;
        std::vector<unsigned int> verticesIt2 = (*faceIt2)->VerticesNums();
        for(std::vector<unsigned int>::iterator it=vertices.begin();it != vertices.end();it++)
        {
          if(std::find(verticesIt2.begin(), verticesIt2.end(), *it)!=verticesIt2.end())
            //if((verticesIt2.find(*it) != verticesIt2.end()))
          {
            hits++;
          }
          if(hits == 2)
          {
            (*faceIt)->AddNeighbor((*faceIt2)->GetIndex());
            hits = 0;
          }
        }
      }
    }
    std::string position = "";
    float pos = m_vertices[vertices[0]]->X_pos();
    for(unsigned int i = 1; i<vertices.size();i++)
    {
      if(pos != m_vertices[vertices[i]]->X_pos())
        break;
      if(i == vertices.size() - 1)
        position = "x";
    }
    if(position == "")
    {
      pos = m_vertices[vertices[0]]->Y_pos();
      for(unsigned int i = 1; i<vertices.size();i++)
      {
        if(pos != m_vertices[vertices[i]]->Y_pos())
          break;
        if(i == vertices.size() - 1)
          position = "y";
      }
    }
    if(position == "")
    {
      pos = m_vertices[vertices[0]]->Z_pos();
      for(unsigned int i = 1; i<vertices.size();i++)
      {
        if(pos != m_vertices[vertices[i]]->Z_pos())
          break;
        if(i == vertices.size() - 1)
          position = "z";
      }
    }
    if(position == "z")
    {
      float minX = m_vertices[vertices[0]]->X_pos();
      float minY = m_vertices[vertices[0]]->Y_pos();
      float maxX = m_vertices[vertices[0]]->X_pos();
      float maxY = m_vertices[vertices[0]]->Y_pos();
      for(unsigned int i = 0; i<vertices.size();i++)
      {
        if(m_vertices[vertices[i]]->X_pos() < minX)
        {
          minX = m_vertices[vertices[i]]->X_pos();
        }
        if(m_vertices[vertices[i]]->X_pos() > maxX)
        {
          maxX = m_vertices[vertices[i]]->X_pos();
        }
        if(m_vertices[vertices[i]]->Y_pos() < minY)
        {
          minY = m_vertices[vertices[i]]->Y_pos();
        }
        if(m_vertices[vertices[i]]->Y_pos() > maxY)
        {
          maxY = m_vertices[vertices[i]]->Y_pos();
        }
      }
      for(unsigned int i = 0; i<vertices.size();i++)
      {
        if(m_vertices[vertices[i]]->Y_pos() == minY && m_vertices[vertices[i]]->X_pos() == minX)
        {
          (*faceIt)->BL(vertices[i]);
          continue;
        }
        if(m_vertices[vertices[i]]->Y_pos() == minY && m_vertices[vertices[i]]->X_pos() == maxX)
        {
          (*faceIt)->BR(vertices[i]);
          continue;
        }
        if(m_vertices[vertices[i]]->Y_pos() == maxY && m_vertices[vertices[i]]->X_pos() == minX)
        {
          (*faceIt)->TL(vertices[i]);
          continue;
        }
        if(m_vertices[vertices[i]]->Y_pos() == maxY && m_vertices[vertices[i]]->X_pos() == maxX)
        {
          (*faceIt)->TR(vertices[i]);
        }
      }
    }
  }
}

void CmPart::SetNumOfHoles(const unsigned int &num)
{
  m_holes = num;
}

unsigned int CmPart::GetNumOfHoles()
{
  return m_holes;
}


std::set<unsigned int> CmPart::GetAllBCsVerticesIndex()
{
  return m_verticesBCs;
}
