#include "robot_dart.hpp"

#include <pybind11/eigen.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <robot_dart/robot_dart_simu.hpp>

namespace robot_dart {
    namespace python {
        void py_simu(py::module& m)
        {
            using namespace robot_dart;
            using Descriptor = descriptor::BaseDescriptor;

            // Descriptor class
            class PyDescriptor : public Descriptor {
            public:
                using Descriptor::BaseDescriptor;

                /* Trampolines */
                void operator()() override
                {
                    PYBIND11_OVERLOAD_PURE_NAME(
                        void, /* return type */
                        descriptor::BaseDescriptor, /* parent class */
                        "__call__", /* name in python */
                        operator(), /* name in C++ */
                        /* arguments */
                    );
                }
            };

            class PublicistDescriptor : public Descriptor {
            public:
                using Descriptor::_desc_period;
                using Descriptor::_simu;
            };

            py::class_<Descriptor, PyDescriptor, std::shared_ptr<Descriptor>>(m, "Descriptor")
                .def(py::init<size_t>(),
                    py::arg("desc_dump") = 1)

                .def_readwrite("_desc_period", &PublicistDescriptor::_desc_period)
                .def_readonly("_simu", &PublicistDescriptor::_simu)

                .def("desc_dump", &Descriptor::desc_dump)
                .def("set_desc_dump", &Descriptor::set_desc_dump)

                .def("__call__", &Descriptor::operator());

            // TextData
            using simu::TextData;
            py::class_<TextData, std::shared_ptr<TextData>>(m, "TextData")
                .def(py::init<const std::string&, const Eigen::Affine2d&, const Eigen::Vector4d&, std::uint8_t, bool, const Eigen::Vector4d&, double>(),
                    py::arg("text"),
                    py::arg("transformation") = Eigen::Affine2d::Identity(),
                    py::arg("color") = Eigen::Vector4d(1, 1, 1, 1),
                    py::arg("alignment") = (1 | 3 << 3),
                    py::arg("draw_background") = false,
                    py::arg("background_color") = Eigen::Vector4d(0, 0, 0, 0.75),
                    py::arg("font_size") = 28.)

                .def_readwrite("text", &TextData::text)
                .def_readwrite("transformation", &TextData::transformation)
                .def_readwrite("color", &TextData::color)
                .def_readwrite("alignment", &TextData::alignment)
                .def_readwrite("draw_background", &TextData::draw_background)
                .def_readwrite("background_color", &TextData::background_color)
                .def_readwrite("font_size", &TextData::font_size);

            // RobotDARTSimu class
            py::class_<RobotDARTSimu>(m, "RobotDARTSimu")
                .def(py::init<double>(),
                    py::arg("timestep") = 0.015)

                .def("run", &RobotDARTSimu::run,
                    py::arg("max_duration") = 5.,
                    py::arg("reset_commands") = false)
                .def("step_world", &RobotDARTSimu::step_world,
                    py::arg("reset_commands") = false)
                .def("step", &RobotDARTSimu::step,
                    py::arg("reset_commands") = false)

                .def("scheduler", (Scheduler & (RobotDARTSimu::*)(void)) & RobotDARTSimu::scheduler, py::return_value_policy::reference)
                .def("schedule", &RobotDARTSimu::schedule)

                .def("physics_freq", &RobotDARTSimu::physics_freq)

                .def("control_freq", &RobotDARTSimu::control_freq)
                .def("set_control_freq", &RobotDARTSimu::set_control_freq)

                .def("graphics_freq", &RobotDARTSimu::graphics_freq)
                .def("set_graphics_freq", &RobotDARTSimu::set_graphics_freq)

                .def("graphics", &RobotDARTSimu::graphics)
                .def("set_graphics", &RobotDARTSimu::set_graphics, py::keep_alive<2, 1>())

                .def("world", &RobotDARTSimu::world)

                .def("add_descriptor", (void (RobotDARTSimu::*)(const std::shared_ptr<descriptor::BaseDescriptor>&)) & RobotDARTSimu::add_descriptor, py::keep_alive<2, 1>(),
                    py::arg("desc_dump") = 1)
                .def("descriptors", &RobotDARTSimu::descriptors)
                .def("descriptor", &RobotDARTSimu::descriptor)

                .def("remove_descriptor", (void (RobotDARTSimu::*)(const std::shared_ptr<descriptor::BaseDescriptor>&)) & RobotDARTSimu::remove_descriptor)
                .def("remove_descriptor", (void (RobotDARTSimu::*)(size_t)) & RobotDARTSimu::remove_descriptor)
                .def("clear_descriptors", &RobotDARTSimu::clear_descriptors)

                .def("add_sensor", (void (RobotDARTSimu::*)(const std::shared_ptr<sensor::Sensor>&)) & RobotDARTSimu::add_sensor,
                    py::keep_alive<2, 1>(),
                    py::arg("sensor"))
                .def("sensors", &RobotDARTSimu::sensors)
                .def("sensor", &RobotDARTSimu::sensor)

                .def("remove_sensor", (void (RobotDARTSimu::*)(const std::shared_ptr<sensor::Sensor>&)) & RobotDARTSimu::remove_sensor)
                .def("remove_sensor", (void (RobotDARTSimu::*)(size_t)) & RobotDARTSimu::remove_sensor)
                .def("remove_sensors", &RobotDARTSimu::remove_sensors,
                    py::arg("type"))
                .def("clear_sensors", &RobotDARTSimu::clear_sensors)

                .def("timestep", &RobotDARTSimu::timestep)
                .def("set_timestep", &RobotDARTSimu::set_timestep)

                .def("gravity", &RobotDARTSimu::gravity)
                .def("set_gravity", &RobotDARTSimu::set_gravity)

                .def("stop_sim", &RobotDARTSimu::stop_sim,
                    py::arg("disable") = true)
                .def("halted_sim", &RobotDARTSimu::halted_sim)

                .def("num_robots", &RobotDARTSimu::num_robots)
                .def("robots", &RobotDARTSimu::robots)
                .def("robot", &RobotDARTSimu::robot)
                .def("robot_index", &RobotDARTSimu::robot_index)

                .def("add_robot", &RobotDARTSimu::add_robot, py::keep_alive<2, 1>())
                .def("add_visual_robot", &RobotDARTSimu::add_visual_robot, py::keep_alive<2, 1>())
                .def("remove_robot", (void (RobotDARTSimu::*)(const std::shared_ptr<Robot>&)) & RobotDARTSimu::remove_robot)
                .def("remove_robot", (void (RobotDARTSimu::*)(size_t)) & RobotDARTSimu::remove_robot)
                .def("clear_robots", &RobotDARTSimu::clear_robots)

                .def("enable_text_panel", &RobotDARTSimu::enable_text_panel,
                    py::arg("enable") = true,
                    py::arg("font_size") = -1)
                .def("text_panel_text", &RobotDARTSimu::text_panel_text)
                .def("set_text_panel", &RobotDARTSimu::set_text_panel,
                    py::arg("str") = "")

                .def("enable_status_bar", &RobotDARTSimu::enable_status_bar,
                    py::arg("enable") = true,
                    py::arg("font_size") = -1)
                .def("status_bar_text", &RobotDARTSimu::status_bar_text)

                .def("add_text", &RobotDARTSimu::add_text, py::return_value_policy::reference,
                    py::arg("text"),
                    py::arg("transformation") = Eigen::Affine2d::Identity(),
                    py::arg("color") = Eigen::Vector4d(1, 1, 1, 1),
                    py::arg("alignment") = (1 | 3 << 3),
                    py::arg("draw_bg") = false,
                    py::arg("bg_color") = Eigen::Vector4d(0, 0, 0, 0.75),
                    py::arg("font_size") = 28)

                .def("add_floor", &RobotDARTSimu::add_floor,
                    py::arg("floor_width") = 10.,
                    py::arg("floor_height") = 0.1,
                    py::arg("tf") = Eigen::Isometry3d::Identity(),
                    py::arg("floor_name") = "floor")

                .def("add_checkerboard_floor", &RobotDARTSimu::add_checkerboard_floor,
                    py::arg("floor_width") = 10.,
                    py::arg("floor_height") = 0.1,
                    py::arg("size") = 1.,
                    py::arg("tf") = Eigen::Isometry3d::Identity(),
                    py::arg("floor_name") = "checkerboard_floor",
                    py::arg("first_color") = dart::Color::White(1.),
                    py::arg("second_color") = dart::Color::Gray(1.))

                .def("set_collision_detector", &RobotDARTSimu::set_collision_detector)
                .def("collision_detector", &RobotDARTSimu::collision_detector)

                .def("set_collision_mask", (void (RobotDARTSimu::*)(size_t, uint16_t)) & RobotDARTSimu::set_collision_mask)
                .def("set_collision_mask", (void (RobotDARTSimu::*)(size_t, const std::string&, uint16_t)) & RobotDARTSimu::set_collision_mask)
                .def("set_collision_mask", (void (RobotDARTSimu::*)(size_t, size_t, uint16_t)) & RobotDARTSimu::set_collision_mask)

                .def("collision_mask", (uint16_t(RobotDARTSimu::*)(size_t, const std::string&)) & RobotDARTSimu::collision_mask)
                .def("collision_mask", (uint16_t(RobotDARTSimu::*)(size_t, size_t)) & RobotDARTSimu::collision_mask)

                .def("remove_collision_mask", (void (RobotDARTSimu::*)(size_t)) & RobotDARTSimu::remove_collision_mask)
                .def("remove_collision_mask", (void (RobotDARTSimu::*)(size_t, const std::string&)) & RobotDARTSimu::remove_collision_mask)
                .def("remove_collision_mask", (void (RobotDARTSimu::*)(size_t, size_t)) & RobotDARTSimu::remove_collision_mask)

                .def("remove_all_collision_masks", &RobotDARTSimu::remove_all_collision_masks);
        }
    } // namespace python
} // namespace robot_dart
