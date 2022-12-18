#ifndef GRAPHICS_COURSEWORK_SHADOWS_HPP
#define GRAPHICS_COURSEWORK_SHADOWS_HPP

#include <QVector3D>

class Shadows {
public:
    Shadows(std::array<QVector3D, 8> fig1, std::array<QVector3D, 4> fig2);

    void update(std::array<QVector3D, 8> fig1, std::array<QVector3D, 4> fig2);

    std::vector<QPointF> get_shadow1();

    std::vector<QPointF> get_shadow2();

    void change_projection();

private:
    void project();

    void project_flat();

    void project_volume();

    bool is_flat = true;

    std::vector<QVector3D> shadows_figure1;
    std::vector<QPointF> shadow1;

    std::vector<QVector3D> shadows_figure2;
    std::vector<QPointF> shadow2;

    float projection_angle = 30 * std::numbers::pi / 180;

    float y_floor = 300;
};


#endif //GRAPHICS_COURSEWORK_SHADOWS_HPP
