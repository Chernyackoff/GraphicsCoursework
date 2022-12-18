#include "Parallelepiped.hpp"

#include <cmath>


Parallelepiped::Parallelepiped(std::array<QVector3D, 8> points, QVector3D cam, QVector3D light) : camera(cam),
                                                                                                  light_point(light) {
    points3D = points;
    update_center();
}

void Parallelepiped::update_center() {
    float sum_x = 0, sum_y = 0, sum_z = 0;
    for (auto &point: points3D) {
        sum_x += point.x();
        sum_y += point.y();
        sum_z += point.z();
    }
    center.setX(sum_x / 8);
    center.setY(sum_y / 8);
    center.setZ(sum_z / 8);
}

void Parallelepiped::change_projection() {
    is_flat = !is_flat;
}

bool Parallelepiped::is_visible(int *face) {
    float vx1, vy1, vz1;
    float vx2, vy2, vz2;
    vx1 = points3D[face[0]].x() - points3D[face[1]].x();
    vy1 = points3D[face[0]].y() - points3D[face[1]].y();
    vz1 = points3D[face[0]].z() - points3D[face[1]].z();

    vx2 = points3D[face[1]].x() - points3D[face[2]].x();
    vy2 = points3D[face[1]].y() - points3D[face[2]].y();
    vz2 = points3D[face[1]].z() - points3D[face[2]].z();

    float a, b, c, d;
    a = vy1 * vz2 - vy2 * vz1;
    b = vz1 * vx2 - vz2 * vx1;
    c = vx1 * vy2 - vx2 * vy1;
    d = -(a * points3D[face[1]].x() + b * points3D[face[1]].y() + c * points3D[face[1]].z());
    float sign = ((a * center.x() + b * center.y() + c * center.z() + d) < 0) ? 1 : -1;
    a *= sign;
    b *= sign;
    c *= sign;
    d *= sign;

    if ((a * camera.x() + b * camera.y() + c * camera.z() + d) < 0) return false;
    return true;
}

bool Parallelepiped::is_shadowed(int *face) {
    float vx1, vy1, vz1;
    float vx2, vy2, vz2;
    vx1 = points3D[face[0]].x() - points3D[face[1]].x();
    vy1 = points3D[face[0]].y() - points3D[face[1]].y();
    vz1 = points3D[face[0]].z() - points3D[face[1]].z();

    vx2 = points3D[face[1]].x() - points3D[face[2]].x();
    vy2 = points3D[face[1]].y() - points3D[face[2]].y();
    vz2 = points3D[face[1]].z() - points3D[face[2]].z();

    float a, b, c, d;
    a = vy1 * vz2 - vy2 * vz1;
    b = vz1 * vx2 - vz2 * vx1;
    c = vx1 * vy2 - vx2 * vy1;
    d = -(a * points3D[face[1]].x() + b * points3D[face[1]].y() + c * points3D[face[1]].z());
    float sign = ((a * center.x() + b * center.y() + c * center.z() + d) < 0) ? 1 : -1;
    a *= sign;
    b *= sign;
    c *= sign;
    d *= sign;

    if ((a * light_point.x() + b * light_point.y() + c * light_point.z() + d) < 0) return true;
    return false;
}

std::vector<int *> Parallelepiped::get_visible_faces() {
    std::vector<int*> result;
    for (auto& face : faces){
        if (is_visible(face)){
            result.push_back(face);
        }
    }
    return result;
}

std::vector<bool> Parallelepiped::get_shadowed_faces(const std::vector<int *>& vis_faces) {
    std::vector<bool> result;
    for (auto & face : vis_faces){
        result.push_back(is_shadowed(face));
    }
    return result;
}

std::array<QPointF, 8> Parallelepiped::get_points() {
    project();
    return points2D;
}

void Parallelepiped::project() {
    is_flat ? project_flat() : project_volume();
}

void Parallelepiped::project_flat() {
    for(auto i = 0; i < 8; i++){
        points2D[i] = points3D[i].toPointF();
    }
}

void Parallelepiped::project_volume() {
    for (int i = 0; i < 8; i++) {
        double xp, yp;
        xp = points3D[i].x() + points3D[i].z() * std::cos(projection_angle); // Jean Cavalier projection
        yp = points3D[i].y() + points3D[i].z() * std::sin(projection_angle);
        points2D[i].setX(xp);
        points2D[i].setY(yp);
    }
}

void Parallelepiped::move(float dx, float dy, float dz) {
    for(auto& point : points3D){
        point.setX(point.x() + dx);
        point.setY(point.y() + dy);
        point.setZ(point.z() + dz);
    }
    update_center();
}

void Parallelepiped::rotateX(double angle) {
    for (auto &point: points3D) {
        auto dz = point.z() - center.z(), dy = point.y() - center.y();
        double sz, sy;
        sy = dy * cos(angle) - dz * sin(angle);
        sz = dy * sin(angle) + dz * cos(angle);
        point.setZ(sz + center.z());
        point.setY(sy + center.y());
    }
}

void Parallelepiped::rotateY(double angle) {
    for (auto &point: points3D) {
        auto dz = point.z() - center.z(), dx = point.x() - center.x();
        double sx, sz;
        sx = dx * cos(angle) + dz * sin(angle);
        sz = -dx * sin(angle) + dz * cos(angle);
        point.setZ(sz + center.z());
        point.setX(sx + center.x());
    }
}

void Parallelepiped::rotateZ(double angle) {
    for (auto &point: points3D) {
        auto dx = point.x() - center.x(), dy = point.y() - center.y();
        double sx, sy;
        sx = dx * cos(angle) - dy * sin(angle);
        sy = dx * sin(angle) + dy * cos(angle);
        point.setX(sx + center.x());
        point.setY(sy + center.y());
    }
}

void Parallelepiped::resize(float delta) {
    for (auto &point: points3D) {
        auto dx = point.x() - center.x(), dy = point.y() - center.y(), dz = point.z() - center.z();
        double sx, sy, sz;
        sx = dx * delta;
        sy = dy * delta;
        sz = dz * delta;
        point.setX(sx + center.x());
        point.setY(sy + center.y());
        point.setZ(sz + center.z());
    }
}

QVector3D *Parallelepiped::operator[](int i) {
    return &points3D[i];
}

std::array<QVector3D, 8> Parallelepiped::get_3D_points() {
    return points3D;
}
