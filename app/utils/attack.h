#ifndef ATTACK_H
#define ATTACK_H

void attack_player(Player *player, Monster *monster);
void manage_player_attack(Monsters *monsters, Player *player, bool *print_entities);
void attack_monster(Monster *monster, Player *player);

#endif // ATTACK_H