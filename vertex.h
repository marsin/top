#ifndef VERTEX_H
#define VERTEX_H
#include <boost/shared_ptr.hpp>

class vertex
{
public:
    vertex(const unsigned int &number,const float &x_pos,const float &y_pos,const float &z_pos);
    virtual ~vertex(){}

    void Number(const unsigned int &number);
    unsigned int Number();

    void X_pos(const float &x_pos);
    float X_pos();
    void Y_pos(const float &y_pos);
    float Y_pos();
    void Z_pos(const float &z_pos);
    float Z_pos();

    void SetPosition(const float &x_pos,const float &y_pos,const float &z_pos);

private:
    unsigned int m_number;
    float m_X_pos;
    float m_Y_pos;
    float m_Z_pos;
};
typedef boost::shared_ptr<vertex> vertexp;

#endif // VERTEX_H
