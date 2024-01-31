#ifndef ENTITIES_H
#define ENTITIES_H

#include "../../utils/linkedList.h"
#include "../game_defines.h"

void mine_create(game_entity *self, list **visual_effects);
void mine_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p);
void mine_death(game_entity *self, list **entities, list **visual_effects);

void cargo_create(game_entity *self, list **visual_effects);
void cargo_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p);
void cargo_death(game_entity *self, list **entities, list **visual_effects);

void bomber_create(game_entity *self, list **visual_effects);
void bomber_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p);
void bomber_death(game_entity *self, list **entities, list **visual_effects);

void bomber_bullet_create(game_entity *self, list **visual_effects);
void bomber_bullet_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p);
void bomber_bullet_death(game_entity *self, list **entities, list **visual_effects);

void fighter_create(game_entity *self, list **visual_effects);
void fighter_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p);
void fighter_death(game_entity *self, list **entities, list **visual_effects);

void fighter_bullet_create(game_entity *self, list **visual_effects);
void fighter_bullet_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p);
void fighter_bullet_death(game_entity *self, list **entities, list **visual_effects);

void drone_create(game_entity *self, list **visual_effects);
void drone_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p);
void drone_death(game_entity *self, list **entities, list **visual_effects);

void heart_create(game_entity *self, list **visual_effects);
void heart_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p);
void heart_death(game_entity *self, list **entities, list **visual_effects);

#endif