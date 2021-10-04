#ifndef ROBOT_DART_ROBOTS_TIAGO_HPP
#define ROBOT_DART_ROBOTS_TIAGO_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/force_torque.hpp"
#include "robot_dart/sensor/imu.hpp"
#include "robot_dart/sensor/torque.hpp"

namespace robot_dart {
    namespace robots {
        /// datasheet: https://pal-robotics.com/wp-content/uploads/2021/07/Datasheet-complete_TIAGo-2021.pdf
        class Tiago : public Robot {
        public:
            Tiago(RobotDARTSimu* simu, size_t frequency = 1000, const std::string& urdf = "tiago/tiago_steel.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"tiago_description", "tiago/tiago_description"}});

            const sensor::ForceTorque& ft_wrist() const { return *_ft_wrist; }
        protected:
            std::shared_ptr<sensor::ForceTorque> _ft_wrist;
        };
    } // namespace robots
} // namespace robot_dart
#endif
