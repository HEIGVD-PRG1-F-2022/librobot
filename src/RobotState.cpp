#include "librobots/RobotState.h"
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

RobotState::RobotState(Robot *robot, Position pos, size_t side, unsigned energy,
                       unsigned power)
    : robot(robot), pos(pos), energy(energy), power(power) {
    robot->setConfig(side, side, energy, power);
}

string RobotState::getName() const { return robot->name(); }

Message RobotState::getAction() const { return action; }

unsigned RobotState::getPower() const { return power; }

Position RobotState::getPosition() const { return pos; }

unsigned int RobotState::getEnergy() const { return energy; }

string RobotState::getDeathCause() const {
    return deathCause;
}

bool RobotState::isDead() const { return energy == 0; }

void RobotState::actionAttack(const RobotState &attacker,
                              const Position &dest) {
    if (dest == pos) {
        if (attacker.power < energy) {
            energy -= attacker.power;
        } else {
            deathCause = "attack from " + attacker.getName();
            energy = 0;
        }
    }
}

void RobotState::actionMove(const Direction &relative) { pos += relative; }

void RobotState::actionRadar(const std::vector<Position> &positions) {
    for (auto p: positions) {
        if (pos == p) {
            continue;
        }
        updates_cache.push_back(Message::updateRobot(pos.directionTo(p)));
    }
}

void RobotState::actionBonus(const Position &bonus) {
    updates_cache.push_back(Message::updateBonus(pos.directionTo(bonus)));
}

void RobotState::actionEnergy(unsigned bonusEnergy) {
    energy += bonusEnergy;
    updates_cache.push_back(Message::updateEnergy(bonusEnergy));
}

void RobotState::actionPower(unsigned bonusPower) {
    power += bonusPower;
    updates_cache.push_back(Message::updatePower(bonusPower));
}

void RobotState::checkCollision(RobotState &other) {
    if (pos == other.pos) {
        if (energy <= other.energy) {
            other.energy -= energy;
            deathCause = "Collision with " + other.getName();
            energy = 0;
        } else {
            energy -= other.energy;
            updates_cache.push_back(Message::updateDamage(Direction(), other.energy));
            other.energy = 0;
            other.deathCause = "Collision with " + getName();
        }
    }
}

void RobotState::sendUpdate(const string &updateBoard) {
    vector<string> updates(1 + updates_cache.size());
    updates.at(0) = updateBoard;
    copy(updates_cache.begin(), updates_cache.end(), updates.begin() + 1);
    auto action_str = robot->action(updates);
    action = Message(action_str);
    switch (action.msg) {
        case MessageType::ActionMove:
            action.robots.at(0).unitary();
            break;
        case MessageType::ActionAttack:
            if (action.robots.at(0).mag() >= 3) {
                action.robots.at(0).unitary();
                action.robots.at(0) *= 2;
            }
            break;
        default:
            break;
    }
    updates_cache.clear();
}

class RobotDummy : public Robot {
public:
    RobotDummy() = default;

    void setConfig(size_t init_width, size_t init_height, unsigned init_energy,
                   unsigned init_power) override {
        // Avoid warning of unused parameters.
        (void) (init_width + init_height + init_energy + init_power);
    }

    std::string action(std::vector<std::string> updates) override {
        // Avoid warning of unused parameters.
        (void) updates;
        return "move 1,1";
    };

    [[nodiscard]] std::string name() const override { return "Random one"; }
};

void testRobotState() {
    Position p(0, 0, 10, 10);
    vector<RobotState> robots{RobotState{new RobotDummy(), p, 10, 10, 1},
                              RobotState{new RobotDummy(), p, 10, 10, 1}};
    robots.at(0).checkCollision(robots.at(1));
    cout << robots.at(0).isDead() << endl;
    cout << robots.at(1).isDead() << endl;
    robots.erase(remove_if(robots.begin(), robots.end(),
                           [](const RobotState &rs) { return rs.isDead(); }),
                 robots.end());
    cout << robots.size() << endl;
}