#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/tiago.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    double dt = 0.01;
    robot_dart::RobotDARTSimu simu(dt);
    simu.set_collision_detector("fcl");

    size_t freq = 1. / dt;
    auto robot = std::make_shared<robot_dart::robots::Tiago>(freq);
    std::cout << "The model used is: [" << robot->model_filename() << "]" << std::endl;

#ifdef GRAPHIC
    robot_dart::gui::magnum::GraphicsConfiguration configuration;
    configuration.width = 1280;
    configuration.height = 960;
    configuration.bg_color = Eigen::Vector4d{1.0, 1.0, 1.0, 1.0};
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>(configuration);
    simu.set_graphics(graphics);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
    graphics->record_video("tiago_dancing.mp4");
#endif
    simu.add_checkerboard_floor();
    simu.add_robot(robot);

    simu.set_control_freq(100);
    std::vector<std::string> dofs = {"arm_1_joint",
        "arm_2_joint",
        "arm_3_joint",
        "arm_4_joint",
        "arm_5_joint",
        "torso_lift_joint"};

    Eigen::VectorXd init_positions = robot->positions(dofs);

    auto start = std::chrono::steady_clock::now();
    while (simu.scheduler().next_time() < 20. && !simu.graphics()->done()) {
        if (simu.schedule(simu.control_freq())) {
            Eigen::VectorXd delta_pos(6);
            delta_pos << sin(simu.scheduler().current_time() * 2.) + M_PI_2,
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.);
            Eigen::VectorXd commands = (init_positions + delta_pos) - robot->positions(dofs);
            robot->set_commands(commands, dofs);
        }

        simu.step_world();
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "benchmark time: " << elapsed_seconds.count() << "s\n";

    robot.reset();
    return 0;
}
