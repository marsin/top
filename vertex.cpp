#include "vertex.h"

vertex::vertex(const unsigned int &number,const float &x_pos,const float &y_pos,const float &z_pos)
{
    m_number = number;
    m_X_pos = x_pos;
    m_Y_pos = y_pos;
    m_Z_pos = z_pos;
}


void vertex::Number(const unsigned int &number)
{
    m_number = number;
}

unsigned int vertex::Number()
{
    return m_number;
}

void vertex::X_pos(const float &x_pos)
{
    m_X_pos = x_pos;
}

float vertex::X_pos()
{
    return m_X_pos;
}

void vertex::Y_pos(const float &y_pos)
{
    m_Y_pos = y_pos;
}

float vertex::Y_pos()
{
    return m_Y_pos;
}

void vertex::Z_pos(const float &z_pos)
{
    m_Z_pos = z_pos;
}

float vertex::Z_pos()
{
    return m_Z_pos;
}

void vertex::SetPosition(const float &x_pos, const float &y_pos, const float &z_pos)
{
    m_X_pos = x_pos;
    m_Y_pos = y_pos;
    m_Z_pos = z_pos;
}
