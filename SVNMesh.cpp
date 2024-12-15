#include "stdafx.h"
#include "SVNMesh.h"


SVNMesh::SVNMesh(DisplayData* dd) {
    m_data = dd;
}

SVNMesh::~SVNMesh() {
    if (m_data != nullptr) {
        delete m_data;
        m_data = nullptr;
    }
}

bool SVNMesh::FaceMesh::Display() const
{
  /*  //for (unsigned int jj = 0; jj < faceMesh.size(); jj++) {
        const GLfloat red = (GLfloat)color.red();
        const GLfloat green = (GLfloat)color.green();
        const GLfloat blue = (GLfloat)color.blue();
        const GLfloat model_diffuse[4] = { red, green, blue, 0.0f };
        const GLfloat model_specular[4] = { red, green, blue, 0.0f };
        const GLfloat model_shininess = 100.0f;
        glMaterialfv(GL_FRONT, GL_DIFFUSE, model_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, model_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, model_shininess);

        glBegin(GL_TRIANGLES);
        unsigned int nT = (unsigned int)(numIndices / 3);
        for (unsigned int ii = 0; ii < nT; ii++) {
            int ll = 3 * ii + baseIndex;
            int kk = 3 * m_data->triangles[ll];
            glNormal3f(m_data->normalCoords[kk], m_data->normalCoords[kk + 1], m_data->normalCoords[kk + 2]);
            glVertex3f(m_data->faceCoords[kk], m_data->faceCoords[kk + 1], m_data->faceCoords[kk + 2]);
            kk = 3 * m_data->triangles[ll + 1];
            glNormal3f(m_data->normalCoords[kk], m_data->normalCoords[kk + 1], m_data->normalCoords[kk + 2]);
            glVertex3f(m_data->faceCoords[kk], m_data->faceCoords[kk + 1], m_data->faceCoords[kk + 2]);
            kk = 3 * m_data->triangles[ll + 2];
            glNormal3f(m_data->normalCoords[kk], m_data->normalCoords[kk + 1], m_data->normalCoords[kk + 2]);
            glVertex3f(m_data->faceCoords[kk], m_data->faceCoords[kk + 1], m_data->faceCoords[kk + 2]);
        }
        glEnd();
    }
    glLineWidth(1);
    glDisable(GL_LIGHTING);
    for (unsigned int nn = 0; nn < m_data->edgeMesh.size(); nn++) {
        const GLfloat red = (GLfloat)m_data->edgeMesh[nn].color.red();
        const GLfloat green = (GLfloat)m_data->edgeMesh[nn].color.green();
        const GLfloat blue = (GLfloat)m_data->edgeMesh[nn].color.blue();
        glColor3f(red, green, blue);
        glBegin(GL_LINE_STRIP);
        unsigned int nI = (unsigned int)(m_data->edgeMesh[nn].numIndices / 3);
        for (unsigned int ii = 0; ii < nI; ii++) {
            int ll = 3 * ii + m_data->edgeMesh[nn].baseVertex;
            glVertex3f(m_data->edgeCoords[ll], m_data->edgeCoords[ll + 1], m_data->edgeCoords[ll + 2]);
        }
        glEnd();
    }
    glEnable(GL_LIGHTING);*/

    return true;
}
