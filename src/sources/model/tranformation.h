#ifndef CPP4_3DVIEWER_V2_0_MODEL_TRANSFORMATION_H_
#define CPP4_3DVIEWER_V2_0_MODEL_TRANSFORMATION_H_

#include <cmath>
#include <vector>

namespace s21 {

enum class Axis {
  kX,
  kY,
  kZ,
};

enum class Action {
  move,
  rotate,
  scale,
};

class ITransform {
 public:
  virtual void Transform(std::vector<double>&, double&, Axis&) = 0;
};

class Move : public ITransform {
 public:
  void Transform(std::vector<double>& vertex_coords, double& step,
                 Axis& axis) override;
};

class Rotate : public ITransform {
 public:
  void Transform(std::vector<double>& vertex_coords, double& angle,
                 Axis& axis) override;
};

class Scale : public ITransform {
 public:
  void Transform(std::vector<double>& vertex_coords, double& scale,
                 Axis&) override;
};

class Transformer {
 public:
  Transformer* Action(enum Action action);
  void Transform(std::vector<double>& vertex_coords, double& value, Axis& axis);

 private:
  Move move_;
  Rotate rotate_;
  Scale scale_;
  enum Action action_;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_MODEL_TRANSFORMATION_H_
