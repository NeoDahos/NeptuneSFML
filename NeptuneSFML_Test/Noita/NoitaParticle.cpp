#include "NoitaParticle.h"

const NoitaParticle NoitaParticle::EmptyParticle = { sf::Vector2f(0.f, 0.f), 0, Type::Empty, true };
const NoitaParticle NoitaParticle::SandParticle = { sf::Vector2f(0.f, -1.f), 0, Type::Sand, false };
const NoitaParticle NoitaParticle::WaterParticle = { sf::Vector2f(0.f, -1.f), 0, Type::Water, false };
const NoitaParticle NoitaParticle::WoodParticle = { sf::Vector2f(0.f, 0.f), 0, Type::Wood, true };