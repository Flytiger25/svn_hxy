#pragma once
#include <rgbcolor.hxx>
class FACE;
class SVNMesh
{
public:
    struct FaceMesh {
        unsigned int numIndices;
        unsigned int baseVertex;
        unsigned int baseIndex;
        FACE* ptrFace;
        rgb_color color;
        FaceMesh() {
            numIndices = 0;
            baseVertex = 0;
            baseIndex = 0;
            color = 0;
            ptrFace = nullptr;
        }
        bool Display() const;
    };
    struct EdgeMesh {
        unsigned int numIndices;
        unsigned int baseVertex;
        rgb_color color;
        EdgeMesh() {
            numIndices = 0;
            baseVertex = 0;
            color = 0;
        }
    };

    struct DisplayData {
        std::vector<float> faceCoords;
        std::vector<int> triangles;
        std::vector<float> normalCoords;
        std::vector<float> edgeCoords;
        std::vector<SVNMesh::FaceMesh> faceMesh;
        std::vector<SVNMesh::EdgeMesh> edgeMesh;
    };
    // »ñµÃDisplayData
    SVNMesh(DisplayData* dd);
    ~SVNMesh();

private:
    int m_level;
    DisplayData* m_data;

};

