# LibRobots

Une librairie pour le projet des robots.
Ici on va ajouter au fur et à mesure des outils pour faciliter la programmation des robots.
Pour le moment il y a la définition de la class virtuelle pour les robots, et un outil pour
séparer un string en plusieurs parties.

Pour utiliser la librairie, ajoutez les lignes suivantes dans votre `CMakeLists.txt`:

```cmake
include(FetchContent) # once in the project to include the module

FetchContent_Declare(librobots
        GIT_REPOSITORY https://github.com/HEIGVD-PRG1-F-2022/librobots.git
        GIT_TAG v0.2.8
        )
FetchContent_MakeAvailable(librobots)

# Here are your other declarations, like `add_executable`

# Replace `PROJECT` with the name of your executable
target_link_libraries(PROJECT PRIVATE librobots)
```

Après vous pouvez utiliser la librairie en l'important et déclarant votre classe comme
implémentant la classe abstraite `Robot`:

```c++
#include <librobots.h>

class MyRobot1 : public Robot {
    // Implement all virtual methods here
}

int main(){
    vector<Robot> robots = {new MyRobot1()};
    // Utiliser le vecteur 'robots'
    return 0;
}
```

Pour un exemple plus complet, voir [exemple](example/main.cpp).

# CHANGELOG

- v0.2.8
  - Rename "boni" element of Message::MESSAGE_STRINGS to "bonus"
- v0.2.7
  - Catches all exceptions from robots 
- v0.2.6
  - Adds the boni to `RobotState::actionRadar`
- v0.2.5
  - Correctly updating the attack message in `RobotState`
- v0.2.4
  - Rename `Direction::getY` to `Direction::getdY` and marked `Direction::getY` as deprecated 
  - Catch exception from `Message(str)` in `RobotState::sendUpdate`
- v0.2.3 
  - Add `Direction::turn` and fixes `RobotState::CheckCollision` to correctly send messages 
- v0.2.2
  - Added * and / to Direction, as well as 'neg'
  - RobotState now has a deathCause and verifies the return actions from the robot
  - Message now also shows the boni in the board
  - Message returns the type as a string
- v0.2.1
 - fixing includes 
- v0.2.0 
  - ajout de librairies pour le projet  
- v0.1 
  - version initiale

# Classes disponibles

Pour vous aider dans vos projets, j'ai ajouté les classes suivantes:

## Bonus

[Bonus](include/librobots/Bonus.h):

The bonus class is very simply and probably could be a `struct`. It exposes its
variables as public members.

## Direction

[Direction](include/librobots/Direction.h):

A direction towards another object. It can be used together with the Position class.

## Message

[Message](include/librobots/Message.h):

A message is an interpretation of the string passed between the game
engine and the robot.
It also has static methods that allow to create a string based on the
values.
Instead of getters, all variables are public.

## Position

[Position](include/librobots/Position.h):

Represents a Position either with wraparound or without. If a width and height
are given, the + and - operations will wrap around.

## RobotState

[RobotState](include/librobots/RobotState.h):

The RobotState holds a robot together with all its necessary values:
its position, energy, and power.
It collects updates in a cache until the game decides to update the robots.
The resulting action is stored in RobotState for the game to retrieve at a
later moment.
