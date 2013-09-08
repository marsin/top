#ifndef FILESMANAGER_H
#define FILESMANAGER_H
#include "model.h"
#include "part.h"

class CmfileManager
{
public:
    CmfileManager();

    CmModelp OpenModelFile(const std::string &path, int &error);
    void SaveTextToFile(const std::string &text,const std::string &fileName, int &error);
private:

};
typedef boost::shared_ptr<CmfileManager> CmfileManagerp;
#endif // FILESMANAGER_H
