#include "FldData.h"
#include "vtkCamera.h"
#include "vtkClipDataSet.h"
#include "vtkDataSetMapper.h"
#include "vtkLODActor.h"
#include "vtkPNGWriter.h"
#include "vtkPlane.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkWindowToImageFilter.h"

#include <chrono>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <field_file>" << std::endl;
    return 1;
  }
  std::string filename{argv[1]};

  fld::FldData<float, int> data(filename);

  auto gridStart = std::chrono::steady_clock::now();
  // auto hexGrid = data.GetHexGrid();
  auto hexGrid = data.GetLagrangeHexGrid();
  auto gridEnd = std::chrono::steady_clock::now();

  vtkNew<vtkPlane> clipPlane;
  clipPlane->SetOrigin(hexGrid->GetCenter());
  clipPlane->SetNormal(1.0, 0.0, 0.0);

  vtkNew<vtkClipDataSet> clipper;
  clipper->SetClipFunction(clipPlane);
  clipper->SetInputData(hexGrid);
  clipper->SetValue(0.0);
  clipper->GenerateClippedOutputOn();
  clipper->Update();

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputData(clipper->GetOutput());
  mapper->SetScalarRange(hexGrid->GetScalarRange());

  vtkNew<vtkLODActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->EdgeVisibilityOn();
  actor->GetProperty()->SetLineWidth(0.25);
  actor->GetProperty()->SetAmbient(50);

  vtkNew<vtkRenderer> ren;
  ren->AddActor(actor);
  ren->SetBackground(245, 245, 220); // beige

  // Move camera
  // vtkNew<vtkCamera> camera;
  // camera->SetPosition(-5, 0, 20);
  // camera->SetFocalPoint(5, 0, 0);
  // camera->Zoom(4);
  // ren->SetActiveCamera(camera);

  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer(ren);
  renWin->SetSize(1600, 800);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);

  auto renderStart = std::chrono::steady_clock::now();
  renWin->Render();
  auto renderEnd = std::chrono::steady_clock::now();

  std::chrono::duration<double> gridTime = gridEnd - gridStart;
  std::chrono::duration<double> renderTime = renderEnd - renderStart;
  std::cout << "Grid setup time: " << gridTime.count() << " s" << std::endl;
  std::cout << "Rendering time: " << renderTime.count() << " s" << std::endl;

  // Write to .png
  // vtkNew<vtkPNGWriter> writer;
  // vtkNew<vtkWindowToImageFilter> winToImg;
  // winToImg->SetInput(renWin);
  // winToImg->SetScale(1);
  // winToImg->SetInputBufferTypeToRGB();
  // winToImg->ReadFrontBufferOff();
  // winToImg->Update();
  // writer->SetFileName("rod_short.png");
  // writer->SetInputConnection(winToImg->GetOutputPort());
  // writer->Write();

  iren->Initialize();
  iren->Start();
}