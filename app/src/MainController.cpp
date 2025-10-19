//
// Created by matfrg on 10/18/25.
//

#include "MainController.hpp"

#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

namespace app {
void MainController::initialize() { engine::graphics::OpenGL::enable_depth_testing(); }

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) { return false; }
    return true;
}

void MainController::draw_garage() {

    //model

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model *garage = resources->model("garage");

    //shader


    engine::resources::Shader *shader = resources->shader("garage");
    shader->use();

    shader->set_mat4("projection", graphics->projection_matrix());

    // Uzimanje view matrice iz kamere i postavljanje u shader
    glm::mat4 view = graphics->camera()->view_matrix();
    shader->set_mat4("view", view);

    // Izvlačenje pozicije kamere iz inverse view matrice i slanje u shader
    glm::vec3 camPos = glm::vec3(glm::inverse(view)[3]);
    shader->set_vec3("viewPos", camPos);

    // Postavljanje pozicije i boje svetla u shader
    shader->set_vec3("lightPos", glm::vec3(2.0f, 4.0f, 2.0f));
    shader->set_vec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));// bela svetlost


    // Kreiranje model matrice: pozicioniranje i skaliranje modela
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
    model = glm::scale(model, glm::vec3(0.02f));

    shader->set_mat4("model", model);


    garage->draw(shader);

}

void MainController::begin_draw() { engine::graphics::OpenGL::clear_buffers(); }

void MainController::draw() {
    //clear buffers

    draw_garage();

    //swap buffer


}

void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}
}

// app