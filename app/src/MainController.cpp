//
// Created by matfrg on 10/18/25.
//

#include "MainController.hpp"

#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

namespace app {
void MainController::initialize() { spdlog::info("MainController initialized."); }

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) { return false; }
    return true;
}

void MainController::draw_garage() {

    //model

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    engine::resources::Model *model = resources->model("garage");

    //shader

    engine::resources::Shader *shader = resources->shader("garage");

    model->draw(shader);

}

void MainController::draw() { draw_garage(); }
}

// app