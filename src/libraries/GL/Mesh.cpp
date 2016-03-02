#include "Mesh.h"

Mesh::Mesh()
{


}

void Mesh::bind()
{
    glBindVertexArray(m_vao);
}

void Mesh::unbind()
{
    glBindVertexArray(0);
}
