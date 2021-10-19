#include "NoitaParticle.h"

const NoitaParticle NoitaParticle::EmptyParticle = { sf::Vector2f(0.f, 0.f), 0, NoitaElementCommon::Type::Empty, true };
const NoitaParticle NoitaParticle::SandParticle = { sf::Vector2f(0.f, -1.f), 0, NoitaElementCommon::Type::Sand, false };
const NoitaParticle NoitaParticle::WaterParticle = { sf::Vector2f(0.f, -1.f), 0, NoitaElementCommon::Type::Water, false };
const NoitaParticle NoitaParticle::WoodParticle = { sf::Vector2f(0.f, 0.f), 0, NoitaElementCommon::Type::Wood, true };