#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QFileDialog>
#include <QtCore>
#include <stdlib.h>
#include <QtWidgets>

MainWindow::MainWindow(const CmModelp &model,QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  //    openFile = new QAction(tr("&Open"),this);
  //    openFile->setStatusTip(tr("Open project file"));
  //connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
  connect(&mRenderTimer, SIGNAL(timeout()), this, SLOT(actionPrintModel()));
  mRenderTimer.start(500);
  ui->setupUi(this);
  ui->modelView->init(model);

  m_model = model;
  m_appPath = "F:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat";
  m_scriptPath = "D:\\Python_for_Abaqus\\openmodel.py";
  m_fileManager = CmfileManagerp(new CmfileManager());
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::SetModel(const CmModelp &model)
{
  m_model = model;
  //modelView->init(m_model);

}

void MainWindow::actionPrintModel()
{
  ui->modelView->init(m_model);
}

//void MainWindow::OpenFileAction()
//{
//    QTextStream cout(stderr);
//
//    fprintf(stderr,"OpenFileAction");
//}



void MainWindow::on_actionQuit_triggered()
{
  this->close();

}

void MainWindow::on_actionOpen_triggered()
{
  int error;
  QString supportedFiles("*.cae");
  supportedFiles = tr("Abaqus Model Database (") + supportedFiles + tr(")");
  QString fileName = QFileDialog::getOpenFileName(this,tr("Open project file"),"",supportedFiles);
  std::string confJson = "{\"Config\": {\n";
  confJson += "\t\"ModelPath\":\"" + fileName.toStdString() + "\"\n";
  confJson += "}}";
  m_fileManager->SaveTextToFile(confJson,"conf",error);
}

void MainWindow::on_actionConfiguration_triggered()
{

}
