//
// Created by matfrg on 10/18/25.
//

//#include "../include/MyApp.hpp"

#include <GuiController.hpp>
#include <MainController.hpp>
#include <MyApp.hpp>
#include <spdlog/spdlog.h>
#include <MainController.hpp>

namespace app {
//class MainController;

void MyApp::app_setup() {
    spdlog::info("App setup complited!");
    auto main_controller = register_controller<app::MainController>();
    auto gui_controller = register_controller<app::GUIController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    main_controller->before(gui_controller);

}
}// app