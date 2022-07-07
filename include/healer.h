#ifndef _HEALER_H_
#define _HEALER_H_

#include <iostream>
#include <vector>
#include "vector.h"
#include "game_manager.h"


class healer : public enemy
{
    private:
        float                       healingSpeed;
        float                       timeBetweenHeal;

        int                         healPower;

        bool                        healing;
        bool                        timePrevHeal;

        t_point                     posEnemyLock;
        t_circle                    healRange;



        bool                        interact_with_enemy(std::unique_ptr<enemy>& enemy);
        void                        heal_himself();

    public:
        healer();
        ~healer();

        void                        heal(game& game);
        void                        display(const game& game) override;
        void                        update_enemy(game& game) override;
};


#endif