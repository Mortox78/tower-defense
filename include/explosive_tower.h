#ifndef _EXPLOSIVE_TOWER_H_
#define _EXPLOSIVE_TOWER_H_

#include "tower.h"

class explosive_tower : public tower
{
    private:
        float                           timeRocketExplosion;
        float                           radDisplayRocketExplo;

        bool                            rocketExploded;
        bool                            rocketLaunched;

        t_referential                   rocketRef;
        std::vector<t_point>            rocketPatternDesign;
        t_point                         aimPos;
        t_vector2                       vecRocketAimPos;
        t_circle                        rangeRocket;
        t_circle                        rangeDisplayRocket;


        void                            add_radius_display_rocket_explosion();
        void                            reset_time_rocket_explosion();
        void                            change_tower_color(game& game) override;

    public:

        void                            display_tower(game& game) override;
        void                            display_rocket(game& game);
        void                            display_rocket_exploded(game& game);
                            
        void                            init_tower() override;
        void                            init_rocket();
        void                            move_rocket(game& game);
        void                            rocket_exploded(game& game);
        void                            shoot_rocket(t_point aimPos, game& game);
        bool                            interact_with_enemy(std::unique_ptr<enemy>& enemy, game& game) override;
        void                            upgrade(game& game) override;


        explosive_tower();
        ~explosive_tower();
};

#endif