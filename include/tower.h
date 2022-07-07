#ifndef _TOWER_H_
#define _TOWER_H_

#include "vector.h"
#include "enemy.h"

typedef class enemy enemy;

class game;

class tower
{
    protected:
        float                               damage;
        float                               shootingSpeed;
        float                               timeBetweenShot;

        bool                                shooting;
        bool                                bought;

        int                                 level;
        int                                 posXHUD;
        int                                 posYHUD;

        t_referential                       localRef;
        t_circle                            range;
        std::vector<t_point>                array;



        virtual void                        change_tower_color(game& game);
        virtual bool                        interact_with_enemy(std::unique_ptr<enemy>& enemy, game& game);

    public:
        tower();
        ~tower();

        void                                aim_at_enemy(enemy& enemy);
        void                                change_pos(const int& x, const int& y);
        void                                set_bought_true();
        void                                display_tower_HUD(game& game);
        void                                display_buying_tower(game& game);
        virtual void                        shoot_enemy(game& game);
        virtual void                        init_tower();
        virtual void                        upgrade(game& game);
        virtual void                        display_tower(game& game);


        t_point get_pos() const;

};


#endif