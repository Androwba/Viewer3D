#include <QApplication>

#include "controller/controller.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::View view;
  view.setWindowTitle("3D_Viewer_v2.0");
  s21::Controller controller(&view);

  QObject::connect(&controller, &s21::Controller::SolutionReady, &view,
                   &s21::View::HandleSolution_);
  QObject::connect(&controller, &s21::Controller::ErrorHasOccured, &view,
                   &s21::View::HandleError_);

  view.show();
  return QApplication::exec();
}
