#include "model.h"

CmModel::CmModel(const std::string &name)
{
    m_name = name;
}

void CmModel::AddPart(const CmPartp &part)
{
    m_parts.push_back(part);
}

void CmModel::Parts(const std::vector<CmPartp> &parts)
{
    m_parts = parts;
}

std::vector<CmPartp> CmModel::Parts()
{
    return m_parts;
}
