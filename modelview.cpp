#include "modelview.h"
#include <QVector2D>
#include <QVector3D>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QGLWidget>
#include <GL/glu.h>
#include "vertex.h"

struct VertexData
{
  QVector3D position;
  QVector2D texCoord;
};

ModelView::ModelView(QWidget *parent) :
  QGLWidget(parent)
{
  connect(&mRenderTimer, SIGNAL(timeout()), this, SLOT(update()));
  mRenderTimer.start(500);
  m_model = CmModelp();
  m_drawBCs = true;
  m_drawLoads = true;
  m_drawVertices = true;
  m_drawFaces = true;
  m_drawFacesLines = m_drawFaces;
}

void ModelView::init(const CmModelp &model)
{
  m_model = model;

}


void ModelView::initializeGL()
{
  glShadeModel(GL_SMOOTH);
  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}

void ModelView::resizeGL(int w, int h)
{
  double xMin = 0, xMax = 10, yMin = 0, yMax = 10;
  glViewport(0,0,(GLint)w,(GLint)h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1,1,-1,1);
  if(w < h)
  {
    h = h?h:1;
    double newW = (xMax - xMin) * w / h;
    double difW = newW - (xMax - xMin);
    xMin = 0.0 + difW / 2.0;
    xMax = 10 + difW / 2.0;
  }
  else
  {
    w = w?w:1;
    double newH = (yMax - yMin) * w / h;
    double difH = newH - (yMax - yMin);
    yMin = 0.0 + difH / 2.0;
    yMax = 10 + difH / 2.0;
  }
  gluOrtho2D(xMin,xMax,yMin,yMax);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void ModelView::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glTranslated(5.0,5.0,0.0);
  glLineWidth(1.0);
  glColor3f(0,0.7f,0.7f);

  int sides = 3;
  float radius = 1.0;

//  glBegin(GL_POLYGON);
//  for(int i = 0; i<sides;i++)
//  {
//    glVertex2f(radius*cos(i*2*3.14159/sides),
//               radius*sin(i*2*3.14159/sides));
//  }
//  glEnd();

  glLineWidth(1);
  glColor3f(0,0,0);

//  glBegin(GL_LINE_LOOP);
//  for(int i = 0;i<sides;i++)
//  {
//    glVertex2f(radius*cos(i*2*3.14159/sides),
//               radius*sin(i*2*3.14159/sides));
//  }
//  glEnd();

//  {
//    glBegin(GL_POLYGON);
//    glVertex3f(0.5,0.5,0);
//    glVertex3f(1,0.5,0);
//    glVertex3f(1,1,0);
//    glVertex3f(0.5,1,0);
//    glEnd();
//  }
  float zoom = 10.0;
  float moveOnX = 5.0;
  float moveOnY = 0;
  float moveOnZ = 0;
  if(m_model != 0)
  {
    CmPartp part = m_model->Parts()[0];
    if(m_drawFaces)
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glColor3f(0,1,0);
      std::vector<CmFacep> faces = part->Faces();
      for(std::vector<CmFacep>::iterator it=faces.begin();it!=faces.end();it++)
      {
        glBegin(GL_POLYGON);
        vertexp ver = part->Vertices()[(*it)->BL()];
        glVertex3f((ver->X_pos()/zoom)+moveOnX,(ver->Y_pos()/zoom)+moveOnY,(ver->Z_pos()/zoom)+moveOnZ);
        ver = part->Vertices()[(*it)->BR()];
        glVertex3f((ver->X_pos()/zoom)+moveOnX,(ver->Y_pos()/zoom)+moveOnY,(ver->Z_pos()/zoom)+moveOnZ);
        ver = part->Vertices()[(*it)->TR()];
        glVertex3f((ver->X_pos()/zoom)+moveOnX,(ver->Y_pos()/zoom)+moveOnY,(ver->Z_pos()/zoom)+moveOnZ);
        ver = part->Vertices()[(*it)->TL()];
        glVertex3f((ver->X_pos()/zoom)+moveOnX,(ver->Y_pos()/zoom)+moveOnY,(ver->Z_pos()/zoom)+moveOnZ);
        glEnd();
      }
    }
    if(m_drawFacesLines)
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glColor3f(0,0,0);
      std::vector<CmFacep> faces = part->Faces();
      for(std::vector<CmFacep>::iterator it=faces.begin();it!=faces.end();it++)
      {
        glBegin(GL_LINE_LOOP);
        vertexp ver = part->Vertices()[(*it)->BL()];
        glVertex3f((ver->X_pos()/zoom)+moveOnX,(ver->Y_pos()/zoom)+moveOnY,(ver->Z_pos()/zoom)+moveOnZ);
        ver = part->Vertices()[(*it)->BR()];
        glVertex3f((ver->X_pos()/zoom)+moveOnX,(ver->Y_pos()/zoom)+moveOnY,(ver->Z_pos()/zoom)+moveOnZ);
        ver = part->Vertices()[(*it)->TR()];
        glVertex3f((ver->X_pos()/zoom)+moveOnX,(ver->Y_pos()/zoom)+moveOnY,(ver->Z_pos()/zoom)+moveOnZ);
        ver = part->Vertices()[(*it)->TL()];
        glVertex3f((ver->X_pos()/zoom)+moveOnX,(ver->Y_pos()/zoom)+moveOnY,(ver->Z_pos()/zoom)+moveOnZ);
        glEnd();
      }
    }
    if(m_drawVertices)
    {
      glColor3f(0,0,0);
      glPointSize(2.0);
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);

      std::vector<vertexp> vertices = part->Vertices();
      for(std::vector<vertexp>::iterator it=vertices.begin();it!=vertices.end();it++)
      {
        glBegin(GL_POINTS);
        glVertex3f(((*it)->X_pos()/zoom)+moveOnX,((*it)->Y_pos()/zoom)+moveOnY,((*it)->Z_pos()/zoom)+moveOnZ);
        glEnd();
      }
    }
    if(m_drawBCs)
    {
      glColor3f(0,0,1);
      glPointSize(2.5);
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      std::set<unsigned int> bcs = part->GetAllBCsVerticesIndex();
      for(std::set<unsigned int>::iterator it=bcs.begin();it!=bcs.end();it++)
      {
        vertexp ver = part->Vertices()[*it];
        glBegin(GL_POINTS);
        glVertex3f((ver->X_pos()/zoom)+moveOnX,(ver->Y_pos()/zoom)+moveOnY,(ver->Z_pos()/zoom)+moveOnZ);
        glEnd();
      }
    }
    if(m_drawLoads)
    {
      glColor3f(1,0,0);
      glPointSize(2.5);
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      std::set<unsigned int> loads = part->GetAllLoadsVerticesIndex();
      for(std::set<unsigned int>::iterator it=loads.begin();it!=loads.end();it++)
      {
        vertexp ver = part->Vertices()[*it];
        glBegin(GL_POINTS);
        glVertex3f((ver->X_pos()/zoom)+moveOnX,(ver->Y_pos()/zoom)+moveOnY,(ver->Z_pos()/zoom)+moveOnZ);
        glEnd();
      }
    }

  }
}
