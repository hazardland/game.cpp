#ifndef TEST_ENUM
#define TEST_ENUM

int RED[][3] = {{164, 0, 0}, {124, 0, 0}, {92, 4, 0}, {68, 4, 0}, -1};
int BLUE[][3] = {{12, 72, 204}, {4, 40, 160}, {0, 20, 116}, {0, 4, 76}, -1};
int GREEN[][3] = {{44, 180, 148}, {20, 132, 92}, {4, 84, 44}, {0, 40, 12}, -1};
int VIOLET[][3] = {{152, 72, 176}, {116, 44, 132}, {80, 24, 88}, {44, 8, 44}, -1};
int ORANGE[][3] = {{248, 140, 20}, {200, 96, 16}, {152, 60, 16}, {108, 32, 12}, -1};
int BLACK[][3] = {{40, 40, 60}, {28, 28, 44}, {20, 20, 32}, {12, 12, 20}, -1};
int WHITE[][3] = {{224, 224, 224}, {152, 152, 180}, {84, 84, 128}, {36, 40, 76}, -1};
int YELLOW[][3] = {{252, 252, 72}, {228, 204, 40}, {204, 160, 16}, {180, 116, 0}, -1};

const int UNIT_MOVE = 100;
const int UNIT_ATTACK = 200;

const int UNIT_STAND_UP = 1;
const int UNIT_STAND_DOWN = 2;
const int UNIT_STAND_LEFT = 30;
const int UNIT_STAND_RIGHT = 40;
const int UNIT_STAND_UP_RIGHT = UNIT_STAND_UP + UNIT_STAND_RIGHT;
const int UNIT_STAND_UP_LEFT = UNIT_STAND_UP + UNIT_STAND_LEFT;
const int UNIT_STAND_DOWN_RIGHT = UNIT_STAND_DOWN + UNIT_STAND_RIGHT;
const int UNIT_STAND_DOWN_LEFT = UNIT_STAND_DOWN + UNIT_STAND_LEFT;

const int UNIT_MOVE_UP = UNIT_STAND_UP + UNIT_MOVE;
const int UNIT_MOVE_DOWN = UNIT_STAND_DOWN + UNIT_MOVE;
const int UNIT_MOVE_LEFT = UNIT_STAND_LEFT + UNIT_MOVE;
const int UNIT_MOVE_RIGHT = UNIT_STAND_RIGHT + UNIT_MOVE;
const int UNIT_MOVE_UP_RIGHT = UNIT_STAND_UP_RIGHT + UNIT_MOVE;
const int UNIT_MOVE_UP_LEFT = UNIT_STAND_UP_LEFT + UNIT_MOVE;
const int UNIT_MOVE_DOWN_RIGHT = UNIT_STAND_DOWN_RIGHT + UNIT_MOVE;
const int UNIT_MOVE_DOWN_LEFT = UNIT_STAND_DOWN_LEFT + UNIT_MOVE;

const int UNIT_ATTACK_UP = UNIT_STAND_UP + UNIT_ATTACK;
const int UNIT_ATTACK_DOWN = UNIT_STAND_DOWN + UNIT_ATTACK;
const int UNIT_ATTACK_LEFT = UNIT_STAND_LEFT + UNIT_ATTACK;
const int UNIT_ATTACK_RIGHT = UNIT_STAND_RIGHT + UNIT_ATTACK;
const int UNIT_ATTACK_UP_RIGHT = UNIT_STAND_UP_RIGHT + UNIT_ATTACK;
const int UNIT_ATTACK_UP_LEFT = UNIT_STAND_UP_LEFT + UNIT_ATTACK;
const int UNIT_ATTACK_DOWN_RIGHT = UNIT_STAND_DOWN_RIGHT + UNIT_ATTACK;
const int UNIT_ATTACK_DOWN_LEFT = UNIT_STAND_DOWN_LEFT + UNIT_ATTACK;

#endif