//
// Created by matfrg on 10/18/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include <chrono>

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

private:
    bool m_action_triggered = false;
    bool m_left_car_removed = false;
    bool m_right_car_removed = false;
    float m_light_intensity = 5.0f;
    std::chrono::steady_clock::time_point m_action_time;

public:
    std::string_view name() const override { return "app:MainController"; }
};

}// app

#endif //MAINCONTROLLER_HPP
