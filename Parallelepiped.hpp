#ifndef GRAPHICS_COURSEWORK_PARALLELEPIPED_HPP
#define GRAPHICS_COURSEWORK_PARALLELEPIPED_HPP


#include <array>
#include <vector>
#include <QVector3D>

class Parallelepiped {
public:
    Parallelepiped(std::array<QVector3D, 8> points, QVector3D cam, QVector3D light);

    //return faces (lists of indexes)
    std::vector<int *> get_visible_faces();

    // return list of bool is_shadowed of visible faces true if shadowed, false if not
    std::vector<bool> get_shadowed_faces(const std::vector<int *> &vis_faces);

    bool is_shadowed(int *face);

    std::array<QPointF, 8> get_points();

    std::array<QVector3D, 8> get_3D_points();

    void move(float dx, float dy, float dz);

    void rotateX(double angle);

    void rotateY(double angle);

    void rotateZ(double angle);

    void resize(float delta);

    void change_projection();

    QVector3D *operator[](int i);

protected:

    void update_center();

    bool is_visible(int *face);

    void project();

    void project_flat();

    void project_volume();

private:
    bool is_flat = true;
    std::array<QVector3D, 8> points3D;
    std::array<QPointF, 8> points2D;
    QVector3D center;
    QVector3D camera;
    QVector3D light_point;
    float projection_angle = 30 * std::numbers::pi / 180;

    // array of arrays of indexes of points
    int faces[6][4] = {{0, 1, 2, 3},
                       {0, 1, 5, 4},
                       {0, 3, 7, 4},
                       {1, 2, 6, 5},
                       {2, 6, 7, 3},
                       {4, 5, 6, 7}};

};


#endif //GRAPHICS_COURSEWORK_PARALLELEPIPED_HPP
