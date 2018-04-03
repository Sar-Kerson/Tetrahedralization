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
#include "vtkLine.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"

#include "../strutil/include/strutil.h"
#include "../cellPicker/include/cellPicker.h"

using namespace std;

int main(int argc, char const *argv[])
{
	/* code */
	FILE *fp = NULL;
	if ((fp=fopen("../data/nodes.xyz", "r")) == NULL) {
		cout << "error!\n";
		return 0;
	}
	double arr[3];
	int id;
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		
	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();

	while(!feof(fp)) {
		int ret = fscanf(fp, "%d %lf %lf %lf", &id, &arr[0], &arr[1], &arr[2]);
		if (ret != 4) break;
		points->InsertPoint(id, arr);
	}
	fclose(fp);
	if ((fp=fopen("../data/topo", "r")) == NULL) {
		cout << "error!\n";
		return 0;
	}
	// int count = 0;
	vtkIdType ids[4];
	// set points
	vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid = 
		vtkSmartPointer<vtkUnstructuredGrid>::New();
	unstructuredGrid->SetPoints(points);
	while(!feof(fp)) {
		int ret = fscanf(fp, "%lld %lld %lld %lld", &ids[0], &ids[1], &ids[2], &ids[3]);
		if(ret != 4) break;
		unstructuredGrid->InsertNextCell(VTK_TETRA, 4, ids);
	}

	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();

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

	// Set the custom stype to use for interaction.
	MouseInteractorStyle *style = new MouseInteractorStyle();
	style->SetDefaultRenderer(ren);

	style->Data = unstructuredGrid;
	iren->SetInteractorStyle(style);

	iren->Start();

	cout << "nums:" << points->GetNumberOfPoints() << "\n";
	// cout << "count:" << count << "\n";
	return 0;
}