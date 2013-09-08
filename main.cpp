#include "mainwindow.h"
#include <QApplication>
#include <boost/any.hpp>

#include <QGLWidget>
#include <QtWidgets>
#include "filemanager.h"
#include "errorCodes.h"
#include "model.h"
#include "optimalizationmanager.h"
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QProcess exec;
  std::string appPath = "F:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat";
  std::string scriptPath = "D:\\Python_for_Abaqus\\openmodel.py";
  std::string command = appPath + " cae noGUI=" + scriptPath;
//  exec.start(command.c_str());
//  if(exec.state()>0)
//  {
//    std::cerr << "Command started:";
//    std::cerr << command;
//  }
//  else
//  {
//    std::cerr << "Command fail to start:";
//    std::cerr << command;
//    return a.exec();
//  }
//  exec.waitForFinished();
  int exitcode = exec.exitCode();
  std::cerr << "Abaqus script exited with code: "<<exitcode;
  CmfileManagerp fileManager = CmfileManagerp(new CmfileManager());
  int error;
  CmModelp model = fileManager->OpenModelFile("temp",error);
  CmOptimalizationManagerp optimManager = CmOptimalizationManagerp(new CmOptimalizationManager());

  if(error != ERR_NO_ERROR)
  {
    std::cerr<<"Error opening file";

  }
//  optimManager->Init(model->Parts()[0], error);

  MainWindow w(model);

  w.show();
  w.SetModel(model);

  int maxTextureSize = 1024;
  QGLWidget *widget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
  widget->makeCurrent();




  return a.exec();
}
