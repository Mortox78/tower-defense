#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <iostream>
#include <vector>
#include "vector.h"
#include "game_manager.h"

class game;

class enemy
{
    private:
        t_circle circleEnemyDisplay;

    protected:
        float                   speed;
        float                   definedSpeed;
        float                   slowTime;

        bool                    alive;
        bool                    appeared;
        bool                    slowed;

        int                     damage;
        int                     health;
        int                     counterPattern;
        int                     definedHealth;
        
        t_vector2               vecMove;
        t_point                 posPoint;
        std::vector<t_point>    pattern;



        bool                    reach_point_pattern()       const;
        void                    init_movement_pattern();
        void                    manage_slow(game& game);
        void                    hit_castle(game& game);

    public:
        int identifier;

        enemy();
        ~enemy();


        void                    take_damage(const int& damageReceived, game& game);
        void                    take_heal(const int& healReceived);
        void                    move(game& game);
        void                    die();
        void                    spawn();
        void                    slow_down();
        virtual void            display(const game& game);
        virtual void            update_enemy(game& game);


        bool                    get_alive()     const;
        bool                    get_appeared()  const;
        int                     get_health()    const;
        t_point                 get_pos()       const;
};

#endif