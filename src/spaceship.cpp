#include "SpaceShip.hpp"

SpaceShip::SpaceShip(YAML::Node& config)
{
    image = LoadTexture(config["Path"]["SpaceShipImg"].as<std::string>().c_str());
    position.x = (GetScreenWidth() - image.width) / 2;
    position.y = GetScreenHeight() - image.height - 100;
    speed.x = config["Game"]["SpaceShip"]["Speed"].as<int>();;
    speed.y = 5;
    fireInterval = config["Game"]["SpaceShip"]["FireInterval"].as<float>();
    lastFireTime = 0.0;
    laserSound = LoadSound(config["Path"]["LaserSound"].as<std::string>().c_str());
    laserSpeed = config["Game"]["Laser"]["SpaceshipLaserSpeed"].as<int>();;
}

SpaceShip::~SpaceShip()
{
    UnloadTexture(image);
    UnloadSound(laserSound);
}

void SpaceShip::Draw()
{
    DrawTextureV(image, position, WHITE);
}

void SpaceShip::Update()
{
    for (auto& laser : lasers)
        laser.Update();
}

void SpaceShip::MoveLeft()
{
    position.x -= speed.x;
    if (position.x < 25)
        position.x = 25;
}

void SpaceShip::MoveRight()
{
    position.x += speed.x;
    if (position.x > GetScreenWidth() - image.width - 25)
        position.x = GetScreenWidth() - image.width - 25;
}

void SpaceShip::MoveUp()
{
    position.y -= speed.y;
    if (position.y < 0)
        position.y = 0;
}

void SpaceShip::MoveDown()
{
    position.y += speed.y;
    if (position.y > GetScreenHeight() - image.height)
        position.y = GetScreenHeight() - image.height;
}

void SpaceShip::Fire()
{
    // add delay between fires
    if (GetTime() - lastFireTime >= fireInterval)
    {
        // the laser has 4 pixel of width, so minus 2 to centerize
        lasers.emplace_back(
            Vector2{position.x + image.width/2 - 2, position.y}, laserSpeed, 1);
        lastFireTime = GetTime();
        PlaySound(laserSound);
    }
}

Rectangle SpaceShip::GetRect()
{
    return {position.x, position.y, static_cast<float>(image.width), static_cast<float>(image.height)};
}

void SpaceShip::Reset()
{
    position.x = (GetScreenWidth() - image.width) / 2;
    position.y = GetScreenHeight() - image.height - 100;
    lasers.clear();
}
