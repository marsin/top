#include "group.h"

CmGroup::CmGroup()
{
}


void CmGroup::AddNeighbor(const unsigned int &neighborIndex)
{
    m_neighbors.insert(neighborIndex);
}

void CmGroup::AddMember(const unsigned int &memberIndex)
{
    m_members.insert(memberIndex);
}

std::set<unsigned int> CmGroup::GetMembers()
{
    return m_members;
}

std::set<unsigned int> CmGroup::GetNeighbors()
{
    return m_neighbors;
}

void CmGroup::RemoveMember(const unsigned int &memberIndex)
{
    m_members.erase(memberIndex);
}

void CmGroup::RemoveNeighbor(const unsigned int &neighborIndex)
{
    m_neighbors.erase(neighborIndex);
}

bool CmGroup::MembersContians(const unsigned int &index)
{
    return (m_members.find(index) != m_members.end());
}

bool CmGroup::NeighborsContians(const unsigned int &index)
{
    return (m_neighbors.find(index) != m_neighbors.end());
}

void CmGroup::ClearMembers()
{
  m_members.clear();
}

void CmGroup::SetGroupType(const groupType &type)
{
  m_type = type;
}

groupType CmGroup::GetGroupType()
{
  return m_type;
}

void CmGroup::SetIndex(const unsigned int &index)
{
    m_index = index;
}


unsigned int CmGroup::GetIndex()
{
    return m_index;
}
