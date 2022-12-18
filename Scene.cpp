#include "Scene.hpp"

Scene::Scene(QGraphicsScene *sc) : scene(sc) {
    camera = QVector3D(0, 0, 1000);
    light_point = QVector3D(0, -500, 0);

    parallelepiped = new Parallelepiped({QVector3D(30, 30, 100), {30, 30, 30}, {70, 30, 30},
                                         {70, 30, 100}, {30, 60, 100}, {30, 60, 30},
                                         {70, 60, 30}, {70, 60, 100}}, camera, light_point);
    triangular_pyramid = new TriangularPyramid({QVector3D(90, 40, 75), {80, 60, 100}, {90, 60, 55},
                                                {100, 60, 100}}, camera, light_point);

    shadows = new Shadows(parallelepiped->get_3D_points(), triangular_pyramid->get_3D_points());

    update();
}

void Scene::change_projection() {
    parallelepiped->change_projection();
    triangular_pyramid->change_projection();
    shadows->change_projection();
    update();
}

void Scene::move(Figure f, float dx, float dy, float dz) {
    if (f == pyramid) {
        triangular_pyramid->move(dx, dy, dz);
    } else if (f == paral) {
        parallelepiped->move(dx, dy, dz);
    }
    update();
}

void Scene::update() {
    for (auto & poly : polygons){
        scene->removeItem(poly);
        delete poly;
    }
//    for(auto& item: scene->items()){
//        scene->removeItem(item);
//        delete item;
//    }

    polygons.clear();

    shadows->update(parallelepiped->get_3D_points(), triangular_pyramid->get_3D_points());
    draw_shadows();

    visible_parallelepiped = parallelepiped->get_visible_faces();

    visible_pyramid = triangular_pyramid->get_visible_faces();

    sort_by_z(visible_parallelepiped, visible_pyramid);

    parallelepiped_points2D = parallelepiped->get_points();
    pyramid_points2D = triangular_pyramid->get_points();

    draw();

}

void Scene::rotateX(Figure f, double angle) {
    if (f == pyramid) {
        triangular_pyramid->rotateX(angle);
    } else if (f == paral) {
        parallelepiped->rotateX(angle);
    }
    update();
}

void Scene::rotateY(Figure f, double angle) {
    if (f == pyramid) {
        triangular_pyramid->rotateY(angle);
    } else if (f == paral) {
        parallelepiped->rotateY(angle);
    }
    update();
}

void Scene::rotateZ(Figure f, double angle) {
    if (f == pyramid) {
        triangular_pyramid->rotateZ(angle);
    } else if (f == paral) {
        parallelepiped->rotateZ(angle);
    }
    update();
}

void Scene::sort_by_z(std::vector<int *> p_sides, std::vector<int *> t_sides) {
    bubble_sort_by_z(p_sides, paral);
    bubble_sort_by_z(t_sides, pyramid);
    int i_p = 0, i_t = 0;
    while (i_p < p_sides.size() || i_t < t_sides.size()) {
        if (i_t == t_sides.size()) {
            sorted_visible.push_back(p_sides[i_p]);
            sorted_types.push_back(paral);
            sorted_shadowed.push_back(parallelepiped->is_shadowed(p_sides[i_p]));
            i_p++;
            continue;
        }
        if (i_p == p_sides.size()) {
            sorted_visible.push_back(t_sides[i_t]);
            sorted_types.push_back(pyramid);
            sorted_shadowed.push_back(triangular_pyramid->is_shadowed(t_sides[i_t]));
            i_t++;
            continue;
        }
        if (max_z(p_sides[i_p], paral) < max_z(t_sides[i_t], pyramid)) {
            sorted_visible.push_back(p_sides[i_p]);
            sorted_types.push_back(paral);
            sorted_shadowed.push_back(parallelepiped->is_shadowed(p_sides[i_p]));
            i_p++;
        } else {
            sorted_visible.push_back(t_sides[i_t]);
            sorted_types.push_back(pyramid);
            sorted_shadowed.push_back(triangular_pyramid->is_shadowed(t_sides[i_t]));
            i_t++;
        }

    }
}

void Scene::bubble_sort_by_z(std::vector<int *> &sides, Figure f) {
    for (auto i = 0; i < sides.size(); i++) {
        bool sorted = true;
        for (auto j = 0; j < sides.size() - (i + 1); j++) {
            if (max_z(sides[j], f) >= max_z(sides[j + 1], f)) {
                sorted = false;
                std::swap(sides[j], sides[j + 1]);
            }
        }
        if(sorted) break;
    }
}

float Scene::max_z(int *side, Figure f) {
    int max;
    float max_z;
    if (f == pyramid) {
        max = 3;
        max_z = (*triangular_pyramid)[side[0]]->z();
    } else {
        max = 4;
        max_z = (*parallelepiped)[side[0]]->z();
    }

    for (auto i = 0; i < max; i++) {
        if (f == pyramid && max_z < (*triangular_pyramid)[side[i]]->z()) {
            max_z = (*triangular_pyramid)[side[i]]->z();
        } else if (f == paral && max_z < (*parallelepiped)[side[i]]->z()) {
            max_z = (*parallelepiped)[side[i]]->z();
        }
    }

    return max_z;
}

void Scene::resize(Figure f, float delta) {
    if (f == pyramid) {
        triangular_pyramid->resize(delta);
    } else if (f == paral) {
        parallelepiped->resize(delta);
    }
    update();
}

void Scene::draw() {

    for (auto i = 0; i < sorted_visible.size(); i++) {
        if (sorted_types[i] == paral) {
            draw_parallelepiped_side(sorted_visible[i], sorted_shadowed[i]);
        } else {
            draw_pyramid_side(sorted_visible[i], sorted_shadowed[i]);
        }
    }
}

void Scene::draw_pyramid_side(int *side, bool shadowed) {
    QBrush brush(pyramid_color);
    if(shadowed){
        brush.setColor(Qt::black);
    }
    QPen pen(Qt::black);
    QList<QPointF> temp;
    for (auto i = 0; i < 3; i++) {
        temp.push_back(pyramid_points2D[side[i]]);
    }
    polygons.push_back(scene->addPolygon(temp, pen, brush));

}

void Scene::draw_parallelepiped_side(int *side, bool shadowed) {
    QBrush brush(paral_color);
    if(shadowed){
        brush.setColor(Qt::black);
    }
    QPen pen(Qt::black);
    QList<QPointF> temp = {parallelepiped_points2D[side[0]], parallelepiped_points2D[side[1]],
                           parallelepiped_points2D[side[2]]};
    polygons.push_back(scene->addPolygon(temp, pen, brush));
    temp = {parallelepiped_points2D[side[0]], parallelepiped_points2D[side[2]],
            parallelepiped_points2D[side[3]]};
    polygons.push_back(scene->addPolygon(temp, pen, brush));
}

void Scene::draw_shadows() {
    for(auto &item: shadowed_poly){
        scene->removeItem(item);
        delete item;
    }
    shadowed_poly.clear();
    QBrush brush(Qt::gray);
    QPen pen(Qt::gray);
    auto shadow1 = shadows->get_shadow1();
    auto shadow2 = shadows->get_shadow2();

    for(auto i = 1; i < shadow1.size() - 1; i++){
        for(auto j = i+1; j < shadow1.size(); j++){
            shadowed_poly.push_back(scene->addPolygon({shadow1[0], shadow1[i], shadow1[j]}, pen, brush));
        }
    }

    for(auto i = 1; i < shadow2.size() - 1; i++){
        for(auto j = i+1; j < shadow2.size(); j++){
            shadowed_poly.push_back(scene->addPolygon({shadow2[0], shadow2[i], shadow2[j]}, pen, brush));
        }
    }
}

