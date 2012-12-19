/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef mitkSimulationDrawTool_h
#define mitkSimulationDrawTool_h

#include <SimulationExports.h>
#include <sofa/core/visual/DrawTool.h>

class vtkActor;
class vtkObjectBase;
class vtkProperty;

namespace mitk
{
  /** \brief Utility class used by SOFA classes to draw objects other than visual models.
    *
    * Implements the <tt>sofa::core::visual::DrawTool</tt> interface and replaces the default SOFA OpenGL implementation <tt>sofa::core::visual::DrawToolGL</tt>.
    * All draw methods create a VTK actor and append it to an internal list which can be queried by calling GetActors() to finally draw them.
    * In contrary to the original concept of <tt>DrawToolGL</tt>, which is set once during initialization of SOFA applications, every mitk::Simulation has its own exclusive copy of mitk::SimulationDrawTool.
    * Since SOFA can handle only a single <tt>DrawTool</tt> at once it must be ensured that the correct mitk::SimulationDrawTool is set when switching the active simulation.
    *
    * Each draw method checks if it is necessary to generate a new up-to-date VTK actor, i.e. after a simulation step, or if it is sufficient to use the already generated one.
    * This decision is controlled through the methods Reset() and DisableUpdate().
    * The former method must be called right before a simulation step and mitk::SimulationMapper3D calls the latter method during rendering to disable unnecessary updates until the next simulation step.
    */
  class Simulation_EXPORT SimulationDrawTool : public sofa::core::visual::DrawTool
  {
  public:
    SimulationDrawTool();
    ~SimulationDrawTool();

    /** \brief Disables creation of new VTK actors when calling draw methods.
      */
    void DisableUpdate();

    /** \brief Returns current list of VTK actors which were generated by draw methods since the last reset.
      *
      * \note Do not delete the VTK actors returned by this method.
      */
    std::vector<vtkActor*> GetActors() const;

    /** \brief Clears internal lists of current VTK objects and enables creation of new VTK actors when calling draw methods.
      */
    void Reset();

    void drawPoints(const std::vector<Vector3>& points, float pointSize, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawLines(const std::vector<Vector3>& points, float lineWidth, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawLines(const std::vector<Vector3>& points, const std::vector<Vec2i>& indices, float lineWidth, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawTriangles(const std::vector<Vector3>& points, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawTriangles(const std::vector<Vector3>& points, const Vector3 normal, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawTriangles(const std::vector<Vector3>& points, const std::vector<Vec3i>& indices, const std::vector<Vector3>& normals, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawTriangles(const std::vector<Vector3>& points, const std::vector<Vector3>& normals, const std::vector<Vec4f>& colors);
    void drawTriangleStrip(const std::vector<Vector3>& points, const std::vector<Vector3>& normals, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawTriangleFan(const std::vector<Vector3>& points, const std::vector<Vector3>& normals, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawFrame(const Vector3& position, const Quaternion& orientation, const Vec3f& size);
    void drawSpheres(const std::vector<Vector3>& points, const std::vector<float>& radii, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawSpheres(const std::vector<Vector3>& points, float radius, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawCone(const Vector3& point1, const Vector3& point2, float radius1, float radius2, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f), int subdivisions = 16);
    void drawCube(const float& edgeRadius, const Vec4f& color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f), const int& subdivisions = 16);
    void drawCylinder(const Vector3& point1, const Vector3& point2, float radius, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f), int subdivisions = 16);
    void drawCapsule(const Vector3& point1, const Vector3& point2, float radius, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f), int subdivisions = 16);
    void drawArrow(const Vector3& point1, const Vector3& point2, float radius, const Vec4f color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f), int subdivisions = 16);
    void drawPlus(const float& edgeRadius, const Vec4f& color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f), const int& subdivisions = 16);
    void drawPoint(const Vector3& position, const Vec4f& color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawPoint(const Vector3& position, const Vector3& normal, const Vec4f& color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawTriangle(const Vector3& point1, const Vector3& point2, const Vector3& point3, const Vector3& normal);
    void drawTriangle(const Vector3& point1, const Vector3& point2, const Vector3& point3, const Vector3& normal, const Vec4f& color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    void drawTriangle(const Vector3& point1, const Vector3& point2, const Vector3& point3, const Vector3& normal, const Vec4f& color1, const Vec4f& color2, const Vec4f& color3);
    void drawSphere(const Vector3& position, float radius);
    void pushMatrix();
    void popMatrix();
    void multMatrix(float* matrix);
    void scale(float factor);
    void setMaterial(const Vec4f& color, std::string name = "");
    void resetMaterial(const Vec4f& color, std::string name = "");
    void setPolygonMode(int mode, bool wireframe);
    void setLightingEnabled(bool isEnabled);
    void writeOverlayText(int x, int y, unsigned int fontSize, const Vec4f& color, const char* text);

  private:
    SimulationDrawTool(const SimulationDrawTool&);
    SimulationDrawTool& operator=(const SimulationDrawTool&);

    void DeleteVtkObjects();
    void InitProperty(vtkProperty* property) const;

    std::vector<vtkObjectBase*> m_VtkObjects;
    std::vector<vtkActor*> m_Actors;
    int m_PolygonMode;
    bool m_Wireframe;
    bool m_Update;
  };
}

#endif
