#include "filemanager.h"
#include <iostream>
#include <fstream>
#include "errorCodes.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace boost;

enum elemType {other, faceType, verticeType};

CmfileManager::CmfileManager()
{

}

CmModelp CmfileManager::OpenModelFile(const std::string &path, int &error)
{
  error = ERR_NO_ERROR;
  CmModelp model;
  CmPartp part = CmPartp();
  std::ifstream file;
  const char* temp = path.c_str();
  elemType reading = other;
  file.open(temp,std::ifstream::in);
  if(!file)
  {
    error = ERR_OPEN_FILE;
    return CmModelp();
  }
  while(file.good())
  {
    std::string line;
    std::vector< std::string > sequence;
    std::getline(file,line);
    if(algorithm::find_first(line,"//"))
    {
      continue;
    }
    if(algorithm::contains(line,"#Model:"))
    {
      algorithm::split(sequence,line,algorithm::is_any_of(":"));
      model = CmModelp(new CmModel(sequence[1]));
    }
    else if(boost::algorithm::contains(line,"##Part:"))
    {
      algorithm::split(sequence,line,algorithm::is_any_of(":"));
      part = CmPartp(new CmPart(sequence[1]));
    }
    else if(boost::algorithm::contains(line,"###Faces:"))
    {
      reading = faceType;
      continue;
    }
    else if(boost::algorithm::contains(line,"###Vertices:"))
    {
      reading = verticeType;
      continue;
    }
    else if(boost::algorithm::contains(line,"###Load:"))
    {
      continue;
    }
    else if(boost::algorithm::contains(line,"###BC:"))
    {
      continue;
    }
    else if(boost::algorithm::contains(line,"####LVerticesIndexList:"))
    {
      algorithm::split(sequence,line,algorithm::is_any_of(":"));
      algorithm::split(sequence,sequence[1],algorithm::is_any_of(" "));
      for(size_t it=0;it < sequence.size();it++)
      {
        if(!sequence[it].empty())
          part->AddLoadForVertex(atoi(sequence[it].c_str()),error);
      }
      continue;
    }
    else if(boost::algorithm::contains(line,"####BVerticesIndexList:"))
    {
      algorithm::split(sequence,line,algorithm::is_any_of(":"));
      algorithm::split(sequence,sequence[1],algorithm::is_any_of(" "));
      for(size_t it=0;it < sequence.size();it++)
      {
        if(!sequence[it].empty())
          part->AddBCForVertex(atoi(sequence[it].c_str()),error);
      }
      continue;
    }
    else if(reading == faceType)
    {
      algorithm::split(sequence,line,algorithm::is_any_of(" "));
      unsigned int num = atoi(sequence[0].c_str());
      std::vector<unsigned int> vertices;
      for(size_t it=1;it < sequence.size();it++)
      {
        algorithm::erase_all(sequence[it],"(");
        algorithm::erase_all(sequence[it],",");
        algorithm::erase_all(sequence[it],")");
        vertices.push_back(atoi(sequence[it].c_str()));
      }
      CmFacep face = CmFacep(new CmFace(num,vertices,error));
      face->SetMaterialIndex(1);
      if(error == ERR_NO_ERROR)
        part->AddFace(face);
    }
    else if(reading == verticeType)
    {
      algorithm::split(sequence,line,algorithm::is_any_of(" "));
      if(sequence.size() == 4)
      {
        std::vector<unsigned int> vertices;
        for(size_t it=1;it < sequence.size();it++)
        {
          algorithm::erase_all(sequence[it],"(");
          algorithm::erase_all(sequence[it],",");
          algorithm::erase_all(sequence[it],")");
          vertices.push_back(atoi(sequence[it].c_str()));
        }
        vertexp overtex = vertexp(new vertex(atoi(sequence[0].c_str()),
                                  atof(sequence[1].c_str()),
            atof(sequence[2].c_str()),
            atof(sequence[3].c_str())));
        part->AddVertex(overtex);
      }
    }
  }
  part->SetNumOfHoles(0);
  part->SetupNeighbors();
  model->AddPart(part);
  return model;
}

void CmfileManager::SaveTextToFile(const std::string &text, const std::string &fileName, int &error)
{
  std::ofstream file;
  file.open(fileName.c_str());
  if(file.is_open())
  {
    file << text;
    file.close();
    error = ERR_NO_ERROR;
  }
  else
  {
    error = ERR_OPEN_FILE;
  }
}
