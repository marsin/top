#ifndef GROUP_H
#define GROUP_H
#include <set>
#include <boost/shared_ptr.hpp>

enum groupType {EnMaterial, EnHole, EnBorderEmpty};

class CmGroup
{
public:
    CmGroup();

    void AddNeighbor(const unsigned int &neighborIndex);
    void AddMember(const unsigned int &memberIndex);
    std::set<unsigned int> GetMembers();
    std::set<unsigned int> GetNeighbors();
    void RemoveMember(const unsigned int &memberIndex);
    void RemoveNeighbor(const unsigned int &neighborIndex);
    bool MembersContians(const unsigned int &index);
    bool NeighborsContians(const unsigned int &index);
    void ClearMembers();

    void SetGroupType(const groupType &type);
    groupType GetGroupType();

    void SetIndex(const unsigned int &index);
    unsigned int GetIndex();

private:
    groupType m_type;
    unsigned int m_index;
    std::set<unsigned int> m_members;
    std::set<unsigned int> m_neighbors;
};
typedef boost::shared_ptr<CmGroup> CmGroupp;
#endif // GROUP_H
