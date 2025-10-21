//
// Created by matfrg on 10/18/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {

class MainController : public engine::core::Controller {
    void initialize() override;

    bool loop() override;

    void draw_garage();

    void update_camera();

    void update() override;

    void begin_draw() override;

    void draw() override;

    void end_draw() override;

    void trigger_event_a();

    void trigger_event_b();

    void on_button_pressed();

    void update_events();

    void lower_light();

    void amplify_light();

public:
    std::string_view name() const override { return "app:MainController"; }
};

}// app

#endif //MAINCONTROLLER_HPP
