#ifndef GRAPHICS_COURSEWORK_SCENE_HPP
#define GRAPHICS_COURSEWORK_SCENE_HPP

#include <QGraphicsScene>
#include "Parallelepiped.hpp"
#include "TriangularPyramid.hpp"
#include "Shadows.hpp"

enum Figure {
    paral,
    pyramid
};

class Scene {
public:
    Scene(QGraphicsScene * sc);

    void change_projection();

    void move(Figure f, float dx, float dy, float dz);

    void rotateX(Figure f, double angle);

    void rotateY(Figure f, double angle);

    void rotateZ(Figure f, double angle);

    void resize(Figure f, float delta);

protected:
    void update();

    void sort_by_z(std::vector<int *> p_sides, std::vector<int *> t_sides);

    void bubble_sort_by_z(std::vector<int *> &sides, Figure f);

    float max_z(int *side, Figure f);

    void draw();

    void draw_pyramid_side(int* side, bool shadowed);

    void draw_parallelepiped_side(int* side, bool shadowed);

    void draw_shadows();

private:
    QGraphicsScene *scene;
    QVector3D camera;
    QVector3D light_point;

    Parallelepiped* parallelepiped;
    std::vector<int *> visible_parallelepiped;
    std::vector<bool> shadowed_parallelepiped;
    std::array<QPointF, 8> parallelepiped_points2D;

    TriangularPyramid* triangular_pyramid;
    std::vector<int *> visible_pyramid;
    std::vector<bool> shadowed_pyramid;
    std::array<QPointF, 4> pyramid_points2D;

    Shadows *shadows;

    std::vector<int *> sorted_visible;
    std::vector<Figure> sorted_types;
    std::vector<bool> sorted_shadowed;

    QColor pyramid_color = Qt::green;
    QColor paral_color = Qt::red;

    std::vector<QGraphicsPolygonItem *> polygons;
    std::vector<QGraphicsPolygonItem *> shadowed_poly;
};


#endif //GRAPHICS_COURSEWORK_SCENE_HPP
