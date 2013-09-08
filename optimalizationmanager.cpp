#include "optimalizationmanager.h"
#include "errorCodes.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <QProcess>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>

std::string itos(long n){
  std::stringstream stream;
  stream <<n;
  return stream.str();
}

float objective1D(GAGenome &);

/* ----------------------------------------------------------------------------
   Operator definitions
---------------------------------------------------------------------------- */
// The random initializer sets the elements of the array based on the alleles
// set.  We choose randomly the allele for each element.
template <class ARRAY_TYPE> void
GA1DArrayAlleleGenome<ARRAY_TYPE>::UniformInitializerWithConectivityAnalisis(GAGenome & c)
{
  int error;
  GA1DArrayAlleleGenome<ARRAY_TYPE> &child=
    DYN_CAST(GA1DArrayAlleleGenome<ARRAY_TYPE> &, c);
  CmPartp *part = (CmPartp*)child.userData();
  child.resize(GAGenome::ANY_SIZE); // let chrom resize if it can
  std::set<unsigned int> LoadsSet = (*part)->GetAllLoadsIndex();
  std::set<unsigned int> BCsSet = (*part)->GetAllBCsIndex();

  bool isGenomeOk = false;
  while(!isGenomeOk)
  {
    for(int i=child.length()-1; i>=0; i--)
      child.gene(i, child.alleleset(i).allele());
    (*part)->AddBCForVertex(23,error);
    for(std::set<unsigned int>::iterator it=LoadsSet.begin();
        it != LoadsSet.end(); ++it  )
    {
      if(child.gene(*it) == 0)
      {
        child.gene(*it,1);
      }
    }
    for(std::set<unsigned int>::iterator it=BCsSet.begin();
        it != BCsSet.end(); ++it  )
    {
      if(child.gene(*it) == 0)
      {
        child.gene(*it,1);
      }
    }
    std::vector<CmGroupp> groups;
    std::set<unsigned int> toCheck;
    for(unsigned int i = 0; i<(*part)->Faces().size();i++)
    {
      toCheck.insert(i);
    }
    {
      std::vector<CmFacep> faces = (*part)->Faces();
      std::set<unsigned int>::iterator cIt;
      unsigned int groupIndex = 0;
      std::set<unsigned int> saved;
      while(toCheck.size()>0)
      {
        cIt = toCheck.begin();
        CmGroupp group = CmGroupp(new CmGroup());
        if(child.gene(*cIt)>0)
        {
          group->AddMember(*cIt);
          std::set<unsigned int> neighbors = faces[*cIt]->GetNeighbors();
          toCheck.erase(cIt);
          toCheck.insert(saved.begin(),saved.end());
          CheckNeighbors(neighbors,group,faces,child,toCheck);
          if(group->GetMembers().size()>1 || group->GetNeighbors().size()>0)
          {
            group->SetIndex(groupIndex);
            groups.push_back(group);
            groupIndex++;
          }
        }
        else
        {
          saved.insert(*cIt);
          toCheck.erase(cIt);
        }
      }
    }


    std::set<std::pair<unsigned int,unsigned int> >connectedGroups;
    for(std::vector<CmGroupp>::iterator git=groups.begin();git!=groups.end();git++)
    {
      bool connected = false;
      for(std::vector<CmGroupp>::iterator git2=groups.begin();git2!=groups.end();git2++)
      {
        //        if(!(connectedGroups.find(std::make_pair<unsigned int,unsigned int>((*git)->GetIndex(),(*git2)->GetIndex())) != connectedGroups.end()))
        {
          if((*git)->GetIndex() != (*git2)->GetIndex())
          {
            std::set<unsigned int> neighbors = ((*git)->GetNeighbors());
            std::set<unsigned int> neighbors2 = ((*git2)->GetNeighbors());
            for(std::set<unsigned int>::iterator nit=neighbors.begin();nit!=neighbors.end();nit++)
            {
              if(neighbors2.find(*nit) != neighbors2.end())
              {
                int temp=*nit;
                child.gene(*nit,1);
                //(*git)->RemoveNeighbor(*nit);
                //(*git2)->RemoveNeighbor(*nit);

                //                    connectedGroups.insert(std::make_pair<unsigned int,unsigned int>((*git)->GetIndex(),(*git2)->GetIndex()));
                //                    connectedGroups.insert(std::make_pair<unsigned int,unsigned int>((*git2)->GetIndex(),(*git)->GetIndex()));
                break;
              }
            }
          }
        }
      }
    }
    groups.clear();
    toCheck.clear();
    for(unsigned int i = 0; i<(*part)->Faces().size();i++)
    {
      toCheck.insert(i);
    }
    {
      std::vector<CmFacep> faces = (*part)->Faces();
      std::set<unsigned int>::iterator cIt;
      unsigned int groupIndex = 0;
      while(toCheck.size()>0)
      {
        cIt = toCheck.begin();
        CmGroupp group = CmGroupp(new CmGroup());
        group->AddMember(*cIt);
        std::set<unsigned int> neighbors = faces[*cIt]->GetNeighbors();
        toCheck.erase(cIt);


        CheckNeighbors(neighbors,group,faces,child,toCheck);
        if(group->GetMembers().size()>1 || group->GetNeighbors().size()>0)
        {
          unsigned int contains = 0;
          for(std::set<unsigned int>::iterator lit=LoadsSet.begin();
              lit != LoadsSet.end(); ++lit  )
          {
            if(group->MembersContians(*lit))
            {
              contains++;
            }
          }
          for(std::set<unsigned int>::iterator bit=BCsSet.begin();
              bit != BCsSet.end(); ++bit  )
          {
            if(group->MembersContians(*bit))
            {
              contains++;
            }
          }
          if(contains > 0)
          {
            group->SetIndex(groupIndex);
            groups.push_back(group);
            groupIndex++;
          }
          else
          {
            std::set<unsigned int> members = group->GetMembers();
            for(std::set<unsigned int>::iterator grit=members.begin();grit!=members.end();grit++)
            {
              child.gene(*grit,0);
            }
          }
        }
      }
    }
    int numOfGroups = groups.size();
    if (numOfGroups == 1)
    {
      isGenomeOk = true;
    }
  }
}


CmOptimalizationManager::CmOptimalizationManager()
{
}

void CmOptimalizationManager::Init(const CmPartp &part, int &error)
{
  m_part = part;
  unsigned int seed = 0;
  // TODO: make aset auto settable

  unsigned int ii = 0;
  std::set<unsigned int>::iterator it;
  std::set<unsigned int> LoadsSet = part->GetAllLoadsIndex();
  std::set<unsigned int> BCsSet = part->GetAllBCsIndex();

  std::set<unsigned int> *static_elements = new std::set<unsigned int>;

  static_elements->insert(LoadsSet.begin(),LoadsSet.end());
  static_elements->insert(BCsSet.begin(),BCsSet.end());
  int aset[] = {0,1,0,1};
  GAAlleleSet<int> alleles(2,aset);
  //GA2DArrayAlleleGenome<int> genome(20,20,alleles,objectiveMan);
  CmPartp* partCopy = new CmPartp(part);
  //GA1DBinaryStringGenome genome(part->Faces().size(),objective1D,(void *)partCopy);
  //genome.initializer(GA1DBinaryStringGenome::UniformInitializer);
  //genome.mutator(GA1DBinaryStringGenome::FlipMutator);
  //genome.crossover(GA1DBinaryStringGenome::OnePointCrossover);
  GA1DArrayAlleleGenome<int> genome(part->Faces().size(),alleles,objective1D,(void *)partCopy);
  genome.initializer(GA1DArrayAlleleGenome<int>::UniformInitializerWithConectivityAnalisis);
  genome.mutator(GA1DArrayAlleleGenome<int>::FlipMutator);
  genome.crossover(GA1DArrayGenome<int>::OnePointCrossover);


  GASteadyStateGA ga(genome);

  GASigmaTruncationScaling trunc;
  ga.scaling(trunc);
  GATournamentSelector selector;
  ga.selector(selector);
  ga.set(gaNpopulationSize, 30);
  ga.set(gaNpCrossover, 0.95);
  ga.set(gaNpMutation, 0.01);
  ga.set(gaNnGenerations, 400);
  ga.set(gaNpReplacement, 0.25);
  ga.set(gaNpMigration, 2.0);
  int nbest = 5;
  ga.set(gaNnBestGenomes, nbest);
  ga.set(gaNminimaxi, GAGeneticAlgorithm::MAXIMIZE );

  //ga.parameters(argc, argv);
  int minim = ga.maximize();
  std::cout << "minimize(): " << minim;
  ga.initialize(seed);
  std::cout << "evolving...";
  while(!ga.done()){
    ga.step();
    if(ga.generation() % 50 == 0){
      std::cout << "." << ga.generation(); std::cout.flush();
    }
  }
  int best = ga.nBestGenomes();
  GAStatistics statistics = ga.statistics();
  for(int i=0;i<nbest;i++)
  {
    GA1DArrayAlleleGenome<int> & bestGenome = (GA1DArrayAlleleGenome<int> &)statistics.bestIndividual(i);
    std::ofstream genomeFile;
    std::string final = "G:\\Temp\\final_" + itos(i);
    genomeFile.open(final.c_str());

    if(genomeFile.is_open())
    {
      for(unsigned int it = 0; it < bestGenome.length();it++)
      {
        genomeFile << bestGenome.gene(it);
        genomeFile << " ";
      }
      genomeFile.close();
    }
  }

  std::cout << "Best Genome: " << best;
  std::cout << "done!!";
}

void CheckNeighbors(const std::set<unsigned int> &neighbors, CmGroupp &grupa,
                    const std::vector<CmFacep> &faces, const GA1DArrayAlleleGenome<int> &genome,
                    std::set<unsigned int> &set, const int &searchMatIndex = -1)
{
  std::set<unsigned int>::iterator nIt;
  for(nIt=neighbors.begin();nIt!=neighbors.end();nIt++)
  {
    if(searchMatIndex == -1)
    {
      if(genome.gene(*nIt) > 0)
      {
        if(set.find(*nIt) != set.end())
        {
          grupa->AddMember(*nIt);
          std::set<unsigned int> neighbors = faces[*nIt]->GetNeighbors();
          set.erase(*nIt);
          CheckNeighbors(neighbors,grupa,faces,genome,set);
        }
      }
      else
      {
        grupa->AddNeighbor(*nIt);
      }
    }
    else
    {
      if(genome.gene(*nIt) == searchMatIndex)
      {
        if(set.find(*nIt) != set.end())
        {
          grupa->AddMember(*nIt);
          std::set<unsigned int> neighbors = faces[*nIt]->GetNeighbors();
          set.erase(*nIt);
          CheckNeighbors(neighbors,grupa,faces,genome,set);
        }
      }
      else
      {
        grupa->AddNeighbor(*nIt);
      }
    }
  }
}


float objective1D(GAGenome & c)
{
  GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)c;
  CmPartp *part = (CmPartp*)genome.userData();
  std::set<unsigned int> LoadsSet = (*part)->GetAllLoadsIndex();
  std::set<unsigned int> BCsSet = (*part)->GetAllBCsIndex();
  float value=0.0;
  int count=0;
  unsigned int error = 0;
  bool goodGenome = false;

  for(std::set<unsigned int>::iterator it=LoadsSet.begin();
      it != LoadsSet.end(); ++it  )
  {
    if(genome.gene(*it) == 0)
    {
      genome.gene(*it,1);
    }
  }
  for(std::set<unsigned int>::iterator it=BCsSet.begin();
      it != BCsSet.end(); ++it  )
  {
    if(genome.gene(*it) == 0)
    {
      genome.gene(*it,1);
    }
  }
  std::vector<CmGroupp> groups;
  std::set<unsigned int> toCheck;
//  for(unsigned int i = 0; i<(*part)->Faces().size();i++)
//  {
//    toCheck.insert(i);
//  }
//  {
//    std::vector<CmFacep> faces = (*part)->Faces();
//    std::set<unsigned int>::iterator cIt;
//    unsigned int groupIndex = 0;
//    std::set<unsigned int> saved;
//    while(toCheck.size()>0)
//    {
//      cIt = toCheck.begin();
//      CmGroupp group = CmGroupp(new CmGroup());
//      if(genome.gene(*cIt)>0)
//      {
//        group->AddMember(*cIt);
//        std::set<unsigned int> neighbors = faces[*cIt]->GetNeighbors();
//        toCheck.erase(cIt);
//        toCheck.insert(saved.begin(),saved.end());
//        CheckNeighbors(neighbors,group,faces,genome,toCheck);
//        if(group->GetMembers().size()>1 || group->GetNeighbors().size()>0)
//        {
//          group->SetIndex(groupIndex);
//          groups.push_back(group);
//          groupIndex++;
//        }
//      }
//      else
//      {
//        saved.insert(*cIt);
//        toCheck.erase(cIt);
//      }
//    }
//  }
//  std::set<std::pair<unsigned int,unsigned int> >connectedGroups;
//  for(std::vector<CmGroupp>::iterator git=groups.begin();git!=groups.end();git++)
//  {
//    bool connected = false;
//    for(std::vector<CmGroupp>::iterator git2=groups.begin();git2!=groups.end();git2++)
//    {
//      //        if(!(connectedGroups.find(std::make_pair<unsigned int,unsigned int>((*git)->GetIndex(),(*git2)->GetIndex())) != connectedGroups.end()))
//      {
//        if((*git)->GetIndex() != (*git2)->GetIndex())
//        {
//          std::set<unsigned int> neighbors = ((*git)->GetNeighbors());
//          std::set<unsigned int> neighbors2 = ((*git2)->GetNeighbors());
//          for(std::set<unsigned int>::iterator nit=neighbors.begin();nit!=neighbors.end();nit++)
//          {
//            if(neighbors2.find(*nit) != neighbors2.end())
//            {
//              int stage = 0;
//              CmFacep candidate = (*part)->Faces()[*nit];
//              std::set<unsigned int> mems = (*git)->GetMembers();
//              for(std::set<unsigned int>::iterator itcc=mems.begin();itcc!=mems.end();itcc++)
//              {
//                CmFacep second = (*part)->Faces()[*itcc];
//                for(std::set<unsigned int>::iterator itve=candidate->VerticesNums().begin();
//                    itve!=candidate->VerticesNums().end();itve++)
//                {

//                }
//                if(candidate->VerticesNums().find(*itcc) != candidate->VerticesNums().end())
//                {
//                  stage++;
//                  break;
//                }
//              }
//              genome.gene(*nit,1);
//              //(*git)->RemoveNeighbor(*nit);
//              //(*git2)->RemoveNeighbor(*nit);

//              //                    connectedGroups.insert(std::make_pair<unsigned int,unsigned int>((*git)->GetIndex(),(*git2)->GetIndex()));
//              //                    connectedGroups.insert(std::make_pair<unsigned int,unsigned int>((*git2)->GetIndex(),(*git)->GetIndex()));
//              break;
//            }
//          }
//        }
//      }
//    }
//  }
//  groups.clear();
//  toCheck.clear();
//  for(unsigned int i = 0; i<(*part)->Faces().size();i++)
//  {
//    toCheck.insert(i);
//  }
//  {
//    std::vector<CmFacep> faces = (*part)->Faces();
//    std::set<unsigned int>::iterator cIt;
//    unsigned int groupIndex = 0;
//    while(toCheck.size()>0)
//    {
//      cIt = toCheck.begin();
//      CmGroupp group = CmGroupp(new CmGroup());
//      group->AddMember(*cIt);
//      std::set<unsigned int> neighbors = faces[*cIt]->GetNeighbors();
//      toCheck.erase(cIt);
//      CheckNeighbors(neighbors,group,faces,genome,toCheck);
//      if(group->GetMembers().size()>1 || group->GetNeighbors().size()>0)
//      {
//        unsigned int contains = 0;
//        for(std::set<unsigned int>::iterator lit=LoadsSet.begin();
//            lit != LoadsSet.end(); ++lit  )
//        {
//          if(group->MembersContians(*lit))
//          {
//            contains++;
//          }
//        }
//        for(std::set<unsigned int>::iterator bit=BCsSet.begin();
//            bit != BCsSet.end(); ++bit  )
//        {
//          if(group->MembersContians(*bit))
//          {
//            contains++;
//          }
//        }
//        if(contains > 0)
//        {
//          group->SetIndex(groupIndex);
//          group->SetGroupType(EnMaterial);
//          groups.push_back(group);
//          groupIndex++;
//        }
//        else
//        {
//          std::set<unsigned int> members = group->GetMembers();
//          for(std::set<unsigned int>::iterator grit=members.begin();grit!=members.end();grit++)
//          {
//            genome.gene(*grit,0);
//          }
//        }
//      }

//    }
//  }
//  if(groups.size() > 1)
//  {
//    std::cerr << "Bad genome";
//    return (0);
//  }
  std::vector<CmGroupp> holes;
  toCheck.clear();
  for(unsigned int i = 0; i<(*part)->Faces().size();i++)
  {
    toCheck.insert(i);
  }
  {
    std::vector<CmFacep> faces = (*part)->Faces();
    std::set<unsigned int>::iterator cIt;
    unsigned int groupIndex = 0;
    while(toCheck.size()>0)
    {
      cIt = toCheck.begin();
      CmGroupp group = CmGroupp(new CmGroup());
      group->AddMember(*cIt);
      std::set<unsigned int> neighbors = faces[*cIt]->GetNeighbors();
      toCheck.erase(cIt);
      CheckNeighbors(neighbors,group,faces,genome,toCheck,0);
      group->SetIndex(groupIndex);
      holes.push_back(group);
      groupIndex++;
    }
  }
  //// Print genome into file.
  std::ofstream genomeFile;
  genomeFile.open("G:\\Temp\\genome");
  if(genomeFile.is_open())
  {
    for(unsigned int it = 0; it < genome.length();it++)
    {
      genomeFile << genome.gene(it);
      genomeFile << " ";
    }
    genomeFile.close();
  }

  QProcess exec;
  std::string appPath = "F:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat";
  std::string scriptPath = "D:\\Python_for_Abaqus\\runforgenome.py";
  std::string command = appPath + " cae noGUI=" + scriptPath;
  exec.start(command.c_str());
  exec.waitForFinished();
  int exitcode = exec.exitCode();
  if (exitcode != 0)
  {
    std::cerr << "script failed";
    return 0;
  }
  std::ifstream resultFile;
  resultFile.open("G:\\Temp\\result");
  if(!resultFile)
  {
    error = ERR_OPEN_FILE;
    std::cerr << "error opening file";
    return 0;
  }
  while(resultFile.good())
  {
    std::string line;
    std::getline(resultFile,line);
    value = atof(line.c_str());
    break;
  }
  std::cerr << value;
  std::cerr << "Holes: " << holes.size();
  return(value);
}


