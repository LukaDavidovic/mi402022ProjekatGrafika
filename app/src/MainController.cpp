

#include "../../engine/libs/glad/include/glad/glad.h"

#include <spdlog/spdlog.h>
#include <MainController.hpp>

#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool g_action_triggered = false;
bool g_red_car_removed = false;
bool g_blue_car_removed = false;
float g_light_intensity = 3.0f;

std::chrono::steady_clock::time_point g_action_time;

namespace app {

void MainController::initialize() { engine::graphics::OpenGL::enable_depth_testing(); }

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) { return false; }
    return true;
}

void MainController::draw_garage() {

    //model

    update_events();

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model *garage = resources->model("garage");

    engine::resources::Model *mustang = resources->model("mustang");

    //shader

    engine::resources::Shader *shader = resources->shader("garage");

    engine::resources::Texture *albedo = resources->texture("GARAGE_FLOOR_albedo");
    engine::resources::Texture *ao = resources->texture("GARAGE_FLOOR_ao");
    engine::resources::Texture *bump = resources->texture("GARAGE_FLOOR_bump");
    engine::resources::Texture *metalness = resources->texture("GARAGE_FLOOR_metalness");
    engine::resources::Texture *normal = resources->texture("GARAGE_FLOOR_normal");
    engine::resources::Texture *roughness = resources->texture("GARAGE_FLOOR_roughness");

    shader->use();

    if (albedo) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedo->id());
        shader->set_int("albedoMap", 0);
    }
    if (ao) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ao->id());
        shader->set_int("aoMap", 1);
    }
    if (bump) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, bump->id());
        shader->set_int("bumpMap", 2);
    }
    if (metalness) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, metalness->id());
        shader->set_int("metalnessMap", 3);
    }
    if (normal) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, normal->id());
        shader->set_int("normalMap", 4);
    }
    if (roughness) {
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, roughness->id());
        shader->set_int("roughnessMap", 5);
    }

    shader->set_mat4("projection", graphics->projection_matrix());
    glm::mat4 view = graphics->camera()->view_matrix();
    shader->set_mat4("view", view);

    glm::vec3 camPos = glm::vec3(glm::inverse(view)[3]);
    shader->set_vec3("viewPos", camPos);

    shader->set_vec3("lightPos1", glm::vec3(1.5f, 3.0f, -6.0f));
    shader->set_vec3("lightColor1", glm::vec3(1.0f, 1.0f, 1.0f));// belo

    shader->set_vec3("lightDir1", glm::vec3(2.0f, 4.0f, -1.0f));
    shader->set_vec3("lightColor2", glm::vec3(1.0f, 1.0f, 1.0f));

    float constantAttenuation = 1.0f;
    float linearAttenuation = 0.09f;
    float quadraticAttenuation = 0.032f;

    shader->set_float("constantAttenuation", constantAttenuation);
    shader->set_float("linearAttenuation", linearAttenuation);
    shader->set_float("quadraticAttenuation", quadraticAttenuation);
    shader->set_float("lightIntensity", g_light_intensity);

    // Kreiranje model matrice: pozicioniranje i skaliranje modela
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
    model = glm::scale(model, glm::vec3(0.02f));

    shader->set_mat4("model", model);

    garage->draw(shader);

    engine::resources::Shader *shader1 = resources->shader("car");
    shader1->use();

    shader1->set_mat4("projection", graphics->projection_matrix());

    glm::mat4 view1 = graphics->camera()->view_matrix();
    shader1->set_mat4("view", view1);

    glm::vec3 camPos1 = glm::vec3(glm::inverse(view)[3]);
    shader1->set_vec3("viewPos", camPos1);

    if (!g_red_car_removed) {
        // PRVI AUTO
        shader1->set_vec3("lightColor1", glm::vec3(0.6f, 0.0f, 0.0f));
        glm::mat4 mustangModel = glm::mat4(1.0f);
        mustangModel = glm::translate(mustangModel, glm::vec3(-1.5f, 0.0f, -1.5f));
        mustangModel = glm::scale(mustangModel, glm::vec3(0.04f));
        shader1->set_mat4("model", mustangModel);
        mustang->draw(shader1);
    }
    if (!g_blue_car_removed) {
        // DRUGI AUTO
        shader1->set_vec3("lightColor1", glm::vec3(0.0f, 0.2f, 1.0f));
        glm::mat4 mustangModel1 = glm::mat4(1.0f);
        mustangModel1 = glm::translate(mustangModel1, glm::vec3(3.5f, 0.0f, -1.5f));
        mustangModel1 = glm::scale(mustangModel1, glm::vec3(0.04f));
        shader1->set_mat4("model", mustangModel1);
        mustang->draw(shader1);
    }

}

void MainController::update_camera() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    float dt = platform->dt();

    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt * 2); }
    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt * 2); }
    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt * 2); }
    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt * 2); }
    if (platform->key(engine::platform::KeyId::KEY_X).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::DOWN, dt * 2); }
    if (platform->key(engine::platform::KeyId::KEY_SPACE).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::UP, dt * 2); }
    if (platform->key(engine::platform::KeyId::KEY_L).is_down()) { camera->rotate_camera(engine::graphics::Camera::Movement::LEFT * 5, 0); }   //POMERANJE UDESNO
    if (platform->key(engine::platform::KeyId::KEY_K).is_down()) { camera->rotate_camera(-(engine::graphics::Camera::Movement::LEFT * 5), 0); }//POMERANJE ULEVO
    if (platform->key(engine::platform::KeyId::KEY_G).is_down()) { on_button_pressed(); }
    if (platform->key(engine::platform::KeyId::KEY_1).is_down()) { amplify_light(); }
    if (platform->key(engine::platform::KeyId::KEY_2).is_down()) { lower_light(); }
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

void MainController::trigger_event_a() {

    g_red_car_removed = true;
    spdlog::info("EVENT_A: Crveni auto nestaje sa scene!");

}

void MainController::trigger_event_b() {

    g_blue_car_removed = true;

    spdlog::info("EVENT_B: Plavi auto nestaje sa scene!");
}

void MainController::on_button_pressed() {

    g_action_time = std::chrono::steady_clock::now();
    g_action_triggered = true;
    g_red_car_removed = false;
    g_blue_car_removed = false;


    spdlog::info("ACTION_X: Dugme pritisnutno - pokrenut je tajmer!");


}

void MainController::update_events() {

    if (!g_action_triggered) return;

    auto now = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration<float>(now - g_action_time).count();

    if (elapsed >= 5.0f && !g_red_car_removed) { trigger_event_a(); }

    if (elapsed >= 10.0f && !g_blue_car_removed) {
        trigger_event_b();
        g_action_triggered = false;
    }


}

void MainController::lower_light() { if (g_light_intensity >= 0.1f) g_light_intensity -= 0.1f; }

void MainController::amplify_light() { if (g_light_intensity < 10.0f) g_light_intensity += 0.1f; }

void MainController::update() { update_camera(); }
}

// app