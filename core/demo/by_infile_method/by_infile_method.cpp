#include "tetgen.h" // Defined tetgenio, tetrahedralize().
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkPoints.h"
#include "vtkPolyVertex.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkSmartPointer.h"
#include "vtkCellArray.h"


#include "../../strutil/include/strutil.h"
#include "../../cellPicker/include/cellPicker.h"

int main(int argc, char *argv[])
{
  tetgenio in, out;

  in.firstnumber = 1;
  in.load_stl("../data/Implant.stl");
  
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();


  tetrahedralize("apq1.1", &in, &out);
  int *mtetrahedron = out.tetrahedronlist;
  double *pointslist = out.pointlist;

  double arr[3];
  for (int i = 0; i < out.numberofpoints; ++i) {
    int idx = i * 3;
    arr[0] = pointslist[idx];
    arr[1] = pointslist[idx + 1];
    arr[2] = pointslist[idx + 2];
    points->InsertPoint(i + 1, arr);
  }

  vtkIdType ids[4];
  // set points
  vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid = 
    vtkSmartPointer<vtkUnstructuredGrid>::New();
  unstructuredGrid->SetPoints(points);
  for (int i = 0; i < out.numberoftetrahedra; ++i) {
    int idx = i * 4; 
    ids[0] = mtetrahedron[idx];
    ids[1] = mtetrahedron[idx + 1];
    ids[2] = mtetrahedron[idx + 2];
    ids[3] = mtetrahedron[idx + 3];
    unstructuredGrid->InsertNextCell(VTK_TETRA, 4, ids); 
  }

  vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputData(unstructuredGrid);

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(0.194,0.562, 0.75);

  vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
  ren->AddActor(actor);
  ren->ResetCamera();
  ren->SetBackground(1, 1, 1);

  vtkSmartPointer<vtkRenderWindow> win = vtkSmartPointer<vtkRenderWindow>::New();
  win->AddRenderer(ren);
  win->SetSize(500, 300);

  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(win);

  MouseInteractorStyle *style = new MouseInteractorStyle();
  style->SetDefaultRenderer(ren);

  style->Data = unstructuredGrid;
  iren->SetInteractorStyle(style);

  iren->Start();
  return 0;
}