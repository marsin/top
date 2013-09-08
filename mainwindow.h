#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filemanager.h"
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "model.h"
namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(const CmModelp &model,QWidget *parent = 0);
  ~MainWindow();
  void SetModel(const CmModelp &model);
private slots:
  //void OpenFileAction();
  void actionPrintModel();

  void on_actionQuit_triggered();

  void on_actionOpen_triggered();

  void on_actionConfiguration_triggered();

private:
  CmModelp m_model;
  Ui::MainWindow *ui;
  QMenu *fileMenu;
  QMenu *toolMenu;
  QAction *openFile;
  QTimer mRenderTimer;
  std::string m_appPath;
  std::string m_scriptPath;
  CmfileManagerp m_fileManager;
};

#endif // MAINWINDOW_H
