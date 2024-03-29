#include "vtkActor.h"
#include "vtkActor2D.h"
#include "vtkAxes.h"
#include "vtkAxesActor.h"
#include "vtkCaptionActor2D.h"
#include "vtkDataSetMapper.h"
#include "vtkLabelPlacementMapper.h"
#include "vtkLagrangeHexahedron.h"
#include "vtkPointData.h"
#include "vtkPointSetToLabelHierarchy.h"
#include "vtkPoints.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkStringArray.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkUnstructuredGrid.h"

#include <vector>

int nx = 4;
int dx = 1;

int gpt(int r, int s, int t)
{
  return (r * nx * nx) + (s * nx) + t;
}

int main()
{
  vtkNew<vtkPoints> pts;
  pts->Allocate(nx * nx * nx);

  for (int r = 0; r < nx; ++r) {
    for (int s = 0; s < nx; ++s) {
      for (int t = 0; t < nx; ++t) {
        pts->InsertNextPoint(r * dx, s * dx, t * dx);
      }
    }
  }

  std::vector<int> glid{gpt(0, 0, 0),
                        gpt(nx - 1, 0, 0),
                        gpt(nx - 1, nx - 1, 0),
                        gpt(0, nx - 1, 0),
                        gpt(0, 0, nx - 1),
                        gpt(nx - 1, 0, nx - 1),
                        gpt(nx - 1, nx - 1, nx - 1),
                        gpt(0, nx - 1, nx - 1)};

  // Edge 8, 9
  for (int i = 1; i < nx - 1; ++i) {
    glid.push_back(gpt(i, 0, 0));
  }
  // Edge 10, 11
  for (int i = 1; i < nx - 1; ++i) {
    glid.push_back(gpt(nx - 1, i, 0));
  }
  // Edge 12, 13
  for (int i = 1; i < nx - 1; ++i) {
    glid.push_back(gpt(i, nx - 1, 0));
  }
  // Edge 14, 15
  for (int i = 1; i < nx - 1; ++i) {
    glid.push_back(gpt(0, i, 0));
  }
  // Edge 16, 17
  for (int i = 1; i < nx - 1; ++i) {
    glid.push_back(gpt(i, 0, nx - 1));
  }
  // Edge 18, 19
  for (int i = 1; i < nx - 1; ++i) {
    glid.push_back(gpt(nx - 1, i, nx - 1));
  }
  // Edge 20, 21
  for (int i = 1; i < nx - 1; ++i) {
    glid.push_back(gpt(i, nx - 1, nx - 1));
  }
  // Edge 22, 23
  for (int i = 1; i < nx - 1; ++i) {
    glid.push_back(gpt(0, i, nx - 1));
  }
  // Edge 24, 25
  for (int i = 1; i < nx - 1; ++i) {
    glid.push_back(gpt(0, 0, i));
  }
  // Edge 26, 27
  for (int i = 1; i < nx - 1; ++i) {
    glid.push_back(gpt(nx - 1, 0, i));
  }
  // Edge 30, 31
  for (int i = 1; i < nx - 1; ++i) {
    glid.push_back(gpt(0, nx - 1, i));
  }
  // Edge 28, 29
  for (int i = 1; i < nx - 1; ++i) {
    glid.push_back(gpt(nx - 1, nx - 1, i));
  }

  // Face 40
  for (int t = 1; t < nx - 1; ++t) {
    for (int s = 1; s < nx - 1; ++s) {
      glid.push_back(gpt(0, s, t));
    }
  }
  // Face 44
  for (int t = 1; t < nx - 1; ++t) {
    for (int s = 1; s < nx - 1; ++s) {
      glid.push_back(gpt(nx - 1, s, t));
    }
  }
  // Face 32
  for (int t = 1; t < nx - 1; ++t) {
    for (int r = 1; r < nx - 1; ++r) {
      glid.push_back(gpt(r, 0, t));
    }
  }
  // Face 36
  for (int t = 1; t < nx - 1; ++t) {
    for (int r = 1; r < nx - 1; ++r) {
      glid.push_back(gpt(r, nx - 1, t));
    }
  }
  // Face 48
  for (int s = 1; s < nx - 1; ++s) {
    for (int r = 1; r < nx - 1; ++r) {
      glid.push_back(gpt(r, s, 0));
    }
  }
  // Face 52
  for (int s = 1; s < nx - 1; ++s) {
    for (int r = 1; r < nx - 1; ++r) {
      glid.push_back(gpt(r, s, nx - 1));
    }
  }

  // Interior
  for (int t = 1; t < nx - 1; ++t) {
    for (int s = 1; s < nx - 1; ++s) {
      for (int r = 1; r < nx - 1; ++r) {
        glid.push_back(gpt(r, s, t));
      }
    }
  }

  vtkNew<vtkLagrangeHexahedron> hex;
  hex->GetPointIds()->SetNumberOfIds(nx * nx * nx);
  hex->GetPoints()->SetNumberOfPoints(nx * nx * nx);
  hex->Initialize();

  vtkNew<vtkStringArray> labels;
  labels->SetName("Global Vertex IDs");
  for (int locid = 0; locid < glid.size(); ++locid) {
    hex->GetPointIds()->SetId(locid, glid[locid]);
    labels->InsertValue(glid[locid], std::to_string(locid));
  }

  vtkNew<vtkUnstructuredGrid> hexGrid;
  hexGrid->Allocate();
  hexGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());
  hexGrid->SetPoints(pts);

  //================================================================
  // Labeling: https://www.programmersought.com/article/2153142323/
  //================================================================

  hexGrid->GetPointData()->AddArray(labels);

  vtkNew<vtkTextProperty> textProp;
  textProp->SetFontSize(35);

  vtkNew<vtkPointSetToLabelHierarchy> hie;
  hie->SetInputData(hexGrid);
  hie->SetMaximumDepth(1);
  hie->SetLabelArrayName("Global Vertex IDs");
  hie->SetTextProperty(textProp);

  vtkNew<vtkLabelPlacementMapper> labelMapper;
  labelMapper->SetInputConnection(hie->GetOutputPort());

  vtkNew<vtkActor2D> labelActor;
  labelActor->SetMapper(labelMapper);

  //================================================================
  // Rendering
  //================================================================

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputData(hexGrid);

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->EdgeVisibilityOn();
  actor->GetProperty()->SetColor(0, 0.5, 1);

  vtkNew<vtkAxesActor> axes;
  auto l = dx * (nx + 1);
  axes->SetTotalLength(l, l, l);
  int s = 5;
  axes->GetXAxisCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontSize(s);
  axes->GetYAxisCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontSize(s);
  axes->GetZAxisCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontSize(s);

  vtkNew<vtkRenderer> ren;
  ren->AddActor(actor);
  ren->AddActor(axes);
  ren->AddActor(labelActor);
  ren->SetBackground(0, 0, 0); // beige

  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer(ren);
  renWin->SetSize(1200, 800);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);

  renWin->Render();
  iren->Initialize();
  iren->Start();
}
