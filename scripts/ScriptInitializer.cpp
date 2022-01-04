#include "ScriptInitializer.h"

// TODO: script file parsing so scripts are not hard-coded and compiled

int numTestTargets = 0;

GameObject makeTestTarget(Game& game, float x, float y)
{
    std::string name = "Target", spriteName = "circle";
    int id = 2 + numTestTargets, health = 1, damage = 1, score = 0;
    float pos_x = x, pos_y = y;
    float rotation = 0.0f;
    float size_x = 20.0f, size_y = 20.0f;
    float vel_x = 0.0f, vel_y = 500.0f;
    float accel_x = 0.0f, accel_y = 0.0f;
    float color_r = 255.0f, color_g = 0.0f;
    float color_b = 0.0f, color_a = 1.0f;
    float minFireDelay = 0.90f;
    std::function<void(GameObject* , float)> updateFunction = [&](GameObject* o, float) 
    {
        std::string name = "Target", spriteName = "circle";
        int id = 2 + numTestTargets, health = 1, damage = 1, score = 0;
        float pos_x = o->getPosition().x + (o->getSize().x / 2), pos_y = o->getPosition().y;
        float rotation = 0.0f;
        float size_x = 20.0f, size_y = 20.0f;
        float vel_x = 0.0f, vel_y = -1000.0f;
        float accel_x = 0.0f, accel_y = 0.0f;
        float color_r = 255.0f, color_g = 0.0f;
        float color_b = 0.0f, color_a = 1.0f;
        float minFireDelay = 0.0f;
        std::function<void(GameObject*, float)> bullet_updateFunction = [&](GameObject* o, float)
        {
            //float next_vel_x = game.getPointerToPlayer()->getPosition().x - o->getPosition().x;
            //float next_vel_y = game.getPointerToPlayer()->getPosition().y - o->getPosition().y;

            //o->setVelocity(next_vel_x, next_vel_y);
        };
        std::vector<float> hitboxInfo{ size_x / 2.0f };
        GameObject bullet(name, id, spriteName, health, damage, score,
            pos_x, pos_y, rotation, size_x, size_y, vel_x, vel_y, accel_x, accel_y,
            color_r, color_g, color_b, color_a);
        bullet.setUpdateFunction(bullet_updateFunction);
        o->fire(bullet);
        std::cout << __func__ << ": " << o->getBulletsToFire().size() << std::endl;
    };
    

    numTestTargets++;

    GameObject target(name, id, spriteName, health, damage, score,
        pos_x, pos_y, rotation, size_x, size_y, vel_x, vel_y, accel_x, accel_y,
        color_r, color_g, color_b, color_a);
    target.setUpdateFunction(updateFunction);
    target.setHitboxCircle(pos_x, pos_y, size_x / 2.0f);
    return target;
}

ScriptCollection ScriptInitializer::createGameScripts()
{
    ScriptCollection collection("game script");



    return collection;
}

ScriptCollection ScriptInitializer::createTestScripts(Game& game)
{
    ScriptCollection collection("test script");
    std::vector<GameObject> objects;
    std::vector<Command> commands;

    objects.push_back(makeTestTarget(game, 400, 400));
    //objects.push_back(makeTestTarget(game, 400, 500));
    //objects.push_back(makeTestTarget(game, 500, 600));

    commands.push_back(Command(ScriptCommand::PLACE_AND_WAIT_UNTIL_DEATH, 0));
    //commands.push_back(Command(ScriptCommand::WAIT_FOR, 2));
    //commands.push_back(Command(ScriptCommand::PLACE_AND_WAIT_UNTIL_DEATH, 0));
    //commands.push_back(Command(ScriptCommand::WAIT_FOR, 2));
    //commands.push_back(Command(ScriptCommand::PLACE_AND_WAIT_UNTIL_DEATH, 0));

    collection.addScript(Script("make target", 1, objects, commands));

    return collection;
}

//std::vector<Polygon> starData{
//    Polygon(std::vector<glm::vec2> { glm::vec2(70, 0), glm::vec2(48.73, 46.09), glm::vec2(91.63, 46.09) }),
//    Polygon(std::vector<glm::vec2> { glm::vec2(140, 53.48), glm::vec2(91.63, 46.09), glm::vec2(105, 89.35) }),
//    Polygon(std::vector<glm::vec2> { glm::vec2(113.26, 140), glm::vec2(70, 116.09), glm::vec2(105, 89.35) }),
//    Polygon(std::vector<glm::vec2> { glm::vec2(26.74, 140), glm::vec2(35, 89.35), glm::vec2(70, 116.09) }),
//    Polygon(std::vector<glm::vec2> { glm::vec2(0, 53.48), glm::vec2(48.37, 46.09), glm::vec2(35, 89.35) }),
//    Polygon(std::vector<glm::vec2> { glm::vec2(70, 116.09), glm::vec2(35, 89.35), glm::vec2(48.37, 46.09), glm::vec2(91.63, 46.09), glm::vec2(105, 89.35) })
//};