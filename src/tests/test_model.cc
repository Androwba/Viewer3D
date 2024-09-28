#include "test_core.h"

TEST(ModelTest, WrongExtension) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  std::filesystem::path currentPath(__FILE__);
  std::filesystem::path wrongExtensionPath =
      currentPath.parent_path() / "Makefile";
  model->ParseObj(wrongExtensionPath.generic_string());

  int expected_vertex_coord_size = 0;
  int expected_vertex_index_size = 0;
  EXPECT_EQ(model->GetError(), s21::Error::kFileWrongExtension);
  EXPECT_EQ(model->GetVertexCoord()->size(), expected_vertex_coord_size);
  EXPECT_EQ(model->GetVertexIndex()->size(), expected_vertex_index_size);
}

TEST(ModelTest, CorrectFile) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  std::filesystem::path currentPath(__FILE__);
  std::filesystem::path modelsPath =
      currentPath.parent_path().parent_path() / "models" / "school21.obj";
  model->ParseObj(modelsPath.string());

  EXPECT_EQ(model->GetError(), s21::Error::kOk);

  int expected_vertex_coord_size = 4470;
  int expected_vertex_index_size = 17616;

  EXPECT_EQ(model->GetVertexCoord()->size(), expected_vertex_coord_size);
  EXPECT_EQ(model->GetVertexIndex()->size(), expected_vertex_index_size);
}

TEST(ModelTest, FailedToOpen) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  model->ParseObj("notexist.obj");

  EXPECT_EQ(model->GetError(), s21::Error::kFailedToOpen);
  EXPECT_TRUE(model->GetVertexCoord()->empty());
  EXPECT_TRUE(model->GetVertexIndex()->empty());
}

TEST(ModelTest, IncorrectFile) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  std::filesystem::path currentPath(__FILE__);
  std::filesystem::path modelsPath =
      currentPath.parent_path().parent_path() / "models" / "broken_model.obj";
  model->ParseObj(modelsPath.string());

  EXPECT_EQ(model->GetError(), s21::Error::kIncorrectData);
}

TEST(ModelTest, Move) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  std::filesystem::path currentPath(__FILE__);
  std::filesystem::path modelsPath =
      currentPath.parent_path().parent_path() / "models" / "school21.obj";
  model->ParseObj(modelsPath.generic_string());

  EXPECT_EQ(model->GetError(), s21::Error::kOk);

  auto vertex = model->GetVertexCoord();
  std::vector<double> vertex_copy(*vertex);
  auto axis_x = s21::Axis::kX, axis_y = s21::Axis::kY, axis_z = s21::Axis::kZ;
  double value = 3;
  auto a = s21::Action::move;
  model->Transform(a, value, axis_x);
  model->Transform(a, value, axis_y);
  model->Transform(a, value, axis_z);

  for (std::size_t i = 0; i < vertex_copy.size(); i++) {
    EXPECT_DOUBLE_EQ((*vertex)[i], vertex_copy[i] + 3);
  }
}

TEST(ModelTest, Rotate) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  std::filesystem::path currentPath(__FILE__);
  std::filesystem::path modelsPath =
      currentPath.parent_path().parent_path() / "models" / "school21.obj";
  model->ParseObj(modelsPath.generic_string());

  auto axis_x = s21::Axis::kX, axis_y = s21::Axis::kY, axis_z = s21::Axis::kZ;
  auto r = s21::Action::rotate;
  double value = 5;
  auto vertex = model->GetVertexCoord();
  std::vector<double> vertex_copy(*vertex);

  model->Transform(r, value, axis_x);
  model->Transform(r, value, axis_y);
  model->Transform(r, value, axis_z);

  for (std::size_t i = 0; i < vertex_copy.size(); i++) {
    if (vertex_copy[i] != 0) {
      EXPECT_FALSE((*vertex)[i] == vertex_copy[i]);
    }
  }
}

TEST(ModelTest, Scale) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  std::filesystem::path currentPath(__FILE__);
  std::filesystem::path modelsPath =
      currentPath.parent_path().parent_path() / "models" / "school21.obj";
  model->ParseObj(modelsPath.generic_string());

  auto vertex = model->GetVertexCoord();
  std::vector<double> vertex_copy(*vertex);
  auto axis_x = s21::Axis::kX;
  auto scale = s21::Action::scale;
  double value = 5;
  model->Transform(scale, value, axis_x);

  for (std::size_t i = 0; i < vertex_copy.size(); i++) {
    EXPECT_DOUBLE_EQ((*vertex)[i], vertex_copy[i] * 5);
  }
}

TEST(ModelTest, HandleInvalidIndex) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  std::ofstream tempObjFile("invalid_index.obj");
  tempObjFile << "v 1.0 2.0 3.0\n";
  tempObjFile << "f 1 2 3 -1\n";
  tempObjFile.close();
  model->ParseObj("invalid_index.obj");

  EXPECT_EQ(model->GetError(), s21::Error::kOk);
  std::remove("invalid_index.obj");
}
