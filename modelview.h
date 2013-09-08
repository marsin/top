#ifndef MODELVIEW_H
#define MODELVIEW_H
#include <QtOpenGL>
#include <QGLWidget>
#include "part.h"
#include "model.h"

class ModelView : public QGLWidget
{
  Q_OBJECT
public:
  explicit ModelView(QWidget *parent = 0);
  void init(const CmModelp& model);

protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();
  void drawVertices();

private:
  CmModelp m_model;
  QTimer mRenderTimer;
  bool m_drawVertices;
  bool m_drawBCs;
  bool m_drawLoads;
  bool m_drawFaces;
  bool m_drawFacesLines;

signals:

public slots:

};

#endif // MODELVIEW_H
