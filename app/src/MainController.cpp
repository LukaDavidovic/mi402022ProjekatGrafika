
#include <MainController.hpp>

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

    // svetlo1
    shader->set_vec3("lightPos1", glm::vec3(-2.0f, 4.0f, -5.0f));
    shader->set_vec3("lightColor1", glm::vec3(1.0f, 1.0f, 1.0f));// belo


    // Kreiranje model matrice: pozicioniranje i skaliranje modela
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
    model = glm::scale(model, glm::vec3(0.02f));

    shader->set_mat4("model", model);

    garage->draw(shader);

}

void MainController::update_camera() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    float dt = platform->dt();

    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt); }
    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt); }
    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt); }
    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt); }
    if (platform->key(engine::platform::KeyId::KEY_X).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::DOWN, dt); }
    if (platform->key(engine::platform::KeyId::KEY_SPACE).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::UP, dt); }
    if (platform->key(engine::platform::KeyId::KEY_L).is_down()) { camera->rotate_camera(engine::graphics::Camera::Movement::LEFT * 4, 0); }   //POMERANJE UDESNO
    if (platform->key(engine::platform::KeyId::KEY_K).is_down()) { camera->rotate_camera(-(engine::graphics::Camera::Movement::LEFT * 4), 0); }//POMERANJE ULEVO
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

void MainController::update() { update_camera(); }
}

// app