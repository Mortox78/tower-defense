#ifndef _SLOW_TOWER_H_
#define _SLOW_TOWER_H_

#include "tower.h"

class slow_tower : public tower
{
    private:
        t_point                 posEnemyLock;

        void                    change_tower_color(game& game) override;
        bool                    interact_with_enemy(std::unique_ptr<enemy>& enemy, game& game) override;

    public:

        slow_tower();
        ~slow_tower();
    
        void                    init_tower() override;
        void                    upgrade(game& game) override;
        void                    display_tower(game& game) override;
};


#endif