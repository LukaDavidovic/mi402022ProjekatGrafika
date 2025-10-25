#include <spdlog/spdlog.h>
#include <MainController.hpp>

#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//bool g_action_triggered = false;
//bool g_left_car_removed = false;
//bool g_right_car_removed = false;
//Sfloat g_light_intensity = 5.0f;

//std::chrono::steady_clock::time_point g_action_time;

namespace app {

void MainController::initialize() { engine::graphics::OpenGL::enable_depth_testing(); }

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) { return false; }
    return true;
}

void MainController::draw_garage() {

    update_events();

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model *garage = resources->model("garage");


    //shader

    engine::resources::Shader *shader = resources->shader("garage");

    shader->use();

    std::unordered_map<std::string, engine::resources::Texture *> garage_textures = {

            {"albedoMap", resources->texture("GARAGE_FLOOR_albedo")},
            {"aoMap", resources->texture("GARAGE_FLOOR_ao")},
            {"bumpMap", resources->texture("GARAGE_FLOOR_bump")},
            {"normalMap", resources->texture("GARAGE_FLOOR_normal")},
            {"roughnessMap", resources->texture("GARAGE_FLOOR_roughness")}

    };

    shader->set_mat4("projection", graphics->projection_matrix());
    glm::mat4 view = graphics->camera()->view_matrix();
    shader->set_mat4("view", view);

    glm::vec3 camPos = glm::vec3(glm::inverse(view)[3]);
    shader->set_vec3("viewPos", camPos);

    shader->set_vec3("lightPos1", glm::vec3(2.5f, -2.5f, -5.0f));
    shader->set_vec3("lightColor1", glm::vec3(1.0f, 1.0f, 1.0f));// belo

    shader->set_vec3("lightDir1", glm::vec3(0.0f, 0.0f, -5.0f));
    shader->set_vec3("lightColor2", glm::vec3(1.0f, 1.0f, 1.0f));

    float constantAttenuation = 1.0f;
    float linearAttenuation = 0.09f;
    float quadraticAttenuation = 0.032f;

    shader->set_float("constantAttenuation", constantAttenuation);
    shader->set_float("linearAttenuation", linearAttenuation);
    shader->set_float("quadraticAttenuation", quadraticAttenuation);
    shader->set_float("lightIntensity", m_light_intensity);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
    model = glm::scale(model, glm::vec3(0.02f));

    graphics->draw_model(garage, shader, model, garage_textures);

    garage->draw(shader);


    engine::resources::Model *mustang = resources->model("mustang");
    engine::resources::Shader *shader1 = resources->shader("car");
    shader1->use();

    engine::resources::Texture *carTexture = resources->texture("texturecar");

    std::unordered_map<std::string, engine::resources::Texture *> car_textures = {

            {"carTexture", resources->texture("texturecar")}
    };

    shader1->set_mat4("projection", graphics->projection_matrix());
    glm::mat4 view1 = graphics->camera()->view_matrix();
    shader1->set_mat4("view", view1);
    glm::vec3 camPos1 = glm::vec3(glm::inverse(view)[3]);
    shader1->set_vec3("viewPos", camPos1);

    if (!m_left_car_removed) {
        glm::mat4 Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(-1.0f, 0.0f, -2.8f));
        Model = glm::rotate(Model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        Model = glm::scale(Model, glm::vec3(4.0f));

        graphics->draw_model(mustang, shader1, Model, car_textures);

        mustang->draw(shader1);
    }
    if (!m_right_car_removed) {
        // DRUGI AUTO
        glm::mat4 Model1 = glm::mat4(1.0f);
        Model1 = glm::translate(Model1, glm::vec3(3.0f, 0.0f, -2.8f));
        Model1 = glm::rotate(Model1, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        Model1 = glm::scale(Model1, glm::vec3(4.0f));

        graphics->draw_model(mustang, shader1, Model1, car_textures);

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

    m_left_car_removed = true;
    spdlog::info("EVENT_A: Prvi auto nestaje sa scene!");

}

void MainController::trigger_event_b() {

    m_right_car_removed = true;

    spdlog::info("EVENT_B: Drugi auto nestaje sa scene!");
}

void MainController::on_button_pressed() {

    m_action_time = std::chrono::steady_clock::now();
    m_action_triggered = true;
    m_left_car_removed = false;
    m_right_car_removed = false;


    spdlog::info("ACTION_X: Dugme pritisnutno - pokrenut je tajmer!");


}

void MainController::update_events() {

    if (!m_action_triggered) return;

    auto now = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration<float>(now - m_action_time).count();

    if (elapsed >= 5.0f && !m_left_car_removed) { trigger_event_a(); }

    if (elapsed >= 10.0f && !m_right_car_removed) {
        trigger_event_b();
        m_action_triggered = false;
    }


}

void MainController::lower_light() { if (m_light_intensity >= 0.1f) m_light_intensity -= 0.1f; }

void MainController::amplify_light() { if (m_light_intensity < 10.0f) m_light_intensity += 0.1f; }

void MainController::update() { update_camera(); }
}

