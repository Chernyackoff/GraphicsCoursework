#include "Shadows.hpp"

void Shadows::update(std::array<QVector3D, 8> fig1, std::array<QVector3D, 4> fig2) {
    shadows_figure1.clear();
    shadows_figure2.clear();
    for(auto &point: fig1){
        shadows_figure1.emplace_back(point.x(), y_floor, point.z());
    }
    for(auto &point: fig2){
        shadows_figure2.emplace_back(point.x(), y_floor, point.z());
    }
}

void Shadows::change_projection() {
    is_flat = !is_flat;
}

void Shadows::project() {
    is_flat ? project_flat() : project_volume();

}

void Shadows::project_flat() {
    shadow1.clear();
    for(auto &point : shadows_figure1){
        shadow1.push_back(point.toPointF());
    }

    shadow2.clear();
    for(auto &point : shadows_figure2){
        shadow2.push_back(point.toPointF());
    }
}

void Shadows::project_volume() {
    shadow1.clear();
    shadow2.clear();
    for (auto& point : shadows_figure1) {
        double xp, yp;
        xp = point.x() + point.z() * std::cos(projection_angle); // Jean Cavalier projection
        yp = y_floor + point.z() * std::sin(projection_angle);
        shadow1.emplace_back(xp, yp);
    }
    for (auto& point : shadows_figure2) {
        double xp, yp;
        xp = point.x() + point.z() * std::cos(projection_angle); // Jean Cavalier projection
        yp = y_floor + point.z() * std::sin(projection_angle);
        shadow2.emplace_back(xp, yp);
    }

}

std::vector<QPointF> Shadows::get_shadow1() {
    project();
    return shadow1;
}

std::vector<QPointF> Shadows::get_shadow2() {
    project();
    return shadow2;
}

Shadows::Shadows(std::array<QVector3D, 8> fig1, std::array<QVector3D, 4> fig2) {
    update(fig1, fig2);
}
